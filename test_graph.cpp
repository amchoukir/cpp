#include "graph.hpp"

#include <iostream>
using namespace std;

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(node)
{
};

TEST(node, create)
{
    graph::node a;
    graph::node b;
    CHECK_EQUAL(a.get_id(), 0);
    CHECK_EQUAL(a.has_neighbor(b), false);
    CHECK_EQUAL(a.has_neighbors(), false);
    CHECK_EQUAL(a.get_neighbors().empty(), true);
    CHECK(a == b);
};

TEST(node, id)
{
    graph::node a;
    a.set_id(1);
    CHECK_EQUAL(a.get_id(), 1);
};

TEST(node, neighbor)
{
    graph::node a;
    graph::node b(1);
    a.add_neighbor(b);
    CHECK_EQUAL(a.has_neighbors(), true);
    CHECK_EQUAL(b.has_neighbors(), false);
    CHECK_EQUAL(a.has_neighbor(b), true);
    CHECK_EQUAL(b.has_neighbor(a), false);
    CHECK(a != b);
    auto l= a.get_neighbors();
    CHECK_EQUAL(l.size(), 1);
    auto iter = find_if(l.begin(), l.end(), graph::compare_node_wrapper(b));
    CHECK(iter != l.end());
    graph::node& ref = *iter;
    CHECK_EQUAL(ref.get_id(), 1);
}

TEST(node, neighbors)
{
    graph::node a;
    graph::node b(1);
    graph::node c(2);
    a.add_neighbor(b);
    a.add_neighbor(c);
    CHECK_EQUAL(a.get_neighbors().size(), 2);
    a.remove_neighbor(b);
    auto l = a.get_neighbors();
    auto iter = find_if(l.begin(), l.end(), graph::compare_node_wrapper(c));
    CHECK(iter != l.end());
    graph::node& ref = *iter;
    CHECK_EQUAL(ref.get_id(), 2);
};


TEST(node, alloc)
{
    // Here we mainly want to see if they are memory leaks
    // so the checks are redundant
    auto a = graph::node_ptr(new graph::node);
    auto b = graph::node_ptr(new graph::node(1));
    a->add_neighbor(*b);
    CHECK_EQUAL(a->has_neighbor(*b), true);
    auto l = a->get_neighbors();
    auto iter = find_if(l.begin(), l.end(), graph::compare_node_wrapper(*b));
    CHECK(iter != l.end());
    graph::node& ref = *iter;
    CHECK_EQUAL(ref.get_id(), 1);
}

TEST_GROUP(edge)
{
};

TEST(edge, create)
{
    graph::node a;
    graph::node b(1);
    graph::edge e(a, b);
    CHECK_EQUAL(e.is_edge(a, b), true);
    CHECK_EQUAL(e.is_edge(b, a), true);
    auto p = e.get_edge();
    CHECK(p.first.get() == a);
    CHECK(p.second.get() == b);
    CHECK_EQUAL(e.get_cost(), 0);
};

TEST(edge, cost)
{
    graph::node a;
    graph::node b(1);
    graph::edge e(a, b, 10);
    CHECK_EQUAL(e.get_cost(), 10);
    e.set_cost(20);
    CHECK_EQUAL(e.get_cost(), 20);
};

TEST(edge, alloc)
{
    auto a = graph::node_ptr(new graph::node);
    auto b = graph::node_ptr(new graph::node(1));
    auto e = graph::edge_ptr(new graph::edge(*a, *b));
    CHECK_EQUAL(e->is_edge(*a, *b), true);
    CHECK_EQUAL(e->is_edge(*b, *a), true);
    auto p = e->get_edge();
    CHECK(p.first.get() == *a);
    CHECK(p.second.get() == *b);
    CHECK_EQUAL(e->get_cost(), 0);
};

TEST_GROUP(graph)
{
};

TEST(graph, create)
{
    graph::node a;
    graph::node b(1);
    a.add_neighbor(b);
    graph g;
    CHECK_EQUAL(a.has_neighbor(b), true);
    CHECK_EQUAL(g.node_count(), 0);
    CHECK_EQUAL(g.edge_count(), 0);
    CHECK_EQUAL(g.adjacent(a, b), false); // We did not add a nor b to the graph
    CHECK_EQUAL(g.get_nodes().empty(), true);
    CHECK_EQUAL(g.get_edges().empty(), true);
};

TEST(graph, node)
{
    graph g;
    g.add_node();
    CHECK_EQUAL(g.node_count(), 1);
    g.add_node();
    CHECK_EQUAL(g.node_count(), 2);
    auto& l = g.get_nodes();
    auto iter = l.begin();
    auto& n1 = *iter;
    CHECK_EQUAL(n1->get_id(), 0);
    CHECK_EQUAL(g.get_node_value(*n1), 0);
    g.set_node_value(*n1, 2);
    CHECK_EQUAL(n1->get_id(), 2);
    CHECK_EQUAL(g.get_node_value(*n1), 2);
    ++iter;
    auto& n2 = *iter;
    CHECK_EQUAL(n2->get_id(), 1);
    CHECK_EQUAL(g.get_node_value(*n2), 1);
    g.delete_node(*n1);
    CHECK_EQUAL(g.node_count(), 1);
    g.delete_node(*n2);
    CHECK_EQUAL(g.node_count(), 0);
    graph::node dummy;
    g.delete_node(dummy);
    CHECK_EQUAL(g.node_count(), 0);
};

TEST(graph, edge)
{
    graph g;
    auto& a = g.add_node();
    auto& b = g.add_node();
    auto& e = g.add_edge(a, b);
    CHECK_EQUAL(g.get_edge_value(e), 0);
    CHECK_EQUAL(e.get_cost(), 0);
    CHECK_EQUAL(g.node_count(), 2);
    CHECK_EQUAL(g.edge_count(), 1);
    CHECK(e.is_edge(a, b));
    CHECK(a.has_neighbor(b));
    CHECK(b.has_neighbor(a));
    g.set_edge_value(e, 10);
    CHECK_EQUAL(g.get_edge_value(e), 10);
    CHECK_EQUAL(e.get_cost(), 10);
    g.delete_edge(a, b);
    CHECK_EQUAL(g.node_count(), 2);
    CHECK_EQUAL(g.edge_count(), 0);
};

TEST(graph, gen)
{
    size_t size = 100;
    double density = 0.1;
    int min_cost = 0;
    int max_cost = 10;
    auto g = graph::generate_graph(size,
                                   density,
                                   min_cost,
                                   max_cost);
    CHECK_EQUAL(g->node_count(), 100);

    cout << "g->edge_count() = " << g->edge_count() << endl;
    size_t expectation = size * size * density;
    cout << "expectation = " << expectation << endl;
    cout << "computed density = " << static_cast<double>(g->edge_count()) / (size * size) << endl;
    CHECK(g->edge_count() < (2 * expectation));
    CHECK(g->edge_count() > (0.5 * expectation));
};

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

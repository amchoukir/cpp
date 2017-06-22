#include "graph.hpp"
#include <algorithm>
#include <typeinfo>

using namespace std;

size_t graph::node_count()
{
    return _node_count;
}

size_t graph::edge_count()
{
    return _edge_count;
}

bool graph::adjacent(node& x, node& y)
{
    auto iter = get_node_iterator(x);
    if (iter == _nodes.end()) {
        return false;
    }
    return (*iter)->has_neighbor(y);
}

graph::node& graph::add_node(int id)
{
    auto p = node_ptr(new node(id));
    auto& n = *p;
    _nodes.push_back(move(p));
    ++_node_count;
    return n;
}

graph::node& graph::add_node()
{
    int id = get_id();
    return add_node(id);
}
void graph::delete_node(node& x)
{
    auto iter = get_node_iterator(x);
    if (iter == _nodes.end()) {
        return;
    }
    _nodes.remove(*iter);
    --_node_count;
}

bool graph::has_node(node& x)
{
    return get_node_iterator(x) != _nodes.end();
}

list<graph::node_ptr>::iterator graph::get_node_iterator(node& x)
{
    return find_if (_nodes.begin(),
                        _nodes.end(),
                        compare_node_ptr(x)
                        );
}

graph::edge& graph::add_edge(node& x, node& y, int cost)
{
    auto ix = get_node_iterator(x);
    auto iy = get_node_iterator(y);
    (*ix)->add_neighbor(**iy);
    (*iy)->add_neighbor(**ix);
    auto p = edge_ptr(new edge(**ix, **iy, cost));
    auto& e = *p;
    _edges.push_back(move(p));
    ++_edge_count;
    return e;
}

void graph::delete_edge(node& x, node& y)
{
    for (auto& e: _edges) {
        if (e->is_edge(x, y)) {
            _edges.remove(e);
            x.remove_neighbor(y);
            y.remove_neighbor(x);
            --_edge_count;
            return;
        }
    }
}

bool graph::has_edge(node& x, node& y)
{
    return get_edge_iterator(x, y) != _edges.end();
}

list<graph::edge_ptr>::iterator graph::get_edge_iterator(node& x, node& y)
{
    auto iter = find_if(_edges.begin(),
                        _edges.end(),
                        [&](const edge_ptr& i)
                        {
                            return i->is_edge(x, y);
                        }
                        );
    return iter;
}

int graph::get_node_value(node& x)
{
    return x.get_id();
}

void graph::set_node_value(node& x, int id)
{
    x.set_id(id);
}

int graph::get_edge_value(edge& e)
{
    return e.get_cost();
}

void graph::set_edge_value(edge& e, int cost)
{
    e.set_cost(cost);
}

graph::graph_ptr graph::generate_graph(size_t size, double density, int min_cost, int max_cost)
{
    auto g = graph_ptr(new graph);
    auto p = density_generator(density);
    auto c = cost_generator(min_cost, max_cost);
    
    // Create nodes first
    for (size_t i = 0; i < size; ++i) {
        g->add_node();
    }

    // Now add edges
    for (auto& n1: g->_nodes) {
        for (auto& n2: g->_nodes) {
            if ( (*n1 != *n2) && (!n1->has_neighbor(*n2)) && p() ) {
                g->add_edge(*n1, *n2, c());
            }
        }
    }
    return g;
}

bool graph::edge::is_edge(node& x, node& y)
{
    node& first = _edge.first;
    node& second = _edge.second;
    return (  (first == x && second == y)
           || (first == y && second == x) );
}

void graph::node::add_neighbor(node& x)
{
    _neighbors.push_back(x);
}

void graph::node::remove_neighbor(node& x)
{
    _neighbors.remove_if(compare_node_wrapper(x));
}

bool graph::node::has_neighbor(node& x)
{
    auto iter = find_if(_neighbors.begin(), _neighbors.end(), compare_node_wrapper(x));
    return iter != _neighbors.end();
}

void graph::node::print_neighbors()
{
    for (auto n: _neighbors) {
        cout << n << ", ";
    }
    cout << endl;
}

ostream& operator<<(ostream& out, graph::node& n)
{
    return out << "N(" << n.get_id() << ")";
}

ostream& operator<<(ostream& out, graph::edge& e)
{
    auto& p = e.get_edge();
    return out << p.first.get() << " <-" << e.get_cost() <<"-> " << p.second.get();
}

ostream& operator<<(ostream& out, graph& g)
{
    out << endl;
    out << "Nodes: " << endl;
    for (auto& n: g.get_nodes()) {
        out << *n << endl;
    }
    out << endl;
    out << "Edges: " << endl;
    for (auto& e: g.get_edges()) {
        out << *e << endl;
    }
    return out;
}


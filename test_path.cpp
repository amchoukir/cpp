#include "path.hpp"
#include "graph.hpp"
#include <stdexcept>

using namespace std;

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(path)
{
};

TEST(path, create)
{
    // a <-> b <-> c <-> d
    graph::node a;
    graph::node b(1);
    path p(a);
    CHECK(p.get_node() == a);
    CHECK(p.get_predecessor() == nullptr);
    CHECK_EQUAL(p.get_cost(), 0);
    path p2(b, p);
    CHECK(p2.get_node() == b);
    CHECK(p2.get_predecessor()->get_node() == a);
    CHECK_EQUAL(p2.get_cost(), 0);
    graph::node c(2);
    path p3(c, p2);
    CHECK(p3.get_node() == c);
    CHECK(p3.get_predecessor()->get_node() == b);
    CHECK_EQUAL(p3.get_cost(), 0);
    graph::edge e(b, c);
    path p4(e, p2); // e2: b <-> c, p: a <-> b
    CHECK(p4.get_node() == c);
    CHECK(p4.get_predecessor()->get_node() == b);
    CHECK_EQUAL(p4.get_cost(), 0);
    graph::node d(3);
    graph::edge e2(c, d);
    CHECK_THROWS(runtime_error, {path p5(e2, p);});
}

TEST(path, cost)
{
    // a <-1-> b <-2-> c
    graph::node a;
    graph::node b(1);
    graph::node c(2);
    graph::edge e(b, c, 2);
    path p(a);
    CHECK_EQUAL(p.get_cost(), 0);
    path p2(b, p);
    p2.set_cost(1);
    CHECK_EQUAL(p2.get_cost(), 1);
    path p3(c, p2, 2);
    CHECK_EQUAL(p3.get_cost(), 3);
    path p4(e, p2);
    CHECK_EQUAL(p4.get_cost(), 3);
};

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

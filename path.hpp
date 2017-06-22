#ifndef __PATH__
#define __PATH__
// C++ includes
#include "graph.hpp"
#include <iostream>
#include <stdexcept>

// We need a type for the shortest path segment
class path;
std::ostream& operator<<(std::ostream& out, path& p);
class path
{
    public:
        path(graph::node& n, path* p = nullptr, int cost = 0)
            : _node(n),
              _predecessor(p)
        {
            _cost = cost;
            if (p) {
                _cost += p->get_cost();
            }
        }
        path(graph::node& n, path& p, int cost = 0)
            : _node(n),
              _predecessor(&p),
              _cost(cost + p.get_cost())
        {}
        path(graph::edge& e, path& p)
            : _node(e.get_edge().second),
              _predecessor(&p),
              _cost(e.get_cost() + p.get_cost())
        {
            if (e.get_edge().first.get() != p.get_node()) {
                throw std::runtime_error("Edge and path predecessor mismatch");
            }
        }
        path() = delete;
        void set_predecessor(path& p);
        path* get_predecessor();
        graph::node& get_node();
        void set_cost(int cost);
        int get_cost();
        static void print_full_path(std::ostream& out, path& start);
    private:
        graph::node_ref _node;
        path* _predecessor;
        int _cost;
};

#endif // __PATH__

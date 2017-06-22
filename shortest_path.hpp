#ifndef __SHORTEST_PATH__
#define __SHORTEST_PATH__

#include "graph.hpp"
#include "path.hpp"

#include <iostream>
#include <memory> // For unique_ptr, shared_ptr
#include <unordered_map>
#include <vector>

class shortest_path
{
    public:
        shortest_path(graph& g) : _g(g), _ran(false), _paths(){compute_paths();}
        path* get_path(graph::node& n1, graph::node& n2);
        friend std::ostream& operator<<(std::ostream& out, shortest_path& s);
    private:
        // I tried using unique_ptr here but priority_queue
        // does not provide access to non const version
        // of the contained object so it is impossible to
        // move it
        typedef std::shared_ptr<path> path_ptr;
        struct node_ref_hash
        {
            size_t operator()(const graph::node_ref& n) const;
        };
        struct node_ref_compare
        {
            bool operator()(const graph::node_ref& n1, const graph::node_ref& n2) const
            {
                return n1.get() == n2.get();
            }
        };
        struct open_set_order
        {
            bool operator()(const path_ptr& p1, const path_ptr& p2) const
            {
                return p1->get_cost() > p2->get_cost();
            }
        };
        void compute_paths();
        graph& _g;
        bool _ran;
        // WARNING: Never use std::priority_queue
        // it is pure garbage. There is no way to
        // know if an element is in the queue. There
        // is no way to change the priority of an
        // element. Having access to the underlying
        // container could help work around the issue
        // but there is again no access to the underlying
        // container. So you are better off implementing
        // your own priority queue (heap) or use
        // vector and make_heap
        typedef std::vector<path_ptr> open_set;
        typedef std::unordered_map<graph::node_ref,
                              path_ptr,
                              node_ref_hash,
                              node_ref_compare> node_paths;
        std::unordered_map<graph::node_ref,
                      std::unique_ptr<node_paths>,
                      node_ref_hash,
                      node_ref_compare>
                      _paths;
};

#endif // __SHORTEST_PATH__

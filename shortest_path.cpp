#include "shortest_path.hpp"

#include <functional> // For hash
#include <iostream>

using namespace std;

ostream& operator<<(ostream& out, shortest_path& s)
{
    for (auto& src: s._paths) {
        out << "Path from " << src.first.get() << endl;
        for (auto& dest: *(src.second)) {
            if (src.first.get() == dest.first.get()) {
                continue;
            }
            out << "Path to " << dest.first.get() << endl;
            path::print_full_path(out, *(dest.second));
        }
    }
    return out;
}

size_t shortest_path::node_ref_hash::operator()(const graph::node_ref& n) const
{
    return hash<int>()(n.get().get_id());
}
void shortest_path::compute_paths()
{
    // Compute shortest path with each node in graph as source
    for(auto& node: _g.get_nodes()) {
        auto closed = unique_ptr<node_paths>(new node_paths());
        open_set open;
        auto tmp = path_ptr(new path(*node));
        open.push_back(tmp); // no need to make_heap as it is a single element
        while (!open.empty()) {
            pop_heap(open.begin(), open.end(), open_set_order());
            path_ptr current = open.back(); // Next smallest path
            open.pop_back();
            closed->insert({current->get_node(), current}); // shortest for curremt
            auto neighbors = current->get_node().get_neighbors();
            for (auto neighbor: neighbors) {
                auto closed_iter = closed->find(neighbor);
                if (closed_iter != closed->end()) {
                    continue;
                }
                auto open_iter = find_if(open.begin(),
                                    open.end(),
                                    [&](const path_ptr& i)
                                    {
                                        return i->get_node() == neighbor.get();
                                    }
                                    );
                auto edge_iter = _g.get_edge_iterator(neighbor, current->get_node());
                auto new_path = path_ptr(new path(neighbor, current.get(), (*edge_iter)->get_cost()));
                if (open_iter == open.end()) {
                    // We have never seen this node add a new path
                    open.push_back(new_path);
                    make_heap(open.begin(), open.end(), open_set_order());
                    continue;
                }
                // We have already a path check if we improve it
                if ((*open_iter)->get_cost() > new_path->get_cost()) {
                    swap(*open_iter, new_path);
                    make_heap(open.begin(), open.end(), open_set_order());
                }
            }
        }
        _paths[*node] = move(closed); // We have computed all the paths for the given source
    }
    _ran = true;
}

path* shortest_path::get_path(graph::node& n1, graph::node& n2)
{
    if(!_ran) {
        compute_paths();
    }
    auto iter = _paths.find(ref(n1));
    if (iter == _paths.end()) {
        // Here we should rather assert as normally there should be
        // one entry per node even if the graph is totally disconnected
        return nullptr;
    }
    auto& node_paths = iter->second;
    auto iter2 = node_paths->find(ref(n2));
    if (iter2 == node_paths->end()) {
        return nullptr;
    }
    return iter2->second.get();
}

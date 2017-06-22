#include "path.hpp"

using namespace std;

void path::set_predecessor(path& p)
{
    _predecessor = &p;
}

path* path::get_predecessor()
{
    return _predecessor;
}

graph::node& path::get_node()
{
    return _node;
}

void path::set_cost(int cost)
{
    _cost = cost;
}

int path::get_cost()
{
    return _cost;
}

ostream& operator<<(ostream& out, path& p)
{
    path* predecessor = p.get_predecessor();
    if (predecessor) (out << predecessor->get_node()); else out << "nullptr";
    return out << "<--" << p.get_node();
}

void path::print_full_path(ostream& out, path& start)
{
    path* current = &start;
    while (current) {
        out << current->get_node();
        current = current->get_predecessor();
        if (current) {
            out << "<-->";
        }
    }
    out << endl;
}

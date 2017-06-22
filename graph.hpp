#ifndef __GRAPH__
#define __GRAPH__

 // C++ includes
#include <functional>   // For reference_wrapper
#include <iostream>
#include <list>
#include <memory>       // For unique_ptr
#include <utility>      // For pair
#include <vector>

// C includes
#include <cstddef>      // For size_t
#include <cstdlib>      // For rand
#include <ctime>


//TODO: have a look at the design using properties instead of
//private members

//TODO: move from list to unordered_map

class graph
{
    public:
        class node; // forrward declaration for edge
        typedef std::reference_wrapper<node> node_ref;

        class edge
        {
            public:
                friend class graph;
                edge(node& x, node& y, int cost = 0) : _cost(cost), _edge(x, y) {}
                bool is_edge(node& x, node& y);
                const std::pair<node_ref, node_ref>& get_edge() {return _edge;}
                int get_cost() {return _cost;}
                void set_cost(int cost) {_cost = cost;}
            private:
                int _cost;
                std::pair<node_ref, node_ref> _edge;
        };

        class node
        {
            public:
                friend class graph;
                node(int id = 0) : _id(id){};
                void add_neighbor(node& x);
                void remove_neighbor(node& x);
                void set_id(int id) {_id = id;}
                int get_id() {return _id;}
                bool has_neighbor(node& x);
                bool has_neighbors(){return !_neighbors.empty();}
                void print_neighbors();
                bool operator==(node& other) {return (_id == other._id);}
                bool operator!=(node& other) {return !this->operator==(other);}
                const std::list<node_ref>& get_neighbors(){return _neighbors;}
            private:
                std::list<node_ref> _neighbors;
                int _id;
        };

        typedef std::unique_ptr<edge> edge_ptr;
        typedef std::unique_ptr<node> node_ptr;
        typedef std::unique_ptr<graph> graph_ptr;

        size_t node_count();
        size_t edge_count();
        bool adjacent(node& x, node& y);
        node& add_node(int id);
        node& add_node();
        void delete_node(node& x);
        bool has_node(node& x);
        std::list<node_ptr>::iterator get_node_iterator(node& x);
        edge& add_edge(node& x, node& y, int cost = 0);
        void delete_edge(node& x, node& y);
        bool has_edge(node& x, node& y);
        std::list<edge_ptr>::iterator get_edge_iterator(node& x, node& y);
        int get_node_value(node& x);
        void set_node_value(node& x, int id);
        int get_edge_value(edge& e);
        void set_edge_value(edge& e, int cost);
        static graph_ptr generate_graph(size_t size,
                                        double density = 0.1,
                                        int min_cost = 0.0,
                                        int max_cost = 10.0);
        const std::list<node_ptr>& get_nodes() {return _nodes;}
        const std::list<edge_ptr>& get_edges() {return _edges;}

        int get_id() {return _id++;}

        // The graph own the memory associated with the node
        // and edges so we want to strore references in node
        // and edges to other nodes and the graph stores
        // unique_ptr. weak_ptr is not an option as it only
        // works with shared_ptr and moving to shared_ptr
        // does not make it clear who owns the memory.
        // Now another problem arises, references cannot
        // be stored in containers as they are not default
        // constructible and not assignable which is are
        // requirements for STL containers. The work around
        // is to use reference_wrapper. Now this brings a
        // tid bit more work as the interface does not
        // expose those internal details so most of
        // the time we receive references from the user
        // in turn those are not readili comparable to
        // references so we use predicate to look for
        // references or remove them while they are wrapped
        // in reference_wrapper
        class compare_node_wrapper
        {
            public: 
                compare_node_wrapper(node& x): _x(x) {}
                bool operator()(const node_ref& i) {return i.get() == _x;}
            private:
                node& _x;
        };
        class compare_node_ptr
        {
            public: 
                compare_node_ptr(node& x): _x(x) {}
                bool operator()(const node_ptr& i) {return *i == _x;}
            private:
                node& _x;
        };
        graph(): _node_count(0),
                 _edge_count(0),
                 _nodes(),
                 _edges(),
                 _id(0) {}
    private:
        size_t _node_count;
        size_t _edge_count;
        std::list<node_ptr> _nodes;
        std::list<edge_ptr> _edges;
        int _id;
        class density_generator
        {
            public:
                density_generator(double density = 0.1) : _density(density){srand(time(0));}
                bool operator()() {return ((static_cast<double>(rand())/RAND_MAX) < _density);}
            private:
                double _density;
        };
        class cost_generator
        {
            public:
                cost_generator(int min_cost = 0.0, int max_cost = 10.0)
                    : _min_cost(min_cost),
                      _max_cost(max_cost),
                      _range(max_cost - min_cost){srand(time(0));}
                int operator()() {return (_min_cost + (rand() % _range));}
            private:
                int _min_cost;
                int _max_cost;
                int _range;
        };
};

// Display facilities
std::ostream& operator<<(std::ostream& out, graph::node& n);
std::ostream& operator<<(std::ostream& out, graph::edge& e);
std::ostream& operator<<(std::ostream& out, graph& g);

#endif // __GRAPH__

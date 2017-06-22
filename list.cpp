// C++ headers
#include <iostream>
#include <iterator>

// C headers
#include <cstdlib>
#include <cstddef>
#include <cstring>

using namespace std;

class list_element
{
    public:
        // methods
        list_element(int n = 0, list_element* ptr = nullptr): d(n), next(ptr){};
        list_element(const list_element& other, list_element* ptr = nullptr) {d = other.d; next = ptr;}

        // data
        char d;
        list_element* next;
};

ostream& operator<<(ostream& out, list_element& e)
{
    cout << e.d;
    return out;
}

class list
{
    public:
        list(): head(nullptr), tail(nullptr){};
        list(const char* arr, size_t n);
        list(const list& other);
        ~list();
        void push_front(int n);
        void push_front(const list_element& e);
        void push_back(int n);
        void push_back(const list_element& e);
        friend ostream& operator<<(ostream& out, list& l);
        class iterator : public std::iterator<forward_iterator_tag, list_element>
        {
            public:
                iterator(list_element* cursor = nullptr):cursor(cursor){};
                iterator(const iterator& iter){this->cursor = iter.cursor;};
                ~iterator(){cursor = nullptr;};

                iterator& operator=(const iterator& iter){this->cursor = iter.cursor; return *this;}
                bool operator==(const iterator& iter) const {return (this->cursor == iter.cursor);}
                bool operator!=(const iterator& iter) const {return !(*this == iter);}

                iterator& operator++(){if (cursor) {cursor = cursor->next;} return *this;}
                reference operator*() const {return *cursor;}
                pointer operator->() const {return cursor;}
                // Can be supressed but then need to use an ugly &(*iter) to get access to the pointer type
                pointer operator&() const {return cursor;}
            private:
                list_element* cursor;
        };
        class const_iterator : std::iterator<forward_iterator_tag, const list_element>
        {
            public:
                const_iterator(list_element* cursor = nullptr):cursor(cursor){};
                const_iterator(const const_iterator& iter){this->cursor = iter.cursor;};
                // TODO: Add a constructor taking an iterator to create a const_iterator
                // challenge deal with the private cursor
                ~const_iterator(){cursor = nullptr;};

                const_iterator& operator=(const const_iterator& iter){this->cursor = iter.cursor; return *this;}
                bool operator==(const const_iterator& iter) const {return (this->cursor == iter.cursor);}
                bool operator!=(const const_iterator& iter) const {return !(*this == iter);}

                const_iterator& operator++(){if (cursor) {cursor = cursor->next;} return *this;}
                reference operator*() const {return *cursor;}
                pointer operator->() const {return cursor;}
                // Can be supressed but then need to use an ugly &(*iter) to get access to the pointer type
                pointer operator&() const {return cursor;}
            private:
                const list_element* cursor;
        };
        iterator begin(){return iterator(head);}
        iterator end(){return iterator();}
        const_iterator begin() const {return const_iterator(head);}
        const_iterator end() const {return const_iterator();}
    private:
        list_element* head;
        list_element* tail;
        void push_front_internal(list_element* e);
        void push_back_internal(list_element* e);
};

void list::push_front_internal(list_element* e)
{
    if (!head) {
        tail = e;
    }
    head = e;
}

void list::push_front(int n)
{
    list_element* tmp = new list_element(n, head);
    push_front_internal(tmp);
}

void list::push_front(const list_element& e)
{
    list_element* tmp = new list_element(e, head);
    push_back_internal(tmp);
}

void list::push_back_internal(list_element* e)
{
    if (!tail) {
        head = tail = e;
        return;
    }
    tail->next = e;
    tail = e;
}

void list::push_back(int n)
{
    list_element* tmp = new list_element(n);
    push_back_internal(tmp);
}

void list::push_back(const list_element& e)
{
    list_element* tmp = new list_element(e);
    push_back_internal(tmp);
}

ostream& operator<<(ostream& out, list& l)
{
    for (auto e: l) {
        out << e << " ";
    }
    return out;
}

list::~list()
{
    iterator prev = begin();
    if (prev == end()) {
        return;
    }
    for (iterator curr = next(prev); curr != end(); ++curr) {
        delete &prev;
        prev = curr;
    }
    delete &prev;
}

list::list(const char* arr, size_t n)
{
    if (!arr || !n) {
        return;
    }
    head = nullptr;
    tail = nullptr;
    for (size_t i = 0; i < n; ++i) {
        this->push_back(arr[i]);
    }
}

list::list(const list& other)
{
    head = nullptr;
    tail = nullptr;
    for (auto e: other) {
        push_back(e);
    }
}

int main()
{
    list l;
    l.push_front('e');
    l.push_front('n');
    l.push_front('i');
    l.push_front('m');
    l.push_front('A');
    cout << l << endl;
    list l2(l);
    cout << l2 << endl;
    const char* arr = "Amine";
    list l3(arr, strlen(arr));
    cout << l3 << endl;
    return EXIT_SUCCESS;
}

#ifndef LL_H
#define LL_H

#include <iostream>
#include <exception>
#include <string>

class EmptyList : public std::exception
{
private:
    std::string msg;
public:
    EmptyList() : msg("List is empty!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

class OutOfBounds : public std::exception
{
private:
    std::string msg;
public:
    OutOfBounds() : msg("Index out of bounds!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

class AlreadyPresent : public std::exception
{
private:
    std::string msg;
public:
    AlreadyPresent() : msg("Already present!") {}
    const char* what() const noexcept { return msg.c_str(); }
};

template <typename T>
class LL
{
private:
    // inner struct, hide inner workings
    struct Node
    {
        T data;
        Node* prev;
        Node* next;
        Node(T _data) : data(_data), prev(nullptr), next(nullptr) {}
    };

    // doubly linked list
    Node* head;
    Node* tail;
    int _size;

    // destroy the list, used in assignment operators to clear the list
    // before copying over, and in destructor
    void destroy()
    {
        while(_size) remove_front();
        head = tail = nullptr;
    }

public:
    LL() : head(nullptr), tail(nullptr), _size(0) {}

    // move constructor
    LL(LL&& other)
    {
        head = other.head;
        tail = other.tail;
        _size = other._size;
        other.head = nullptr;
        other.tail = nullptr;
        other._size = 0;
    }

    // move assignment operator
    LL& operator=(LL&& other)
    {
        destroy();
        head = other.head;
        tail = other.tail;
        _size = other._size;
        other.head = nullptr;
        other.tail = nullptr;
        other._size = 0;
        return *this;
    }

    // insert an element at the front of the list
    void insert_front(const T& _data)
    {
        Node* temp = new Node(_data);
        if (head == nullptr)
            head = tail = temp;
        else
        {
            temp->next = head;
            head->prev = temp;
            head = temp;
        }

        ++_size;
    }

    // insert an element at the back of the list
    void insert_back(const T& _data)
    {
        Node* temp = new Node(_data);
        if (head == nullptr)
            head = tail = temp;
        else
        {
            temp->prev = tail;
            tail->next = temp;
            tail = temp;
        }
        ++_size;
    }

    // insertion sort using comparator function
    template <typename compare>
    void ordered_insert(const T& _data, compare cmp)
    {
        if (head == nullptr)
        {
            insert_front(_data);
            return;
        }
        int res = cmp(_data,head->data);
        if (res <= 0)
        {
            insert_front(_data);
            return;
        }
        res = cmp(_data,tail->data);
        if (res >= 1)
        {
            insert_back(_data);
            return;
        }
        Node* curr = head;
        while(curr->next)
        {
            int res = cmp(_data, curr->data);
            if (res == -1) break;
            curr = curr->next;
        }
        static int num = 0;
        if (++num == 14)
        {
            int noop = 1;
            noop++;
        }
        Node* temp = new Node(_data);
        temp->next = curr;
        temp->prev = curr->prev;
        curr->prev->next = temp;
        curr->prev = temp;
        ++_size;
    }

    // insertion sort using default boolean ops
    void ordered_insert(const T& _data)
    {
        if (head == nullptr)
        {
            insert_front(_data);
            return;
        }
        if (_data <= head->data)
        {
            insert_front(_data);
            return;
        }
        if (_data >= tail->data)
        {
            insert_back(_data);
            return;
        }
        Node* curr = head;
        while(curr->next)
        {
            if (_data == curr->data) break;
            curr = curr->next;
        }
        static int num = 0;
        if (++num == 14)
        {
            int noop = 1;
            noop++;
        }
        Node* temp = new Node(_data);
        temp->next = curr;
        temp->prev = curr->prev;
        curr->prev->next = temp;
        curr->prev = temp;
        ++_size;
    }

    // remove the head element
    void remove_front()
    {
        if (head == nullptr) throw EmptyList();
        Node* temp = head;
        head = head->next;
        --_size;
        if (_size <= 1) tail = head;
        delete temp;
    }

    // remove the tail element
    void remove_back()
    {
        if (tail == nullptr) throw EmptyList();
        Node* temp = tail;
        tail = temp->prev;
        --_size;
        if (_size <= 1) head = tail;
        delete temp;
    }

    // copy constructor
    LL(const LL& other)
    {
        _size = other._size;
        Node* curr = other.head;
        while (curr)
        {
            insert_back(curr->data);
            curr = curr->next;
        }
    }

    // copy assignment operator
    LL& operator=(const LL& other)
    {
        destroy();
        _size = other._size;
        Node* curr = other.head;
        while (curr)
        {
            insert_back(curr->data);
            curr = curr->next;
        }
        return *this;
    }

    ~LL()
    {
        destroy();
    }

    inline int size() const { return _size; }

    // access to individual elements via reference so they may be accessed from
    // outside
    T& operator[](int idx)
    {
        if (idx < 0 || _size == 0 || idx >= _size) throw OutOfBounds();
        // linked lists are not sequential memory, so it can get somewhat tedious
        // to loop all the way from beginning to end. examine the index's position 
        // in relation to the point and traverse from there
        Node* curr;
        int mid = _size/2;
        if (idx > mid)
        {
            idx++;
            curr = tail;
            int numtimes = _size - idx;
            while (numtimes--)
            {
                curr = curr->prev;            
            }
        }
        else
        {
            curr = head;
            while (idx--)
                curr = curr->next;
        }
        return curr->data;
    }

    // find the last instance of an element using default boolean ops
    int find_last_instance(const T& val)
    {
        Node* curr = tail;
        int idx = _size - 1;
        while(curr)
        {
            if (curr->data == val) return idx;
            curr = curr->prev;
            --idx;
        }
        return -1;
    }

    // find the last instance of an element using a comparator
    template <typename compare>
    int find_last_instance(const T& val, compare cmp)
    {
        Node* curr = tail;
        int idx = _size - 1;
        while(curr)
        {
            if (cmp(val, curr->data) == 0) return idx;
            curr = curr->prev;
            --idx;
        }
        return -1;
    }

    // find the first instance of an element using a comparator
    template <typename compare>
    int find_first_instance(const T& val, compare cmp)
    {
        Node* curr = head;
        int idx = 0;
        while (curr)
        {
            if (cmp(val, curr->data) == 0) return idx;
            curr = curr->next;
            ++idx;
        }
        return -1;
    }

    // find the first instance of an element using default boolean ops
    int find_first_instance(const T& val)
    {
        Node* curr = head;
        int idx = 0;
        while(curr)
        {
            if (curr->data == val) return idx;
            curr = curr->next;
            ++idx;
        }
        return -1;
    }

    // similar to brace operators, but make const
    const T& at(int idx)
    {
        if (idx < 0 || _size == 0 || idx >= _size) throw OutOfBounds();
        // linked lists are not sequential memory, so it can get somewhat tedious
        // to loop all the way from beginning to end. examine the index's position 
        // in relation to the point and traverse from there
        Node* curr;
        int mid = _size/2;
        if (idx > mid)
        {
            idx++;
            curr = tail;
            int numtimes = _size - idx;
            while (numtimes--)
            {
                curr = curr->prev;            
            }
        }
        else
        {
            curr = head;
            while (idx--)
                curr = curr->next;
        }
        return curr->data;
    }

    // print the linked list
    friend std::ostream& operator<<(std::ostream& out, const LL& other)
    {
        Node* curr = other.head;
        while (curr)
        {
            out << curr->data << std::endl;
            curr = curr->next;
        }
        return out;
    }

    // remove an individual element
    void remove(int pos)
    {
        if (pos < 0 || pos >= _size) throw OutOfBounds();
        if (pos == 0)
        {
            remove_front();
            return;
        }
        if (pos == _size - 1)
        {
            remove_back();
            return;
        }
        Node* curr = head;
        while(pos--) curr = curr->next;
        Node* currPrev = curr->prev;
        Node* currNext = curr->next;
        currPrev->next = currNext;
        currNext->prev = currPrev;
        delete curr;
    }

    /*class iterator
    {
    friend class LL;
    private:
        Node* it;
        iterator(Node* temp) : it(temp) {}

    public:
        iterator() : it(nullptr) {}

        inline bool operator!=(const iterator& other) const { return it != other.it; }
        inline bool operator==(const iterator& other) const { return it == other.it; }


        T& operator*() const { return it->data; }
        T* operator->() const { return &it->data; }
        iterator operator++()
        {
            iterator _it = it;
            it = it->next;
            return _it;
        }
        iterator operator++(int)
        {
            iterator _it = it;
            it = it->next;
            return _it;
        }
        iterator operator--()
        {
            iterator _it = it;
            it = it->prev;
            return _it;
        }
        iterator operator--(int)
        {
            iterator _it = it;
            it = it->prev;
            return _it;
        }
    };

    class reverse_iterator
    {
    friend class LL;
    private:
        Node* it;
        reverse_iterator(Node* temp) : it(temp) {}

    public:
        reverse_iterator() : it(nullptr) {}

        inline bool operator!=(const reverse_iterator& other) const { return it != other.it; }
        inline bool operator==(const reverse_iterator& other) const { return it == other.it; }

        T& operator*() const { return it->data; }
        iterator operator++()
        {
            iterator _it = it;
            it = it->prev;
            return _it;
        }
        iterator operator++(int)
        {
            iterator _it = it;
            it = it->prev;
            return _it;
        }
        iterator operator--()
        {
            iterator _it = it;
            it = it->next;
            return _it;
        }
        iterator operator--(int)
        {
            iterator _it = it;
            it = it->next;
            return _it;
        }
    };

    iterator begin() { return iterator(head);}
    iterator end() { return iterator(nullptr); }

    void erase(iterator it)
    {
        if (it == iterator(head))
        {
            Node* temp = head;
            delete temp;
            head = temp;
        }
    }

    const reverse_iterator rbegin() { return reverse_iterator(tail);}
    const reverse_iterator rend() { return reverse_iterator(nullptr); }*/
};

#endif // LL_H

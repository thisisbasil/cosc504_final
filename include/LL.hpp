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
    struct Node
    {
        T data;
        Node* prev;
        Node* next;
        Node(T _data) : data(_data), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int _size;

public:
    LL() : head(nullptr), tail(nullptr), _size(0) {}

    LL(LL&& other)
    {
        head = other.head;
        tail = other.tail;
        _size = other._size;
        other.head = nullptr;
        other.tail = nullptr;
        other._size = 0;
    }

    LL& operator=(LL&& other)
    {
        head = other.head;
        tail = other.tail;
        _size = other._size;
        other.head = nullptr;
        other.tail = nullptr;
        other._size = 0;
        return *this;
    }

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



    void ordered_insert(const T& _data)
    {
        // reduce traversals
        if (head == nullptr || _data < head->data)
        {
            insert_front(_data);
            return;
        }
        else if (_data > tail->data)
        {
            insert_back(_data);
            return;
        }
        Node* curr = head;
        while (curr)
        {
            if (_data == curr->data) throw AlreadyPresent();
            if (_data < curr->data) break;
            curr = curr->next;
        }
        Node* temp = new Node(_data);
        temp->next = curr;
        temp->prev = curr->prev;
        curr->prev->next = temp;
        curr->prev = temp;
        ++_size;
    }

    void remove_front()
    {
        if (head == nullptr) throw EmptyList();
        Node* temp = head;
        head = head->next;
        --_size;
        delete temp;
    }

    void remove_back()
    {
        if (tail == nullptr) throw EmptyList();
        Node* temp = tail;
        tail = temp->prev;
        --_size;
        delete temp;
    }

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

    LL& operator=(const LL& other)
    {
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
        while(_size) remove_front();
        head = tail = nullptr;
    }

    inline int size() const { return _size; }

    T& operator[](int idx)
    {
        if (idx < 0 || _size == 0 || idx >= _size) throw OutOfBounds();
        Node* curr = head;
        while (idx--)
            curr = curr->next;
        return curr->data;
    }

    const T& at(int idx)
    {
        if (idx < 0 || _size == 0 || idx >= _size) throw OutOfBounds();
        Node* curr = head;
        while (idx--)
            curr = curr->next;
        return curr->data;
    }

    friend std::ostream& operator<<(std::ostream& out, const LL& other)
    {
        Node* curr = other.head;
        int i = 0;
        while (curr)
        {
            out << i++ <<". " << curr->data << std::endl;
            curr = curr->next;
        }
        return out;
    }

    class iterator
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
    const reverse_iterator rend() { return reverse_iterator(nullptr); }
};

#endif // LL_H

#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include <string>

using namespace std;

static inline unsigned fnv1a(const char *s)
{
    unsigned h = 2166136261u;
    while (*s)
    {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h ? h : 1u;
}

template <typename K, typename V>
class MyHashMap
{
private:
    struct Node
    {
        K key;
        V value;
        Node *next;
        Node(const K &k, const V &v, Node *nx = nullptr)
            : key(k), value(v), next(nx) {}
    };

    Node **table;
    int capacity;
    int count;

    unsigned idx(const K &key) const
    {
        if constexpr (is_same<K, string>::value)
            return fnv1a(key.c_str()) % capacity;
        else
            return fnv1a(to_string(key).c_str()) % capacity;
    }

    void reHash(int newCapacity)
    {
        Node **newTable = new Node *[newCapacity];
        for (int i = 0; i < newCapacity; i++)
            newTable[i] = nullptr;

        for (int i = 0; i < capacity; i++)
        {
            Node *cur = table[i];
            while (cur)
            {
                Node* nx = cur->next;
                unsigned hi;
                if constexpr (is_same<K, string>::value)
                    hi = fnv1a(cur->key.c_str()) % newCapacity;
                else
                    hi = fnv1a(to_string(cur->key).c_str()) % newCapacity;
                cur->next = newTable[hi];
                newTable[hi] = cur;
                cur = nx;
            }
        }

        delete[] table;
        table = newTable;
        capacity = newCapacity;
    }

    void ensure()
    {
        if ((count + 1) > capacity)
            reHash(capacity * 2);
    }

public:
    class iterator
    {
        MyHashMap *map;
        int bucket;
        Node *cur;

        void advance()
        {
            while (!cur && ++bucket < map->capacity)
                cur = map->table[bucket];
        }

    public:
        iterator(MyHashMap *m, int b, Node *c) : map(m), bucket(b), cur(c)
        {
            if (!cur)
                advance();
        }
        iterator() : map(nullptr), bucket(0), cur(nullptr) {}

        Node *operator->() const { return cur; }
        bool operator==(const iterator &other) const { return cur == other.cur; }
        bool operator!=(const iterator &other) const { return !(*this == other); }

        iterator &operator++()
        {
            if (cur)
                cur = cur->next;
            if (!cur)
                advance();
            return *this;
        }
    };

    MyHashMap(int c = 101)
        : capacity(c < 17 ? 101 : c), count(0)
    {
        table = new Node *[capacity];
        for (int i = 0; i < capacity; i++)
            table[i] = nullptr;
    }

    ~MyHashMap()
    {
        clear();
        delete[] table;
    }

    V &operator[](const K &k)
    {
        ensure();
        unsigned hi = idx(k);
        for (Node *p = table[hi]; p; p = p->next)
            if (p->key == k)
                return p->value;

        Node *newNode = new Node(k, V(), table[hi]);
        table[hi] = newNode;
        ++count;
        return newNode->value;
    }

    iterator begin() { return iterator(this, -1, nullptr); }
    iterator end() { return iterator(this, capacity, nullptr); }

    iterator find(const K &k)
    {
        unsigned hi = idx(k);
        for (Node *p = table[hi]; p; p = p->next)
            if (p->key == k)
                return iterator(this, hi, p);
        return end();
    }

    bool erase(iterator it)
    {
        if (it == end())
            return false;
        Node *target = it.operator->();
        unsigned hi = idx(target->key);
        Node *p = table[hi];
        Node *prev = nullptr;
        while (p)
        {
            if (p == target)
            {
                if (prev)
                    prev->next = p->next;
                else
                    table[hi] = p->next;
                delete p;
                --count;
                return true;
            }
            prev = p;
            p = p->next;
        }
        return false;
    }

    void clear()
    {
        for (int i = 0; i < capacity; i++)
        {
            Node *p = table[i];
            while (p)
            {
                Node *nx = p->next;
                delete p;
                p = nx;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    int size() const { return count; }
};

template <typename K, typename V>
using HashMap = MyHashMap<K, V>;

#endif

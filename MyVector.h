#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdexcept>

using namespace std;

const int DEFAULT_VECTOR_SIZE = 10;

template <typename T>
class Vector
{
private:
    int size;
    int capacity;
    T *elements;

public:
    Vector()
    {
        size = 0;
        capacity = DEFAULT_VECTOR_SIZE;
        elements = new T[DEFAULT_VECTOR_SIZE];
    }
    Vector(int n, const T &value)
    {
        size = n;
        capacity = 2 * n;
        elements = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            elements[i] = value;
        }
    }
    Vector(const Vector &rhs)
    {
        size = rhs.size;
        capacity = rhs.capacity;
        elements = new T[capacity];
        for (int i = 0; i < rhs.size; i++)
        {
            elements[i] = rhs.elements[i];
        }
    }

    ~Vector()
    {
        delete[] elements;
    }

    int getsize() const
    {
        return this->size;
    }
    int getcapacity() const
    {
        return this->capacity;
    }
    bool isEmpty() const
    {
        return size == 0;
    }

    T &operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw out_of_range("Index out of range\n");
        }
        return elements[index];
    }
    const T &operator[](int index) const
    {
        if (index < 0 || index >= size)
        {
            throw out_of_range("Index out of range\n");
        }
        return elements[index];
    }
    Vector &operator=(const Vector &rhs)
    {
        if (capacity < rhs.size)
        {
            delete[] elements;
            capacity = rhs.size * 2;
            elements = new T[capacity];
        }
        for (int i = 0; i < rhs.size; i++)
        {
            elements[i] = rhs.elements[i];
        }
        size = rhs.size;
        return *this;
    }

    void PushBack(const T &object)
    {
        if (size == capacity)
        {
            T *newarr = new T[capacity * 2];
            for (int i = 0; i < size; i++)
            {
                newarr[i] = elements[i];
            }
            delete[] elements;
            elements = newarr;
            capacity = capacity * 2;
        }
        elements[size] = object;
        size++;
    }
    void PopBack()
    {
        if (size > 0)
        {
            size--;
        }
    }
    void erase(int index)
    {
        if (index < 0 || index >= size)
        {
            throw out_of_range("Index out of range\n");
        }
        for (int i = index; i < size - 1; i++)
        {
            elements[i] = elements[i + 1];
        }
        size--;
    }
    void insert(int index, const T &object)
    {
        if (index < 0 || index > size)
        {
            throw out_of_range("Index out of range\n");
        }
        if (size == capacity)
        {
            capacity = size * 2;
            T *newarr = new T[capacity];
            for (int i = 0; i < size; i++)
            {
                newarr[i] = elements[i];
            }
            delete[] elements;
            elements = newarr;
        }
        for (int i = size; i > index; i--)
        {
            elements[i] = elements[i - 1];
        }
        elements[index] = object;
        size++;
    }
    void clear()
    {
        size = 0;
    }

    T *begin()
    {
        return elements;
    }
    T *end()
    {
        return elements + size;
    }

    const T *begin() const
    {
        return elements;
    }
    const T *end() const
    {
        return elements + size;
    }
    void reserve(int newcapacity)
    {
        if (newcapacity <= capacity || newcapacity <= 0)
            return;
        T *newarr = new T[newcapacity];
        for (int i = 0; i < size; i++)
        {
            newarr[i] = elements[i];
        }
        delete[] elements;
        elements = newarr;
        capacity = newcapacity;
    }
    void removeAt(int idx)
    {
        erase(idx);
    }
};

template <typename T>
using Vec = Vector<T>;

#endif

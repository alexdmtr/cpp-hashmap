//
// Created by Alex on 21-Jun-18.
//

#ifndef CPP_GENERIC_HASHMAP_VECTOR_H
#define CPP_GENERIC_HASHMAP_VECTOR_H
#include <initializer_list>
#include <memory.h>
#include <algorithm>
#include <assert.h>

#define INITIAL_CAPACITY 8
#define GROW_FACTOR 2
#define SHRINK_THRESHOLD_RATIO 0.25
#define SHRINK_FACTOR 0.5

template<typename T>
class array_list
{
private:
    T* table;
    unsigned int capacity;
    unsigned int count;

    void init(unsigned int capacity, std::initializer_list<T> list)
    {
        this->capacity = std::max(capacity, list.size() * GROW_FACTOR);
        this->count = list.size();

        table = (T*)malloc(this->capacity * sizeof(T));
        int position = 0;
        for (const T& element : list)
        {
            table[position++] = element;
        }
    }

    void grow()
    {
        capacity *= GROW_FACTOR;
        table = (T*)realloc(table, capacity * sizeof(T));
    }

    void shrink()
    {
        // Can't lose elements!
        if (capacity / SHRINK_FACTOR >= count) {
            capacity /= SHRINK_FACTOR;
            table = (T *) realloc(table, capacity * sizeof(T));
        }
    }

public:
    ~array_list()
    {
        free(table);
    }

    array_list(std::initializer_list<T> list)
    {
        init(INITIAL_CAPACITY, list);
    }

    explicit array_list(int count)
    {
        init(std::max(INITIAL_CAPACITY, count), {});
        this->count = count;
    }

    void clear()
    {
        for (int i = 0; i < count; i++)
            delete &table[i];
        count = 0;
    }

    void push_back(T element)
    {
        if (count + 1 > capacity)
            grow();

        table[count++] = element;
    }

    T pop_back()
    {
        assert(count > 0);
        T element = table[count-1];
        count--;

        if (count * 1. / capacity <= SHRINK_THRESHOLD_RATIO)
            shrink();
        return element;
    }

    void push_front(T element)
    {
        if (count + 1 > capacity)
            grow();

        for (int i = count-1; i >= 1; i--)
            table[i] = table[i-1];
        table[0] = element;
    }

    T pop_front()
    {
        assert(count > 0);
        T element = table[0];
        for (int i = 0; i < count-1; i++)
            table[i] = table[i+1];
        count--;

        if (count * 1. / capacity <= SHRINK_THRESHOLD_RATIO)
            shrink();
        return element;
    }

    T& at(int index)
    {
        assert(index >= 0 && index < count);
        return table[index];
    }

    T& operator[](int index) const
    {
        return table[index];
    }

    unsigned int size()
    {
        return count;
    }
};
#endif //CPP_GENERIC_HASHMAP_VECTOR_H

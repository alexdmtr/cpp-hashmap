//
// Created by Alex on 20-Jun-18.
//

#ifndef CPP_GENERIC_HASHMAP_HASH_MAP_H
#define CPP_GENERIC_HASHMAP_HASH_MAP_H
#include <iostream>
#include <functional>

#define TABLE_SIZE 997
#define FILL_AMOUNT 0.7
#define GROWTH_FACTOR 2
template<typename K = std::string, typename V = std::string>
class hash_map
{
private:
    struct entry_list
    {
        K key;
        V value;
        entry_list* next;

        static void delete_list(entry_list* entry)
        {
            if (entry->next)
                delete_list(entry->next);
            delete entry;
        }
    };
    entry_list ** table;
    int table_capacity;
    int table_count;
    double fill_amount;

    void init(int initial_capacity = TABLE_SIZE)
    {
        initial_capacity = nearest_prime(initial_capacity);
        table = new entry_list*[initial_capacity];
        table_capacity = initial_capacity;
        table_count = 0;

        for (int i = 0; i < table_capacity; i++)
            table[i] = nullptr;
    }

    bool prime(int n)
    {
        if (n % 2 == 0 && n > 2 || n < 2)
            return false;
        for (int d = 3; d * d <= n; d += 2)
            if (n % d == 0)
                return false;
        return true;
    }

    int nearest_prime(int n)
    {
        if (n % 2 == 0)
            n++;
        while (!prime(n)) n += 2;
        return n;
    }

    void check_capacity()
    {
        if (table_count * 1. / table_capacity >= FILL_AMOUNT)
            rehash();
    }

    void rehash()
    {
        int new_capacity = nearest_prime(table_capacity * GROWTH_FACTOR);
        entry_list** new_table = new entry_list*[new_capacity];

        for (int i = 0; i < new_capacity; i++)
            new_table[i] = nullptr;

        for (int i = 0; i < table_capacity; i++)
        {
            auto entry = table[i];
            while (entry) {
                auto next = entry->next;
                entry->next = nullptr;
                int hash = hash_key(entry->key, new_capacity);
                entry_list* new_entry;
                if (!(new_entry = new_table[hash])) {
                    new_table[hash] = entry;
                }
                else {
                    while (new_entry->next) new_entry = new_entry->next;
                    new_entry->next = entry;
                }

                entry = next;
            }
        }
        std::swap(new_table, table);
        std::swap(table_capacity, new_capacity);
        delete new_table;
    }

    inline int hash_key(const K& key, int size) const noexcept
    {
        std::hash<K> hash;
        return hash(key) % size;
    }

    inline int hash_key(const K& key) const noexcept
    {
        return hash_key(key, table_capacity);
    }


    V& find_or_put(K key)
    {
        int hash;
        auto entry = table[hash = hash_key(key)];

        if (!entry)
        {
            table[hash] = new entry_list { key };
            table[hash]->next = nullptr;
            table_count++;
            return table[hash]->value;
        }

        while (entry)
        {
            if (entry->key == key)
            {
                return entry->value;
            }
            if (entry->next == nullptr)
            {
                entry->next = new entry_list { key };
                entry->next->next = nullptr;
                table_count++;
                return entry->next->value;
            }
            entry = entry->next;
        }
    }

public:
    ~hash_map()
    {
        for (int i = 0; i < table_capacity; i++)
        {
            if (table[i])
                entry_list::delete_list(table[i]);
        }
        delete[] table;
    }

    hash_map(): fill_amount(FILL_AMOUNT)
    {
        init();
    }

    hash_map(int initial_capacity, double fill_amount): fill_amount(FILL_AMOUNT)
    {
        init(initial_capacity);
    }

    hash_map(std::initializer_list<std::pair<K, V>> list): fill_amount(FILL_AMOUNT)
    {
        init();
        for (const auto& kvp: list)
        {
            insert(kvp.first, kvp.second);
        }

    }

    V insert(K key, V value)
    {
        check_capacity();
        find_or_put(key) = value;
        return value;
    }

    V at(K key)
    {
        return find_or_put(key);
    }

    void erase(K key)
    {
        int hash = hash_key(key);
        auto entry = table[hash];

        if (!entry) return;

        auto previous = entry;
        if (entry->key == key)
        {
            auto next = entry->next;
            delete entry;
            table[hash] = next;
            table_count--;
            return;
        }

        previous = entry;
        entry = entry->next;
        while (entry)
        {
            if (entry->key == key)
            {
                auto next = entry->next;
                previous->next = next;
                delete entry;
                table_count--;
                return;
            }
            previous = entry;
            entry = entry->next;
        }
    }

    bool contains(K key) const noexcept
    {
        auto entry = table[hash_key(key)];

        if (!entry) return false;

        while (entry)
        {
            if (entry->key == key)
                return true;
            entry = entry->next;
        }

        return false;
    }

    V& operator[](K key)
    {
        check_capacity();
        return find_or_put(key);
    }


};
#endif //CPP_GENERIC_HASHMAP_HASH_MAP_H

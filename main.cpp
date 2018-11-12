#include <iostream>
#include <string>
#include "array_list.h"
#include "hash_map.h"

using namespace std;

#ifdef assert
#undef assert
#endif
#define assert(check, err) \
        if (!check) \
        { \
            std::cerr << (err); \
            exit(1); \
        }
void tests() {
    hash_map<string, int> order(20, 0.7);

    for (int i = 0; i <= 100; i++)
    {
        string s(1 + i / 26, (char)(i%26+'A'));
        assert(!order.contains(s), "HashMap must not have key " + s );
        order[s] = i;
        assert(order.contains(s), "HashMap must have key " + s );
    }

    for (int i = 100; i >= 0; i--)
    {
        string s(1 + i / 26, (char)(i%26+'A'));
        assert(order.contains(s), "HashMap must have key " + s );
        assert(order[s] == i, "HashMap key " + s + " must correspond to value");
        order.erase(s);
        assert(!order.contains(s), "HashMap must not have key " + s );
    }
}

struct myclass
{
    string operator()() {
        return "myclass";
    }

    ~myclass()
    {
        cout << "Self destruct\n";
    }
};
int main()
{
    tests();
    array_list<myclass> v { myclass {}};
    cout << "About to destruct\n";
    v.clear();

    array_list<int> list { 1, 2, 3, 4 };

    for (int i = 5; i <= 100; i++)
        list.push_back(i);

    list.clear();
    for (int i = 0; i < list.size(); i++)
        cout << list[i] << " ";


    return 0;
}
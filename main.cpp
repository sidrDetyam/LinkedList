#include <iostream>
#include "LinkedList.h"
#include <list>
#include <vector>

using namespace std;


struct Foo{
    int x;
    Foo(): x(10){}

    Foo(const Foo&) = default;
    Foo(Foo&& other) noexcept{
        x = other.x;
        other.x = -1;
    }
};


template<typename T>
void print(const LinkedList<T>& l){

    for(auto &i : l){
        cout << i << ' ';
    }

    cout << endl;
}


int main() {

    vector<int> v{111, 222, 333, 444, 555};
    auto ilist = {1, 2, 3};

    LinkedList<int> list;
    list.insert(list.end(), 10);
    list.insert(list.end(), 20);

    auto itt = list.cbegin();
    ++itt;
    list.insert(itt, ilist);
    list.insert(list.end(), v.begin(), v.end());

    print(list);

    Foo f;
    LinkedList<Foo> l;
    for(int i=0; i<3; ++i){
        l.insert(l.begin(), std::move(f));
    }

    for(auto &i:l){
        cout << i.x << ' ';
    }
    cout << endl;

    return 0;
}

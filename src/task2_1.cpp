#include <iostream>
#include <list>

// Private inheritance hides every base mutator: clients cannot break sorting.
template <class T>
class Ordered_list : private std::list<T> {
    using Base = std::list<T>;
public:
    using const_iterator = typename Base::const_iterator;

    void insert(const T& value) {
        auto it = Base::cbegin();
        while (it != Base::cend() && ! (value < *it)) ++it;
        Base::insert(it, value);
    }
    void remove(const T& value) { Base::remove(value); }
    const_iterator begin() const { return Base::cbegin(); }
    const_iterator end() const { return Base::cend(); }
};

int main() {
    Ordered_list<int> values;
    for (int x : {5, 2, 4, 2, 1}) values.insert(x);
    std::cout << "After insert: ";
    for (int x : values) std::cout << x << ' ';
    values.remove(2); // removes all equal elements, like std::list::remove
    std::cout << "\nAfter remove(2): ";
    for (auto it = values.begin(); it != values.end(); ++it) std::cout << *it << ' ';
    std::cout << '\n';
}

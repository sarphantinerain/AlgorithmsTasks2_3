#include <iostream>
#include <stdexcept>
#include <utility>

// Indices are stable across array growth. Reusing an erased slot invalidates
// the old handle; callers must not retain handles after erasure.
template <class T>
class ArrayLinkedList {
    struct Slot {
        T value{};
        int prev = -1;
        int next = -1; // also the next free slot while unused
        bool used = false;
    };
    Slot* slots_ = nullptr;
    int capacity_ = 0, size_ = 0, head_ = -1, tail_ = -1, free_ = -1;

    void grow() {
        int old_capacity = capacity_;
        int new_capacity = capacity_ ? capacity_ * 2 : 4;
        Slot* next = new Slot[new_capacity];
        for (int i = 0; i < old_capacity; ++i) next[i] = std::move(slots_[i]);
        delete[] slots_;
        slots_ = next;
        capacity_ = new_capacity;
        // Push new cells onto the in-array free stack.
        for (int i = new_capacity - 1; i >= old_capacity; --i) {
            slots_[i].next = free_;
            free_ = i;
        }
    }
    int acquire(const T& value) {
        if (free_ == -1) grow();
        int id = free_;
        free_ = slots_[id].next;
        slots_[id].value = value;
        slots_[id].next = -1;
        slots_[id].prev = -1;
        slots_[id].used = true;
        ++size_;
        return id;
    }
    void check(int id) const {
        if (id < 0 || id >= capacity_ || !slots_[id].used)
            throw std::out_of_range("invalid list index");
    }
public:
    ArrayLinkedList() = default;
    ArrayLinkedList(const ArrayLinkedList&) = delete;
    ArrayLinkedList& operator=(const ArrayLinkedList&) = delete;
    ~ArrayLinkedList() { delete[] slots_; }

    int push_back(const T& value) { return insert_after(tail_, value); }
    int insert_after(int position, const T& value) {
        if (position == -1 && size_ != 0) throw std::out_of_range("position required");
        if (position != -1) check(position);
        int id = acquire(value);
        if (position == -1) {
            head_ = tail_ = id;
        } else {
            int following = slots_[position].next;
            slots_[id].prev = position;
            slots_[id].next = following;
            slots_[position].next = id;
            if (following == -1) tail_ = id;
            else slots_[following].prev = id;
        }
        return id;
    }
    int insert_before(int position, const T& value) {
        check(position);
        int before = slots_[position].prev;
        int id = acquire(value);
        slots_[id].next = position;
        slots_[id].prev = before;
        slots_[position].prev = id;
        if (before == -1) head_ = id;
        else slots_[before].next = id;
        return id;
    }
    void erase(int id) {
        check(id);
        int before = slots_[id].prev, after = slots_[id].next;
        if (before == -1) head_ = after;
        else slots_[before].next = after;
        if (after == -1) tail_ = before;
        else slots_[after].prev = before;
        slots_[id].used = false;
        slots_[id].prev = -1;
        slots_[id].next = free_;
        free_ = id;
        --size_;
    }
    int find(const T& value) const {
        for (int i = head_; i != -1; i = slots_[i].next)
            if (slots_[i].value == value) return i;
        return -1;
    }
    void print() const {
        for (int i = head_; i != -1; i = slots_[i].next) std::cout << slots_[i].value << ' ';
        std::cout << '\n';
    }
    int size() const { return size_; }
};

int main() {
    ArrayLinkedList<int> list;
    int one = list.push_back(1);
    int three = list.insert_after(one, 3);
    int two = list.insert_before(three, 2);
    list.print();
    std::cout << "index of 2: " << list.find(2) << '\n';
    list.erase(two);
    list.print();
    list.erase(one); list.erase(three);
    std::cout << "size after erase: " << list.size() << '\n';
}

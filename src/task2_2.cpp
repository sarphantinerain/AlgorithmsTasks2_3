#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>

template <class T>
class ArrayDeque {
    static constexpr std::size_t minimum = 4;
    std::unique_ptr<T[]> data_ = std::make_unique<T[]>(minimum);
    std::size_t capacity_ = minimum, size_ = 0, first_ = 0;

    void reallocate(std::size_t new_capacity) {
        auto next = std::make_unique<T[]>(new_capacity);
        for (std::size_t i = 0; i < size_; ++i)
            next[i] = std::move(data_[(first_ + i) % capacity_]);
        data_ = std::move(next);
        capacity_ = new_capacity;
        first_ = 0;
    }
    void shrink_if_needed() {
        if (capacity_ > minimum && size_ <= capacity_ / 4)
            reallocate(capacity_ / 2);
    }
public:
    void Push_Back(const T& value) {
        if (size_ == capacity_) reallocate(capacity_ * 2);
        data_[(first_ + size_) % capacity_] = value;
        ++size_;
    }
    void Push_Front(const T& value) {
        if (size_ == capacity_) reallocate(capacity_ * 2);
        first_ = (first_ + capacity_ - 1) % capacity_;
        data_[first_] = value;
        ++size_;
    }
    T Pop_Back() {
        if (!size_) throw std::out_of_range("empty deque");
        T result = std::move(data_[(first_ + size_ - 1) % capacity_]);
        --size_;
        shrink_if_needed();
        return result;
    }
    T Pop_Front() {
        if (!size_) throw std::out_of_range("empty deque");
        T result = std::move(data_[first_]);
        first_ = (first_ + 1) % capacity_;
        --size_;
        shrink_if_needed();
        return result;
    }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
};

int main() {
    ArrayDeque<int> q;
    q.Push_Back(2); q.Push_Front(1); q.Push_Back(3); q.Push_Front(0);
    q.Push_Back(4); // growth: 4 -> 8
    std::cout << "capacity after growth: " << q.capacity() << '\n';
    std::cout << "front=" << q.Pop_Front() << " back=" << q.Pop_Back() << '\n';
    std::cout << "front=" << q.Pop_Front() << " back=" << q.Pop_Back() << '\n';
    std::cout << "capacity after shrink: " << q.capacity() << '\n';
    std::cout << "last=" << q.Pop_Front() << " size=" << q.size() << '\n';
}

#include <iostream>
#include <stack>
#include <vector>

// Only Create, Empty, Push and Pop are used by the sorting routine.
class Stack {
    std::stack<int> data_;
public:
    static Stack Create() { return {}; }
    bool Empty() const { return data_.empty(); }
    void Push(int value) { data_.push(value); }
    int Pop() { int value = data_.top(); data_.pop(); return value; }
};

Stack selection_sort(Stack input, std::size_t count) {
    Stack selected = Stack::Create();
    for (std::size_t remaining = count; remaining > 0; --remaining) {
        int maximum = 0;
        bool first = true;
        for (std::size_t i = 0; i < remaining; ++i) {
            int value = input.Pop();
            if (first || value > maximum) maximum = value;
            first = false;
            selected.Push(value);
        }
        bool skipped = false;
        for (std::size_t i = 0; i < remaining; ++i) {
            int value = selected.Pop();
            if (!skipped && value == maximum) skipped = true;
            else input.Push(value);
        }
        selected.Push(maximum);
    }
    return selected; // top is the minimum, bottom is the maximum
}

int main() {
    const std::vector<int> values{4, -2, 7, 4, 1};
    Stack input = Stack::Create();
    for (int value : values) input.Push(value);
    Stack sorted = selection_sort(std::move(input), values.size());
    std::cout << "Ascending pop order: ";
    while (!sorted.Empty()) std::cout << sorted.Pop() << ' ';
    std::cout << '\n';
}

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "numbers.txt";
    std::ifstream input(path);
    if (!input) { std::cerr << "Cannot open " << path << '\n'; return 1; }
    std::size_t n;
    if (!(input >> n)) { std::cerr << "Expected n followed by n integers\n"; return 1; }
    std::vector<long long> a(n);
    for (auto& value : a)
        if (!(input >> value)) { std::cerr << "Missing integer\n"; return 1; }

    std::vector<std::size_t> answer(n, 0), pending;
    for (std::size_t j = 0; j < n; ++j) {
        while (!pending.empty() && a[j] > a[pending.back()]) {
            answer[pending.back()] = j + 1; // 1-based index
            pending.pop_back();
        }
        pending.push_back(j);
    }
    for (std::size_t i = 0; i < n; ++i)
        std::cout << (i ? " " : "") << answer[i];
    std::cout << '\n'; // 0 means no greater element to the right
}

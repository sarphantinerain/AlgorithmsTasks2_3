#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

struct Student {
    std::string name;
    std::string birth; // ISO YYYY-MM-DD: lexicographic order is chronological
    int group = 0;
};

enum class Key : std::size_t { Name, Birth, Group, Count };

class StudentList {
    static constexpr std::size_t keys = static_cast<std::size_t>(Key::Count);
    struct Node {
        Student value;
        std::array<Node*, keys> next{};
        std::size_t serial = 0;
    };
    Node sentinel_{}; // circular dummy head in all four views
    std::size_t next_serial_ = 0;

    static std::size_t number(Key key) { return static_cast<std::size_t>(key); }
    static bool less(const Node& a, const Node& b, Key key) {
        if (key == Key::Name)
            return std::tie(a.value.name, a.value.birth, a.value.group, a.serial) <
                   std::tie(b.value.name, b.value.birth, b.value.group, b.serial);
        if (key == Key::Birth)
            return std::tie(a.value.birth, a.value.name, a.value.group, a.serial) <
                   std::tie(b.value.birth, b.value.name, b.value.group, b.serial);
        return std::tie(a.value.group, a.value.name, a.value.birth, a.serial) <
               std::tie(b.value.group, b.value.name, b.value.birth, b.serial);
    }
public:
    StudentList() {
        for (auto& link : sentinel_.next) link = &sentinel_;
    }
    StudentList(const StudentList&) = delete;
    StudentList& operator=(const StudentList&) = delete;
    ~StudentList() {
        Node* current = sentinel_.next[0];
        while (current != &sentinel_) {
            Node* next = current->next[0];
            delete current;
            current = next;
        }
    }
    void insert(Student value) {
        Node* node = new Node{std::move(value), {}, next_serial_++};
        for (std::size_t k = 0; k < keys; ++k) {
            auto key = static_cast<Key>(k);
            Node* before = &sentinel_;
            while (before->next[k] != &sentinel_ && less(*before->next[k], *node, key))
                before = before->next[k];
            node->next[k] = before->next[k];
            before->next[k] = node;
        }
    }
    void print(Key key) const {
        auto k = number(key);
        for (const Node* p = sentinel_.next[k]; p != &sentinel_; p = p->next[k])
            std::cout << p->value.name << " | " << p->value.birth << " | group "
                      << p->value.group << '\n';
    }
};

int main() {
    StudentList students;
    students.insert({"Ivan Petrov", "2004-07-13", 22});
    students.insert({"Anna Sidorova", "2005-01-02", 21});
    students.insert({"Boris Ivanov", "2003-12-19", 22});
    for (Key key : {Key::Name, Key::Birth, Key::Group}) {
        std::cout << "Sorted by " << (key == Key::Name ? "name" : key == Key::Birth ? "birth" : "group") << ":\n";
        students.print(key);
    }
}

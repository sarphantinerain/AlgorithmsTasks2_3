#include <iomanip>
#include <iostream>
#include <map>
#include <utility>

using SparseMatrix = std::map<std::pair<int, int>, double>;

void print_matrix(const SparseMatrix& matrix, int rows, int columns) {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const auto item = matrix.find({row, column});
            const double value = item == matrix.end() ? 0.0 : item->second;
            if (column != 0) std::cout << ' ';
            std::cout << value;
        }
        std::cout << '\n';
    }
}

int main() {
    int rows, columns;
    if (!(std::cin >> rows >> columns) || rows <= 0 || columns <= 0) {
        std::cerr << "Expected positive row and column counts.\n";
        return 1;
    }

    SparseMatrix matrix;
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            double value;
            if (!(std::cin >> value)) {
                std::cerr << "Missing matrix element.\n";
                return 1;
            }
            if (value != 0.0) matrix[{row, column}] = value;
        }
    }

    int row_to_change, column_to_change;
    double new_value;
    if (!(std::cin >> row_to_change >> column_to_change >> new_value) ||
        row_to_change < 0 || row_to_change >= rows ||
        column_to_change < 0 || column_to_change >= columns) {
        std::cerr << "Expected valid zero-based indices and a new value.\n";
        return 1;
    }

    std::cout << std::setprecision(12);
    std::cout << "Before:\n";
    print_matrix(matrix, rows, columns);

    const auto position = std::make_pair(row_to_change, column_to_change);
    if (new_value == 0.0)
        matrix.erase(position); // Zero values are not stored.
    else
        matrix[position] = new_value;

    std::cout << "After:\n";
    print_matrix(matrix, rows, columns);
    std::cout << "Stored nonzero elements: " << matrix.size() << '\n';
    std::cout << "Size of map object: " << sizeof(matrix) << " bytes\n";
}

#include <iostream>
#include <string>
#include <utility>
#include "Set.h"

template <typename T>
Set<T> solve_symmetric_difference(const T* arr1, size_t n1, const T* arr2, size_t n2) {
    Set<T> s1(arr1, n1);
    Set<T> s2(arr2, n2);
    return (s1 - s2) + (s2 - s1);
}

int main() {
    try {
        std::cout << "=== TASK 1 ===\n";
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {4, 5, 6, 7, 8};
        Set<int> result = solve_symmetric_difference(arr1, 5, arr2, 5);
        std::cout << "Result: " << result << "\n\n";

        std::cout << "=== DEMONSTRATION ===\n";
        Set<int> rand_set(10, 1, 20);
        std::cout << "Random: " << rand_set << "\n";

        if (rand_set.size() > 0) {
            std::cout << "[0]: " << rand_set[0] << "\n";
            rand_set[0] = 999;
            std::cout << "Changed [0]: " << rand_set[0] << "\n";
        }

        Set<int> add_test = rand_set;
        add_test += 100;
        std::cout << "+= 100: " << add_test << "\n";

        Set<int> sub_test = rand_set;
        if (sub_test.size() > 0) {
            int val = sub_test[0];
            sub_test -= val;
            std::cout << "-= " << val << ": " << sub_test << "\n";
        }

        Set<int> other(5, 15, 25);
        std::cout << "Union: " << (rand_set + other) << "\n";
        std::cout << "Diff: " << (rand_set - other) << "\n";
        std::cout << "Intersect: " << rand_set.intersection(other) << "\n";
        std::cout << "Contains 100: " << (rand_set.contains(100) ? "Yes" : "No") << "\n";

        Set<int> copy_set = rand_set;
        std::cout << "== : " << (rand_set == copy_set ? "True" : "False") << "\n";
        std::cout << "!= : " << (rand_set != other ? "True" : "False") << "\n";

        std::string strs[] = {"a", "b", "c"};
        Set<std::string> str_set(strs, 3);
        str_set += "d";
        std::cout << "Strings: " << str_set << "\n";

        std::pair<int, double> pairs[] = {{1, 1.1}, {2, 2.2}};
        Set<std::pair<int, double>> pair_set(pairs, 2);
        std::cout << "Pairs: " << pair_set << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

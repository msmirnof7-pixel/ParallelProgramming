#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "Set.h"

int main() {
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    std::vector<int> arr2 = {4, 5, 6, 7, 8};

    Set<int> set1(arr1);
    Set<int> set2(arr2);

    Set<int> symmetric_diff = (set1 - set2) + (set2 - set1);

    std::cout << "Symmetric difference of arrays: ";
    for (size_t i = 0; i < symmetric_diff.size(); ++i) {
        std::cout << symmetric_diff[i] << " ";
    }
    std::cout << "\n\n";

    Set<float> float_set(5, 1.5f, 5.5f);
    std::cout << "Random float set size: " << float_set.size() << "\n";

    Set<std::string> string_set({"apple", "banana", "cherry"});
    string_set += "date";
    std::cout << "String set contains 'banana': " << (string_set.contains("banana") ? "yes" : "no") << "\n";

    Set<std::pair<int, double>> pair_set({{1, 1.1}, {2, 2.2}});
    pair_set += {3, 3.3};
    std::cout << "Pair set size: " << pair_set.size() << "\n";

    return 0;
}

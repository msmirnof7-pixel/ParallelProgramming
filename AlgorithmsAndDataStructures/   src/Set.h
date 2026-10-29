#ifndef SET_H
#define SET_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <type_traits>

template <typename T>
class Set {
private:
    std::vector<T> elements_;

    void maintain_order() {
        std::sort(elements_.begin(), elements_.end());
        elements_.erase(std::unique(elements_.begin(), elements_.end()), elements_.end());
    }

public:
    Set() = default;

    explicit Set(const std::vector<T>& initial_values) {
        elements_ = initial_values;
        maintain_order();
    }

    template <typename U = T>
    Set(size_t count, const U& min_val, const U& max_val) {
        if constexpr (std::is_arithmetic_v<U>) {
            std::random_device rd;
            std::mt19937 gen(rd());
            if constexpr (std::is_integral_v<U>) {
                std::uniform_int_distribution<U> dist(min_val, max_val);
                for (size_t i = 0; i < count; ++i) {
                    this->operator+=(dist(gen));
                }
            } else {
                std::uniform_real_distribution<U> dist(min_val, max_val);
                for (size_t i = 0; i < count; ++i) {
                    this->operator+=(dist(gen));
                }
            }
        }
    }

    T& operator[](size_t index) {
        if (index >= elements_.size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return elements_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= elements_.size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return elements_[index];
    }

    Set operator+(const Set& other) const {
        Set result = *this;
        for (const auto& elem : other.elements_) {
            result += elem;
        }
        return result;
    }

    Set operator-(const Set& other) const {
        Set result;
        for (const auto& elem : elements_) {
            if (!other.contains(elem)) {
                result += elem;
            }
        }
        return result;
    }

    Set& operator+=(const T& value) {
        if (!contains(value)) {
            elements_.push_back(value);
            maintain_order();
        }
        return *this;
    }

    Set& operator-=(const T& value) {
        auto it = std::find(elements_.begin(), elements_.end(), value);
        if (it != elements_.end()) {
            elements_.erase(it);
        }
        return *this;
    }

    Set intersection(const Set& other) const {
        Set result;
        for (const auto& elem : elements_) {
            if (other.contains(elem)) {
                result += elem;
            }
        }
        return result;
    }

    bool contains(const T& value) const {
        return std::binary_search(elements_.begin(), elements_.end(), value);
    }

    size_t size() const {
        return elements_.size();
    }
};

#endif // SET_H

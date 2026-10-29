#ifndef SET_H
#define SET_H

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <cmath>
#include <limits>
#include <utility>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T>
class Set {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    static constexpr double EPSILON = 1e-5;

    void maintain_invariants() {
        std::sort(data_, data_ + size_);
        auto last = std::unique(data_, data_ + size_);
        size_ = static_cast<size_t>(last - data_);
    }

    bool find_index(const T& value, size_t& index) const {
        auto it = std::lower_bound(data_, data_ + size_, value);
        if (it != data_ + size_) {
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(*it - value) <= EPSILON) {
                    index = static_cast<size_t>(it - data_);
                    return true;
                }
            } else {
                if (*it == value) {
                    index = static_cast<size_t>(it - data_);
                    return true;
                }
            }
        }
        return false;
    }

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        std::copy(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    Set() : data_(nullptr), size_(0), capacity_(0) {}

    Set(const T* arr, size_t count) : size_(0), capacity_(count > 0 ? count : 1) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < count; ++i) this->operator+=(arr[i]);
    }

    Set(size_t count, const T& min_val, const T& max_val) 
        : size_(0), capacity_(count > 0 ? count : 1) 
    {
        data_ = new T[capacity_];
        if constexpr (std::is_arithmetic_v<T>) {
            std::mt19937 gen(std::random_device{}());
            if constexpr (std::is_integral_v<T>) {
                std::uniform_int_distribution<T> dist(min_val, max_val);
                for (size_t i = 0; i < count; ++i) this->operator+=(dist(gen));
            } else if constexpr (std::is_floating_point_v<T>) {
                std::uniform_real_distribution<T> dist(min_val, max_val);
                for (size_t i = 0; i < count; ++i) this->operator+=(dist(gen));
            }
        }
    }

    ~Set() { delete[] data_; }

    Set(const Set& other) : size_(other.size_), capacity_(other.capacity_) {
        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + size_, data_);
    }

    Set& operator=(const Set& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    T& operator[](size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of bounds");
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) throw std::out_of_range("Index out of bounds");
        return data_[index];
    }

    Set operator+(const Set& other) const {
        Set result = *this;
        for (size_t i = 0; i < other.size_; ++i) result += other.data_[i];
        return result;
    }

    Set operator-(const Set& other) const {
        Set result;
        result.capacity_ = size_;
        result.data_ = new T[result.capacity_];
        for (size_t i = 0; i < size_; ++i) {
            if (!other.contains(data_[i])) result.data_[result.size_++] = data_[i];
        }
        return result;
    }

    Set& operator+=(const T& value) {
        size_t idx;
        if (!find_index(value, idx)) {
            if (size_ == capacity_) resize(capacity_ == 0 ? 1 : capacity_ * 2);
            data_[size_++] = value;
            maintain_invariants();
        }
        return *this;
    }

    Set& operator-=(const T& value) {
        size_t idx;
        if (find_index(value, idx)) {
            for (size_t i = idx; i < size_ - 1; ++i) data_[i] = data_[i+1];
            --size_;
        }
        return *this;
    }

    Set intersection(const Set& other) const {
        Set result;
        result.capacity_ = std::min(size_, other.size_);
        if (result.capacity_ > 0) result.data_ = new T[result.capacity_];
        for (size_t i = 0; i < size_; ++i) {
            if (other.contains(data_[i])) {
                if (result.size_ == result.capacity_) result.resize(result.capacity_ * 2 + 1);
                result.data_[result.size_++] = data_[i];
            }
        }
        return result;
    }

    bool contains(const T& value) const {
        size_t dummy;
        return find_index(value, dummy);
    }

    size_t size() const { return size_; }

    bool operator==(const Set& other) const {
        if (size_ != other.size_) return false;
        for (size_t i = 0; i < size_; ++i) {
            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(data_[i] - other.data_[i]) > EPSILON) return false;
            } else {
                if (data_[i] != other.data_[i]) return false;
            }
        }
        return true;
    }

    bool operator!=(const Set& other) const { return !(*this == other); }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Set<U>& s);
};

template <typename T>
std::ostream& operator

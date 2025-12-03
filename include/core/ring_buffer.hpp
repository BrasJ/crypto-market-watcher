#pragma once

#include <cstddef>
#include <vector>

namespace cmw {

template <typename T>
class RingBuffer {
public:
    explicit RingBuffer(std::size_t capacity = 0)
        : data_(capacity), capacity_(capacity) {}

    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity; }
    bool empty() const noexcept { return size_ == 0; }

    void push_back(const T& value) {
        if (capacity_ == 0) {
            return;
        }
        data_[end_] = value;

        if (size_ < capacity_) {
            ++size_;
        } else {
            start_ = (start_ + 1) % capacity_;
        }

        end_ = (end_ + 1) % capacity_;
    }

    void push_back(T&& value) {
        if (capacity_ == 0) {
            return;
        }
        data_[end_] = std::move(value);

        if (size_ < capacity_) {
            ++size_;
        } else {
            start_ = (start_ + 1) % capacity_;
        }

        end_ = (end_ + 1) % capacity_;
    }

    const T& operator[](std::size_t idx) const {
        return data_[(start_ + idx) % capacity_];
    }

    const T& back() const {
        return operator[](size_ - 1);
    }

private:
    std::vector<T> data_;
    std::size_t capacity_{0};
    std::size_t start_{0};
    std::size_t end_{0};
    std::size_t size_{0};
};

}
#pragma once

#include <iostream>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <utility>

class Any {
public:
    Any() : data_(nullptr) {}

    template <typename T>
    Any(const T& value) : data_(new Holder<T>(value)) {}

    Any(Any& other) : data_(other.data_->Clone()) {}

    Any(Any&& other) noexcept : data_(std::move(other.data_)) {}

    bool Empty() const {
        return data_ == nullptr;
    }

    void Reset() {
        data_.reset();
    }

    void Swap(Any& other) {
        std::swap(data_, other.data_);
    }

    Any& operator=(const Any& other) {
        if (this != &other) {
            data_.reset(other.data_ ? other.data_->Clone() : nullptr);
        }
        return *this;
    }

    Any& operator=(Any&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }

    template <typename T>
    T& Value() {
        if (!data_) {
            throw std::bad_cast();
        }
        Holder<T>* holder = dynamic_cast<Holder<T>*>(data_.get());
        if (!holder) {
            throw std::bad_cast();
        }
        return holder->value_;
    }

    template <typename T>
    const T& Value() const {
        if (!data_) {
            throw std::bad_cast();
        }
        const Holder<T>* holder = dynamic_cast<const Holder<T>*>(data_.get());
        if (!holder) {
            throw std::bad_cast();
        }
        return holder->value_;
    }

private:
    struct BaseHolder {
        virtual ~BaseHolder() = default;
        virtual BaseHolder* Clone() const = 0;
    };

    template <typename T>
    struct Holder : BaseHolder {
        explicit Holder(const T& value) : value_(value) {}
        BaseHolder* Clone() const override {
            return new Holder(value_);
        }
        T value_;
    };

    std::unique_ptr<BaseHolder> data_;
};
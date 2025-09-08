#include <iostream>
#include <algorithm>
#include <utility>

template <typename T>
class Array {
public:
    Array(std::ostream& ostream)
        : ostream_(ostream), size_(0), capacity_(2), data_(new T[2]) {
        ostream_ << "Constructed. " << *this << '\n';
    }

    Array(const Array<T>& array)
        : ostream_(array.ostream_),
          size_(array.size_),
          capacity_(array.capacity_),
          data_(new T[capacity_]) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = array[i];
        }
        ostream_ << "Constructed from another Array. " << *this << '\n';
    }

    Array(size_t size, std::ostream& ostream = std::cout, T default_value = T())
        : ostream_(ostream),
          size_(size),
          capacity_(size_ * 2),
          data_(new T[capacity_]) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = default_value;
        }
        ostream_ << "Constructed with default. " << *this << '\n';
    }

    ~Array() {
        delete[] data_;
        ostream_ << "Destructed " << size_ << "\n";
    }

    size_t Size() const { return size_; }
    size_t Capacity() const { return capacity_; }

    void Reserve(size_t new_capacity) {
        if (capacity_ < new_capacity) {
            capacity_ = new_capacity;
            T* new_data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
    }

    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            capacity_ = new_size;
            T* new_data = new T[new_size];
            for (size_t i = 0; i < new_size; ++i) {
                new_data[i] = T();
            }
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
        size_ = new_size;
    }

    void PushBack(T value = T()) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            T* new_data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
        data_[size_] = value;
        ++size_;
    }

    void PopBack() {
        if (size_ > 0) {
            --size_;
        }
    }

    const T& operator[](size_t index) const { return data_[index]; }
    T& operator[](size_t index) { return data_[index]; }

    explicit operator bool() const { return size_ > 0; }

    bool operator<(const Array<T>& other) const {
        for (size_t i = 0; i < std::min(size_, other.size_); ++i) {
            if (data_[i] < other.data_[i]) {
                return true;
            } else if (data_[i] > other.data_[i]) {
                return false;
            }
        }
        return size_ < other.size_;
    }

    bool operator>(const Array<T>& other) const { return other < *this; }

    bool operator!=(const Array<T>& other) const {
        if (size_ != other.size_) {
            return true;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return true;
            }
        }
        return false;
    }

    bool operator==(const Array<T>& other) const { return !(*this != other); }

    bool operator<=(const Array<T>& other) const {
        return *this < other || *this == other;
    }

    bool operator>=(const Array<T>& other) const {
        return *this > other || *this == other;
    }

    Array<T>& operator<<(const T& value) {
        PushBack(value);
        return *this;
    }

    Array<T>& operator<<(const Array<T>& other) {
        if (size_ + other.size_ > capacity_) {
            capacity_ = size_ + other.size_;
            T* new_data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
        
        for (size_t i = 0; i < other.size_; ++i) {
            data_[size_ + i] = other.data_[i];
        }
        size_ += other.size_;
        return *this;
    }

    bool Insert(size_t pos, const T& value) {
        if (pos > size_) {
            return false;
        }
        
        if (size_ + 1 > capacity_) {
            capacity_ *= 2;
            T* new_data = new T[capacity_];
            for (size_t i = 0; i < pos; ++i) {
                new_data[i] = data_[i];
            }
            new_data[pos] = value;
            for (size_t i = pos; i < size_; ++i) {
                new_data[i + 1] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        } else {
            for (size_t i = size_; i > pos; --i) {
                data_[i] = data_[i - 1];
            }
            data_[pos] = value;
        }
        ++size_;
        return true;
    }

    bool Erase(size_t pos) {
        if (pos >= size_) {
            return false;
        }
        
        for (size_t i = pos; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
        return true;
    }

    void Clear() {
        size_ = 0;
        capacity_ = 2;
        delete[] data_;
        data_ = new T[2];
    }

    const T &operator*() const {
      return *(ptr_ - 1);
    }


    const T* End() const {
        return data_ + size_;
    }

    T* End() {
        return data_ + size_;
    }

    const T* Begin() const {
        return data_;
    }

    T* Begin() {
        return data_;
    }

    T &operator*() {
      return *(ptr_ - 1);
    }


private:
    std::ostream& ostream_;
    size_t size_;
    size_t capacity_;
    T* data_;
    T* ptr_ = nullptr;
};

template <typename T>
std::ostream& operator<<(std::ostream& ostream, const Array<T>& array) {
    ostream << "Result Array's capacity is " << array.Capacity() 
            << " and size is " << array.Size();
    
    if (array.Size() > 0) {
        ostream << ", elements are: ";
        for (size_t i = 0; i < array.Size(); ++i) {
            ostream << array[i];
            if (i + 1 < array.Size()) {
                ostream << ", ";
            }
        }
    }
    
    return ostream;
}

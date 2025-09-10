#include <cstddef>
#include <algorithm>


struct State {
    size_t size_ = 0;
    size_t capacity_ = 2;
    int reference_count = 1; // сколько строк используют этот State.
    char* data_ = nullptr;

    State(const char* str, size_t size) : size_(size) {
        if(size_ * 2 > 0) {
            capacity_ = size_ * 2;
        } else {
            capacity_ = 2;
        }

        data_ = new char[capacity_];
        std::copy(str, str + size_, data_);
    }

    State(const char* str) {
        size_t size = 0;
        for(auto i = str; *i != '\0'; ++i) {
            size++;
        }

        size_ = size;
        if(size_ * 2 > 0) {
            capacity_ = size_ * 2;
        } else {
            capacity_ = 2;
        }

        data_ = new char[capacity_];
        std::copy(str, str + size, data_);
    }

    State(const State&) = delete;
    
    State& operator=(const State&) = delete;

    State(State&&) = delete;
    
    State& operator=(State&&) = delete;

    ~State(){
        delete[] data_;
    }
};

// Cow - copy on write
// При копировании содержимое не копируется, а делится между копиями.
// Когда объект производит модифимодифицирующую операцию,
// он предварительно копирует общие данные себе и модифицирует уже их
class CowString {
public:
    CowString(const char* value = "") : state_(new State(value)) {}

    // Разделяем состояние между объектами
    CowString(CowString& other) : state_(other.state_) {
        state_->reference_count++;
    }

    CowString& operator=(const CowString& other) {
        if(&other != this) { 
            state_->reference_count--;

            if(state_->reference_count == 0) {
                delete state_;
            }

            state_ = other.state_;
            state_->reference_count++;
        }

        return *this;
    }

    const char& At(size_t index) const {
        return state_->data_[index];
    }

    char& operator[](size_t index) {
        if(state_->reference_count > 1){
            state_->reference_count--;
            state_ = new State(state_->data_, state_->size_);
        }

        return state_->data_[index];
    }

    const char& operator[](size_t index) const {
        return state_->data_[index];
    }

    const char& Back() const {
        return state_->data_[state_->size_ - 1];
    }

    void PushBack(char c) {
        // Проверяем необходимость копирования при записи
        if(state_->reference_count > 1) {
            state_->reference_count--;
            state_ = new State(state_->data_, state_->size_);
        }

        if(state_->size_ + 1 > state_->capacity_) {
            Reserve(state_->size_ * 2);
        }
        state_->data_[state_->size_] = c;
        state_->size_++;

    }

    size_t Size() const {
        return state_->size_;
    }
    size_t Capacity() const {
        return state_->capacity_;
    }

    void Reserve(size_t capacity) {
        if(state_->reference_count > 1) {
            state_->reference_count--;
            state_ = new State(state_->data_, state_->size_);
        }

        if(capacity > state_->capacity_) {
            auto new_data = new char[capacity];
            std::copy(state_->data_, state_->data_ + state_->size_, new_data);
            delete[] state_->data_;
            state_->data_ = new_data;
            state_->capacity_ = capacity;
        }
    }

    void Resize(size_t size){
        if(state_->reference_count > 1) {
            state_->reference_count--;
            state_ = new State(state_->data_, state_->size_);
        }

        if(size > state_->capacity_){
            Reserve(size * 2);
        }
        
        state_->size_ = size;
    }

    ~CowString() {
        state_->reference_count--;
        if(state_->reference_count == 0) {
            delete state_;
        }
    }

private:
    State *state_;
};

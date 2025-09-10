#include <chrono>
#include <iostream>

// В конструктор передается интервал времени
// До истечения этого времени метод Expired возвращает false,
// а после true
class Timer {
public:
    explicit Timer(std::chrono::milliseconds interval)
    : begin_(std::chrono::steady_clock::now()),
      interval_(interval) {}

    bool Expired() const {
        auto elapsed = std::chrono::steady_clock::now() - begin_;
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) > interval_;
    }

private:
    std::chrono::steady_clock::time_point begin_;
    std::chrono::milliseconds interval_;
};

// В конструктор передается поток 
// В деструкторе в поток записывается время,
// которое прошло с момента создания экземпляра класса
class TimeMeasurer {
public:
    explicit TimeMeasurer(std::ostream& stream)
    : begin_(std::chrono::steady_clock::now()),
      stream_(stream) {}

    ~TimeMeasurer() {
        auto now_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - begin_);
        stream_ << "Elapsed milliseconds: " << elapsed.count() << "\n";
    }

    TimeMeasurer(const TimeMeasurer&) = delete;
    TimeMeasurer& operator=(const TimeMeasurer&) = delete;
    TimeMeasurer(TimeMeasurer&&) = delete;
    TimeMeasurer& operator=(TimeMeasurer&&) = delete;

private:
    std::chrono::steady_clock::time_point begin_;
    std::ostream& stream_;
};

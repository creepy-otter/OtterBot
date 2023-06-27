#pragma once

#include <vector>

namespace otterbot {
class MA {
 public:
  MA() = default;
  MA(size_t period)
      : period_(period), buffer_(period), sum_(0.0), ptr_(0), filled_(false) {}

  MA(const MA&) = default;
  MA(MA&&) = default;
  MA& operator=(const MA&) = default;
  MA& operator=(MA&&) = default;

  void update(double value) {
    if (filled_) {
      // Subtract the oldest value from the sum
      sum_ -= buffer_[ptr_];
    }

    // Replace the oldest value with the new value
    buffer_[ptr_] = value;

    // Add the new value to the sum
    sum_ += value;

    // Move the pointer to the next position in the circular buffer
    ptr_ = (ptr_ + 1) % period_;

    // If we've gone around the circular buffer once, it's now filled
    if (ptr_ == 0) {
      filled_ = true;
    }
  }

  double value() const {
    if (filled_) {
      return sum_ / period_;
    } else {
      // If the buffer isn't filled yet, calculate the average of the values so
      // far
      return sum_ / ptr_;
    }
  }

  bool isFilled() const { return filled_; }

 private:
  size_t period_;
  std::vector<double> buffer_;
  double sum_;
  size_t ptr_;
  bool filled_;
};
}  // namespace otterbot

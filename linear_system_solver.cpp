#include <iostream>
#include <vector>

template <class T>

class Matrix {

 private:
  size_t num_lines_;
  size_t num_columns_;
  std::vector<std::vector<T>> data_;

 public:

 Matrix() noexcept : num_lines_(0), num_columns_(0) {};
 Matrix(std::vector<sdtd::vector<T>> data) {
  data_ = data;
  num_lines_ = data_.;
  if (!data_.empty):
    num_lines_ = data_.size();
    num_columns_ = data_[0].size();
 } else {
  num_lines_ = 0;
  num_columns_ = 0;
 }
};


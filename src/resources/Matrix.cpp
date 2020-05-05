//
//  Matrix.cpp
//  labrtos_concurrent_programming
//
//  Created by Jakub Tomczyński on 28.02.2017.
//
//

#include "Matrix.h"

Matrix::Matrix(int rows, int cols) {
    data_.resize(rows);
    for (size_t r = 0; r < rows; r++) {
        data_[r].resize(cols);
    }
}

void Matrix::rand() {
    for (size_t r = 0; r < rows(); r++) {
        for (size_t c = 0; c < cols(); c++) {
            data_[r][c] = double(std::rand() % 101)/50.0 - 1;
        }
    }
}

void Matrix::print() {
    for (size_t r = 0; r < data_.size(); r++) {
        for (size_t c = 0; c < data_[r].size(); c++) {
            printf("%5.2f ", data_[r][c]);
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::multiply(const Matrix &rhs) {
    if (this->cols() != rhs.rows()) {
        std::cerr << "multiply(): incorrect matrix dimensions" << std::endl;
        return Matrix();
    }
    Matrix result(this->rows(), rhs.cols());
    for (size_t r = 0; r < result.rows(); r++) {
        for (size_t c = 0; c < result.cols(); c++) {
            result.data_[r][c] = 0;
            for (size_t i = 0; i < this->cols(); i++) {
                result.data_[r][c] += this->data_[r][i]*rhs.data_[i][c];
            }
        }
    }
    return result;
}

bool Matrix::compare(const Matrix &other) {
    if (this->cols() != other.cols()) {
        return false;
    }
    if (this->rows() != other.rows()) {
        return false;
    }
    for (size_t r = 0; r < rows(); r++) {
        for (size_t c = 0; c < cols(); c++) {
            if (data_[r][c] != other.data_[r][c]) {
                return false;
            }
        }
    }
    return true;
}

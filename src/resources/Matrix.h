//
//  Matrix.h
//  labrtos_concurrent_programming
//
//  Created by Jakub Tomczyński on 28.02.2017.
//
//

#ifndef Matrix_h
#define Matrix_h

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>

class Matrix {
    /* ... */
public:
    /**
     * Allocates matrix of size rows x cols
     */
    Matrix(int rows = 0, int cols = 0);
    
    /**
     * Fills the matrix with random values of range [-1.0; 1.0]
     */
    void rand();
    
    /**
     * Prints the matrix
     */
    void print();
    
    /**
     * Multiplies matrix by rhs, returns result
     */
    Matrix multiply(const Matrix &rhs);
    
    /**
     * Compares two matrices by size and contents
     */
    bool compare(const Matrix &other);
    
    /**
     * Returns number of rows
     */
    int rows() const {
        return (int)data_.size();
    }
    
    /**
     * Returns number of columns
     */
    int cols() const {
        if (data_.size() > 0) {
            return (int)data_[0].size();
        } else {
            return 0;
        }
    }
private:
    std::vector<std::vector<double> > data_;
};


#endif /* Matrix_h */

#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <cstdlib>
#include <cstring>

class Matrix{
public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, int **matrix);
    Matrix(const Matrix &other);
    Matrix(Matrix &&other);
    ~Matrix();
    Matrix operator=(const Matrix &other);
    Matrix operator=(Matrix &&other);
    Matrix operator+(const Matrix &other);
    Matrix operator-(const Matrix &other);
    Matrix operator*(Matrix & other);
    bool operator==(const Matrix &other);
    friend std::istream &operator>>(std::istream &in, Matrix &matrix);
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
private:
    int rows;
    int cols;
    int **matrix;
};

#endif
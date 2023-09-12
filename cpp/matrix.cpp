#include "matrix.h"

Matrix::Matrix(int rows,int cols)
{
    this->rows = rows;
    this->cols = cols;
    this->matrix = (int**)malloc(this->rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        this->matrix[i] = (int *)malloc(this->cols * sizeof(int));
        memset(this->matrix[i], 0, this->cols * sizeof(int));
    }
}
Matrix::Matrix(int rows, int cols, int **matrix)
{
    this->rows = rows;
    this->cols = cols;
    this->matrix = (int**)malloc(this->rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        this->matrix[i] = (int *)malloc(this->cols * sizeof(int));
        memcpy(this->matrix[i], matrix[i], this->cols * sizeof(int));
    }
}
Matrix::Matrix(const Matrix &other)
{
    this->rows = rows;
    this->cols = cols;
    this->matrix = (int**)malloc(this->rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        this->matrix[i] = (int *)malloc(this->cols * sizeof(int));
        memcpy(this->matrix[i], matrix[i], this->cols * sizeof(int));
    }
}
Matrix::Matrix(Matrix &&other)
{
    this->rows = other.rows;
    this->cols = other.cols;
    this->matrix = other.matrix;
    other.matrix = nullptr;
}
Matrix::~Matrix()
{
    free(this->matrix);
    std::cout << "Destroy Matrix" << std::endl;
}
Matrix Matrix::operator=(const Matrix &other)
{
    this->rows = rows;
    this->cols = cols;
    this->matrix = (int**)malloc(this->rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        this->matrix[i] = (int *)malloc(this->cols * sizeof(int));
        memcpy(this->matrix[i], matrix[i], this->cols * sizeof(int));
    }
    return *this;
}
Matrix Matrix::operator=(Matrix &&other)
{
    this->rows = other.rows;
    this->cols = other.cols;
    this->matrix = other.matrix;
    other.matrix = nullptr;
    return *this;
}
Matrix Matrix::operator+(const Matrix &other)
{
    Matrix result(this->rows, this->cols);
    for (int i = 0;i<this->rows;i++)
        for (int j = 0;j<this->cols;j++)
            result.matrix[i][j] = this->matrix[i][j] + other.matrix[i][j];
    return result;
}
Matrix Matrix::operator-(const Matrix &other)
{
    Matrix result(this->rows, this->cols);
    for (int i = 0;i<this->rows;i++)
        for (int j = 0;j<this->cols;j++)
            result.matrix[i][j] = this->matrix[i][j] - other.matrix[i][j];
    return result;
}
Matrix Matrix::operator*(Matrix& other)
{
    Matrix result(this->rows, other.cols);
    for (int i = 0; i < this->rows; i++)
        for (int j = 0; j < other.cols; j++)
        {
            result.matrix[i][j] = 0;
            for (int k = 0; k < this->cols; k++)
                result.matrix[i][j] += this->matrix[i][k] * other.matrix[k][j];
        }
    return result;
}
bool Matrix::operator==(const Matrix &other)
{
    if (this->rows != other.rows || this->cols != other.cols)
        return false;
    for (int i = 0;i<this->rows;i++)
        for (int j = 0;j<this->cols;j++)
            if (this->matrix[i][j] != other.matrix[i][j])
                return false;
    return true;
}
std::istream &operator>>(std::istream &in, Matrix &matrix)
{
    for (int i = 0;i<matrix.rows;i++)
        for (int j = 0;j<matrix.cols;j++)
            in >> matrix.matrix[i][j];
    return in;
}
std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    for (int i = 0;i<matrix.rows;i++)
    {
        for (int j = 0;j<matrix.cols;j++)
            os << matrix.matrix[i][j] << " ";
        os << std::endl;
    }
    return os;
}
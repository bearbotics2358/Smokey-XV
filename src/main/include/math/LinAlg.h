#pragma once

#include <utility>
#include <stdexcept>

#include "types.h"

// put these macros to at the start of a method to restrict that method to a certain type of matrix
#define ASSERT_MAT(n) static_assert(rows() == (n) && cols() == (n), "Matrix is not a square matrix of the right dimension");
#define ASSERT_VEC(n) static_assert(rows() == (n) && cols() == 1, "Matrix is not a vector of the right dimension");

#define ASSERT_MAT2 ASSERT_MAT(2)
#define ASSERT_MAT3 ASSERT_MAT(3)
#define ASSERT_MAT4 ASSERT_MAT(4)

#define ASSERT_VEC2 ASSERT_VEC(2)
#define ASSERT_VEC3 ASSERT_VEC(3)
#define ASSERT_VEC4 ASSERT_VEC(4)

// allows method to be defined on vector in a certain range
#define ASSERT_VEC_RANGE(low, high)     \
static_assert(rows() >= (low) && rows() <= (high) && cols() == 1, "Matrix is not a vector of the right dimension");

// define an eccesor mathod on a veector between low and high dimension,
// with name name and accesing data at index index
#define ACCESOR_METHODS(name, index, low, high) \
T& name() & {                                   \
    ASSERT_VEC_RANGE(low, high)                 \
    static_assert(index < size(), "error");     \
    return data[index];                         \
}                                               \
const T& name() const& {                        \
    ASSERT_VEC_RANGE(low, high)                 \
    static_assert(index < size(), "error");     \
    return data[index];                         \
}                                               \
T name() && {                                   \
    ASSERT_VEC_RANGE(low, high)                 \
    static_assert(index < size(), "error");     \
    return data[index];                         \
}

template<typename T, usize r, usize c>
class Matrix {
    public:
        using Mtype = Matrix<T, r, c>;

        Matrix() = default;

        constexpr Matrix(T n) noexcept : data{n} {}

        // this makes a constructor that takes as many elements as the matrix will hold and initilizes the matrix with these arguments
        // the elements start on the first row, move across all the columns, than go to the second row
        template<typename... Args>
        constexpr Matrix(Args... args) noexcept : data{std::forward<Args>(args)...} {
            static_assert(sizeof...(args) == size(), "Incorrect number of arguments for Matrix");
        }

        constexpr static usize rows() {
            return r;
        }

        constexpr static usize cols() {
            return c;
        }

        constexpr static usize size() {
            return r * c;
        }

        // indexing methods
        T& operator[](usize index) & {
            return data[index_inner(index)];
        }

        const T& operator[](usize index) const& {
            return data[index_inner(index)];
        }

        T operator[](usize index) && {
            return data[index_inner(index)];
        }

        T& at(usize x, usize y) & {
            return data[at_inner(x, y)];
        }

        const T& at(usize x, usize y) const& {
            return data[at_inner(x, y)];
        }

        T at(usize x, usize y) && {
            return data[at_inner(x, y)];
        }

        ACCESOR_METHODS(x, 0, 1, 4)
        ACCESOR_METHODS(y, 1, 2, 4)
        ACCESOR_METHODS(z, 2, 3, 4)
        ACCESOR_METHODS(w, 3, 4, 4)

        // operator overloads
        // adition does alement wise adding, or adding a constant to every element
        constexpr Mtype& operator+=(const Mtype& other) {
            for (usize i = 0; i < size(); i ++) {
                data[i] += other.data[i];
            }
            return *this;
        }

        constexpr Mtype operator+(const Mtype& other) const {
            Mtype out(*this);
            out += other;
            return out;
        }

        constexpr Mtype& operator+=(const T& n) {
            for (usize i = 0; i < size(); i ++) {
                data[i] += n;
            }
            return *this;
        }

        constexpr Mtype operator+(const T& n) const {
            Mtype out(*this);
            out += n;
            return out;
        }
    
        // subtraction does alement wise subtraction, or subtracting a constant from every element
        constexpr Mtype& operator-=(const Mtype& other) {
            for (usize i = 0; i < size(); i ++) {
                data[i] -= other.data[i];
            }
            return *this;
        }

        constexpr Mtype operator-(const Mtype& other) const {
            Mtype out(*this);
            out -= other;
            return out;
        }

        constexpr Mtype& operator-=(const T& n) {
            for (usize i = 0; i < size(); i ++) {
                data[i] -= n;
            }
            return *this;
        }

        constexpr Mtype operator-(const T& n) const {
            Mtype out(*this);
            out -= n;
            return out;
        }

        // multiplying or dividing by a number multiplies or divides every element in the matrix
        constexpr Mtype& operator*=(const T& n) {
            for (usize i = 0; i < size(); i ++) {
                data[i] *= n;
            }
            return *this;
        }

        constexpr Mtype operator*(const T& n) const {
            Mtype out(*this);
            out *= n;
            return out;
        }

        constexpr Mtype& operator/=(const T& n) {
            for (usize i = 0; i < size(); i ++) {
                data[i] /= n;
            }
            return *this;
        }

        constexpr Mtype operator/(const T& n) const {
            Mtype out(*this);
            out /= n;
            return out;
        }

        // matrix multiplication
        template<usize other_cols>
        constexpr Matrix<T, r, other_cols>& operator*=(const Matrix<T, c, other_cols>& other) {
            // temporary array
            T row_tmp[cols()];
            for (usize row = 0; row < rows(); row ++) {
                for (usize col = 0; col < other_cols; col ++) {
                    T tmp;
                    for (usize i = 0; i < cols(); i ++) {
                        // TODO: maybe avoid use of at
                        tmp += at(i, row) * other.at(col, i);
                    }
                    row_tmp[col] = tmp;
                }
                // copy temporary row to matrix
                usize base_index = row * cols();
                for (usize col = 0; col < cols(); col ++) {
                    data[base_index + col] = row_tmp[col];
                }
            }
            return *this;
        }

        template<usize other_cols>
        constexpr Matrix<T, r, other_cols> operator*(const Matrix<T, c, other_cols>& other) const {
            Matrix<T, r, other_cols> out;
            for (usize row = 0; row < rows(); row ++) {
                for (usize col = 0; col < other_cols; col ++) {
                    T tmp;
                    // TODO: maybe don't use at if performance is needed
                    for (usize i = 0; i < cols(); i ++) {
                        tmp += at(i, row) * at(col, i);
                    }
                    out.at(col, row) = tmp;
                }
            }
            return out;
        }

    private:
        constexpr static usize get_index(usize x, usize y) {
            return x * cols() + y;
        }

        usize index_inner(usize index) const {
            if (index >= size()) {
                throw std::out_of_range("Mat out of range");
            }
            return index;
        }

        usize at_inner(usize x, usize y) const {
            if (x >= cols() || y >= rows()) {
                throw std::out_of_range("Mat out of range");
            }
            return get_index(x, y);
        }

        T data[size()];
};

using Vec2 = Matrix<num, 2, 1>;
using Vec3 = Matrix<num, 3, 1>;
using Vec4 = Matrix<num, 4, 1>;

using Mat2 = Matrix<num, 2, 2>;
using Mat3 = Matrix<num, 3, 3>;
using Mat4 = Matrix<num, 4, 4>;
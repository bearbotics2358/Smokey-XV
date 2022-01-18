#pragma once

#include "types.h"

template<typename T, usize d>
class Mat {
    Mat() = default;

    // this makes a constructor that takes as many elements as the matrix will hold and initilizes the matrix with these arguments
    // the elements start on the first row, move across all the columns, than go to the second row
    template<typename... Args>
    constexpr Vec(Args... args) noexcept : data{std::forward<Args>(args)...} {
        static_assert(sizeof...(args) == size(), "Incorrect number of arguments for Mat");
    }

    constexpr static usize dim() {
        return d;
    }

    constexpr static usize size() {
        return d * d;
    }

    T& operator[](usize index) & {
        return *index_inner(index);
    }

    const T& operator[](usize index) const& {
        return *index_inner(index);
    }

    T operator[](usize index) && {
        return *index_inner(index);
    }

    T& at(usize x, usize y) & {
        return *at_inner(x, y);
    }

    const T& at(usize x, usize y) const& {
        return *at_inner(x, y);
    }

    T at(usize x, usize y) && {
        return *at_inner(x, y);
    }

    constexpr Mat<T, d>& operator+=(T number) {
        for (usize i = 0; i < size(); i ++) {
            data[i] += number;
        }
        return this;
    }

    constexpr Mat<T, d>& operator+=(const Mat<T, d>& other) {
        for (usize i = 0; i < size(); i ++) {
            data[i] += other[i];
        }
        return this;
    }

    constexpr Mat<T, d>& operator-=(T number) {
        for (usize i = 0; i < size(); i ++) {
            data[i] -= number;
        }
        return this;
    }

    constexpr Mat<T, d>& operator-=(const Mat<T, d>& other) {
        for (usize i = 0; i < size(); i ++) {
            data[i] -= other[i];
        }
        return this;
    }

    constexpr Mat<T, d>& operator*=(T number) {
        for (usize i = 0; i < size(); i ++) {
            data[i] *= number;
        }
        return this;
    }

    constexpr Mat<T, d>& operator*=(const Mat<T, d>& other) {
        // temporary array
        T row_tmp[dim()];
        for (usize row = 0; row < dim(); row ++) {
            for (usize col = 0; col < dim(); col ++) {
                T tmp;
                for (usize i = 0; i < dim(); i ++) {
                    // TODO: maybe avoid use of at
                    tmp += at(row, i) * other.at(i, col);
                }
                row_tmp[col] = tmp;
            }
            // copy temporary row to matrix
            usize base_index = row * dim();
            for (usize col = 0; col < dim(); col ++) {
                data[base_index + col] = row_tmp[col];
            }
        }
    }

    constexpr Mat<T, d>& operator/=(T number) {
        for (usize i = 0; i < size(); i ++) {
            data[i] /= number;
        }
        return this;
    }

    private:
        T *index_inner(usize index) {
            if (index >= size()) {
                throw std::out_of_range("Mat out of range");
            }
            return data + index;
        }

        T *at_inner(usize x, usize y) {
            if (x >= dim() || y >= dim()) {
                throw std::out_of_range("Mat out of range");
            }
            return data + (x * dim() + y);
        }

        T data[size()];
};

template<typename T>
class Mat2t : public Mat<T, 2> {
    Mat2t() = default;

    Mat2t(T n):
    Mat<T, 2>(n, n,
              n, n) {}

    Mat2t(T x1, T x2,
          T y1, T y2):
    Mat<T, 2>(x1, x2,
              y1, y2) {}
};

template<typename T>
class Mat3t : public Mat<T, 3> {
    Mat3t() = default;

    Mat3t(T n):
    Mat<T, 3>(n, n, n,
              n, n, n,
              n, n, n) {}

    Mat3t(T x1, T x2, T x3,
          T y1, T y2, T y3,
          T z1, T z2, T z3):
    Mat<T, 3>(x1, x2, x3,
              y1, y2, y3,
              z1, z2, z3) {}
};

template<typename T>
class Mat4t : public Mat<T, 4> {
    Mat4t() = default;

    Mat4t(T n):
    Mat<T, 4>(n, n, n, n,
              n, n, n, n,
              n, n, n, n,
              n, n, n, n) {}

    Mat4t(T x1, T x2, T x3, T x4,
          T y1, T y2, T y3, T y4,
          T z1, T z2, T z3, T z4,
          T w1, T w2, T w3, T w4):
    Mat<T, 4>(x1, x2, x3, x4
              y1, y2, y3, y4
              z1, z2, z3, z4
              w1, w2, w3, w4) {}
};
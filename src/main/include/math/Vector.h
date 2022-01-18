#pragma once

#include <stdexcept>

#include "types.h"

#define ACCESOR_METHODS(name, index)    \
T& name() & {                           \
    static_assert(index < size());      \
    return (*this)[index];              \
}                                       \
const T& name() const& {                \
    static_assert(index < size());      \
    return (*this)[index];              \
}                                       \
T name() && {                           \
    static_assert(index < size());      \
    return (*this)[index];              \
}

#define VECTOR_OPS(vtype, ntype)                               \
constexpr vtype operator+(const vtype& vector) const {  \
    vtype out(*this);                                   \
    out += vector;                                      \
    return out;                                         \
}                                                       \
constexpr vtype operator+(ntype num) const {            \
    vtype out(*this);                                   \
    out += num;                                         \
    return out;                                         \
}                                                       \
constexpr vtype operator-(const vtype& vector) const {  \
    vtype out(*this);                                   \
    out -= vector;                                      \
    return out;                                         \
}                                                       \
constexpr vtype operator-(ntype num) const {            \
    vtype out(*this);                                   \
    out -= num;                                         \
    return out;                                         \
}                                                       \
constexpr vtype operator*(const vtype& vector) const {  \
    vtype out(*this);                                   \
    out *= vector;                                      \
    return out;                                         \
}                                                       \
constexpr vtype operator*(ntype num) const {            \
    vtype out(*this);                                   \
    out *= num;                                         \
    return out;                                         \
}                                                       \
constexpr vtype operator/(const vtype& vector) const {  \
    vtype out(*this);                                   \
    out /= vector;                                      \
    return out;                                         \
}                                                       \
constexpr vtype operator/(ntype num) const {            \
    vtype out(*this);                                   \
    out /= num;                                         \
    return out;                                         \
}

template<typename T, usize d>
class Vec {
    public:
        Vec() = default;

        // this makes a constructor that takes as many elements as the vector will hold and initilizes the vector with these arguments
        template<typename... Args>
        constexpr Vec(Args... args) noexcept : data{std::forward<Args>(args)...} {
            static_assert(sizeof...(args) == size(), "Incorrect number of arguments for Vec");
        }

        // size of the vector
        constexpr static usize size() {
            return d;
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

        constexpr Vec<T, d>& operator+=(T number) {
            for (usize i = 0; i < size(); i ++) {
                data[i] += number;
            }
            return this;
        }

        constexpr Vec<T, d>& operator+=(const Vec<T, d>& other) {
            for (usize i = 0; i < size(); i ++) {
                data[i] += other[i];
            }
            return this;
        }

        constexpr Vec<T, d>& operator-=(T number) {
            for (usize i = 0; i < size(); i ++) {
                data[i] -= number;
            }
            return this;
        }

        constexpr Vec<T, d>& operator-=(const Vec<T, d>& other) {
            for (usize i = 0; i < size(); i ++) {
                data[i] -= other[i];
            }
            return this;
        }

        constexpr Vec<T, d>& operator*=(T number) {
            for (usize i = 0; i < size(); i ++) {
                data[i] *= number;
            }
            return this;
        }

        constexpr Vec<T, d>& operator*=(const Vec<T, d>& other) {
            for (usize i = 0; i < size(); i ++) {
                data[i] *= other[i];
            }
            return this;
        }

        constexpr Vec<T, d>& operator/=(T number) {
            for (usize i = 0; i < size(); i ++) {
                data[i] /= number;
            }
            return this;
        }

        constexpr Vec<T, d>& operator/=(const Vec<T, d>& other) {
            for (usize i = 0; i < size(); i ++) {
                data[i] /= other[i];
            }
            return this;
        }

    private:
        T *index_inner(usize index) {
            if (index >= size()) {
                throw std::out_of_range("Vec out of range");
            }
            return data + index;
        }

        T data[size()];
};

// specilizations for 2, 3, and 4 dimensional vectors
template<typename T>
class Vec2t : public Vec<T, 2> {
    public:
        Vec2t() = default;

        // initilizes the vector with all the same element
        constexpr Vec2t(T n) : Vec<T, 2>(n, n) {}

        // initilizes the x and y components of the vector
        constexpr Vec2t(T x, T y) : Vec<T, 2>(x, y) {}

        // size of the vector
        // needed here as well because of accesor methods use it
        constexpr static usize size() {
            return 2;
        }

        ACCESOR_METHODS(x, 0)
        ACCESOR_METHODS(y, 1)

        VECTOR_OPS(Vec2t<T>, T)
};

template<typename T>
class Vec3t : public Vec<T, 3> {
    public:
        Vec3t() = default;

        // initilizes the vector with all the same element
        constexpr Vec3t(T n) : Vec<T, 3>(n, n, n) {}

        // initilizes the x, y, and z components of the vector
        constexpr Vec3t(T x, T y, T z) : Vec<T, 3>(x, y, z) {}

        // size of the vector
        constexpr static usize size() {
            return 3;
        }

        ACCESOR_METHODS(x, 0)
        ACCESOR_METHODS(y, 1)
        ACCESOR_METHODS(z, 2)

        VECTOR_OPS(Vec3t<T>, T)
};

template<typename T>
class Vec4t : public Vec<T, 4> {
    public:
        Vec4t() = default;

        // initilizes the vector with all the same element
        constexpr Vec4t(T n) : Vec<T, 4>(n, n, n, n) {}

        // initilizes the x, y, z, and w components of the vector
        constexpr Vec4t(T x, T y, T z, T w) : Vec<T, 4>(x, y, z, w) {}

        // size of the vector
        constexpr static usize size() {
            return 4;
        }

        ACCESOR_METHODS(x, 0)
        ACCESOR_METHODS(y, 1)
        ACCESOR_METHODS(z, 2)
        ACCESOR_METHODS(w, 3)

        VECTOR_OPS(Vec4t<T>, T)
};

using Vec2 = Vec2t<num>;
using Vec3 = Vec3t<num>;
using Vec4 = Vec4t<num>;
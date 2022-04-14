#pragma once

#include <array>
#include <cmath>
#include <sstream>
#include <string>

namespace tr {

template <typename Type>
struct Vec2 {
    typedef Type type;

    union coordinates_t {
        struct texture_t {
            Type u{}, v{};
        } t;
        struct polar_t {
            Type x{}, y{};
        } p;
        std::array<Type, 2> raw{};
    } coords{};

    Vec2() : coords{} {}

    Vec2(Type _u, Type _v) {
        coords.t.u = _u;
        coords.t.v = _v;
    }

    Type x() const { return coords.p.x; }
    Type y() const { return coords.p.y; }
    Type u() const { return coords.t.u; }
    Type v() const { return coords.t.v; }

    Type& operator[](size_t index) { return coords.raw.at(index); }
    Type operator[](size_t index) const { return coords.raw.at(index); }

    inline Vec2<Type> operator+(const Vec2<Type>& v) const {
        return Vec2<Type>(u() + v.u(), v() + v.v());
    }

    inline Vec2<Type> operator-(const Vec2<Type>& v) const {
        return Vec2<Type>(u() - v.u(), v() - v.v());
    }

    inline Vec2<Type> operator*(float f) const {
        return Vec2<Type>(static_cast<Type>(u() * f), static_cast<Type>(v() * f));
    }

    std::string to_string() const {
        std::stringstream s;
        s << static_cast<std::string>("(") << x() << ", " << y()
          << static_cast<std::string>(")\n");
        return s.str();
    }

    template <typename>
    friend std::ostream& operator<<(std::ostream& s, Vec2<Type>& v);
};

template <typename Type>
struct Vec3 {
    typedef Type type;

    union coords {
        struct texture_t {
            Type x{}, y{}, z{};
        } t;
        struct polar_t {
            Type ivert{}, iuv{}, inorm{};
        } p;
        std::array<Type, 3> raw{};
    } coords{};

    Vec3() : coords{} {}

    Vec3(Type _x, Type _y, Type _z) {
        coords.t.x = _x;
        coords.t.y = _y;
        coords.t.z = _z;
    }

    Type x() const { return coords.t.x; }
    Type y() const { return coords.t.y; }
    Type z() const { return coords.t.z; }
    Type ivert() const { return coords.p.ivert; }
    Type iuv() const { return coords.t.iuv; }
    Type inorm() const { return coords.t.inorm; }

    Type& operator[](size_t index) { return coords.raw.at(index); }
    Type operator[](size_t index) const { return coords.raw.at(index); }

    inline Vec3<Type> operator^(const Vec3<Type>& v) const {
        return Vec3<Type>(y() * v.z() - z() * v.y(), z() * v.x() - x() * v.z(),
                          x() * v.y() - y() * v.x());
    }

    inline Vec3<Type> operator+(const Vec3<Type>& v) const {
        return Vec3<Type>(x() + v.x(), y() + v.y(), z() + v.z());
    }

    inline Vec3<Type> operator-(const Vec3<Type>& v) const {
        return Vec3<Type>(x() - v.x(), y() - v.y(), z() - v.z());
    }

    inline Vec3<Type> operator*(float f) const {
        return Vec3<Type>(x() * f, y() * f, z() * f);
    }

    inline Type operator*(const Vec3<Type>& v) const {
        return x() * v.x() + y() * v.y() + z() * v.z();
    }

    float norm() const { return std::sqrt(x() * x() + y() * y() + z() * z()); }

    Vec3<Type>& normalize(Type l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    std::string to_string() const {
        std::stringstream s;
        s << static_cast<std::string>("(") << x() << ", " << y() << ", " << z()
          << static_cast<std::string>(")\n");
        return s.str();
    }

    template <typename>
    friend std::ostream& operator<<(std::ostream& s, Vec3<Type>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <typename Type>
std::ostream& operator<<(std::ostream& s, Vec2<Type>& v) {
    s << v.to_string();
    return s;
}

template <typename Type>
std::ostream& operator<<(std::ostream& s, Vec3<Type>& v) {
    s << v.to_string();
    return s;
}

}  // namespace tr

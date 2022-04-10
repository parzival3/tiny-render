#pragma once

#include <array>
#include <cmath>
#include <string>

namespace tr {

template <typename Type>
struct Vec2 {
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

  inline Vec2<Type> operator+(const Vec2<Type>& V) const {
    return Vec2<Type>(coords.t.u + V.coords.t.u, coords.t.v + V.coords.t.v);
  }

  inline Vec2<Type> operator-(const Vec2<Type>& V) const {
    return Vec2<Type>(coords.t.u - V.coords.t.u, coords.t.v - V.coords.t.v);
  }

  inline Vec2<Type> operator*(float f) const {
    return Vec2<Type>(coords.t.u * f, coords.t.v * f);
  }

  template <typename>
  friend std::ostream& operator<<(std::ostream& s, Vec2<Type>& v);
};

template <typename Type>
struct Vec3 {
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

  inline Vec3<Type> operator^(const Vec3<Type>& v) const {
    return Vec3<Type>(coords.t.y * coords.t.z - coords.t.z * v.coords.t.y,
                      coords.t.z * v.coords.t.x - coords.t.x * v.coords.t.z,
                      coords.t.x * v.coords.t.y - coords.t.y * v.coords.t.x);
  }

  inline Vec3<Type> operator+(const Vec3<Type>& v) const {
    return Vec3<Type>(coords.t.x + v.coords.t.x, coords.t.y + v.coords.t.y,
                      coords.t.z + v.coords.t.z);
  }

  inline Vec3<Type> operator-(const Vec3<Type>& v) const {
    return Vec3<Type>(coords.t.x - v.coords.t.x, coords.t.y - v.coords.t.y,
                      coords.t.z - v.coords.t.z);
  }

  inline Vec3<Type> operator*(float f) const {
    return Vec3<Type>(coords.t.x * f, coords.t.y * f, coords.t.z * f);
  }

  inline Type operator*(const Vec3<Type>& v) const {
    return coords.t.x * v.coords.t.x + coords.t.y * v.coords.t.y +
           coords.t.z * v.coords.t.z;
  }

  float norm() const {
    return std::sqrt(coords.t.x * coords.t.x + coords.t.y * coords.t.y +
                     coords.t.z * coords.t.z);
  }

  Vec3<Type>& normalize(Type l = 1) {
    *this = (*this) * (l / norm());
    return *this;
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
  s << static_cast<std::string>("(") << v.coords.t.x << ", " << v.coords.t.y
    << static_cast<std::string>(")\n");
  return s;
}

template <typename Type>
std::ostream& operator<<(std::ostream& s, Vec3<Type>& v) {
  s << static_cast<std::string>("(") << v.coords.t.x << ", " << v.coords.t.y
    << ", " << v.coords.t.z << static_cast<std::string>(")\n");
  return s;
}

}  // namespace tr

#pragma once

#include <string>
#include <vector>

#include "geometry.hpp"

namespace tr {

class Model {
 public:
  Model(const std::string& filename);
  [[nodiscard]] size_t verts_number() const;
  [[nodiscard]] size_t faces_number() const;
  [[nodiscard]] Vec3f vert(size_t i) const;
  [[nodiscard]] std::vector<size_t> face(size_t i) const;

 private:
  std::vector<Vec3f> verts_;
  std::vector<std::vector<size_t>> faces_;
};

}  // namespace tr

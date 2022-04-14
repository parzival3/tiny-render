#pragma once

#include <string>
#include <vector>

#include "geometry.hpp"

namespace tr {

class Model {
   public:
    Model(const std::string& filename);
    [[nodiscard]] size_t verts_size() const;
    [[nodiscard]] size_t faces_size() const;
    [[nodiscard]] Vec3f vert(size_t index) const;
    [[nodiscard]] std::vector<size_t> face(size_t index) const;
    [[nodiscard]] std::array<Vec3f, 3> face_coords(size_t index) const;
    [[nodiscard]] std::vector<std::vector<size_t>> faces() const { return faces_; }

   private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<size_t>> faces_;
};

}  // namespace tr

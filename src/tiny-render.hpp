#pragma once
#include <optional>

#include "geometry.hpp"
#include "model.hpp"
#include "tga-lib/tgaimage.hpp"

namespace tr {

// helper functions
template <typename VecT>
std::array<VecT, 3> map_obj_to_screen(const std::array<VecT, 3>& world_coords,
                                      const TGAImage& image) {
    std::array<VecT, 3> pts;

    for (size_t i{}; i < 3; ++i) {
        pts[i] = VecT(
            static_cast<int>((world_coords[i].x() + 1.0f) * image.width() / 2.0f + 1.0f),
            static_cast<int>((world_coords[i].y() + 1.0f) * image.height() / 2.0f + 1.0f),
            static_cast<int>(world_coords[i].z()));
    }
    return pts;
}

std::optional<Vec3f> barycentric(const std::array<Vec3f, 3>& pts, const Vec3f& p);

std::array<Vec2f, 2> get_bbox(const std::array<Vec3f, 3>& pts, TGAImage& image);

Vec3f normal_vector(std::array<Vec3f, 3> world_coords);

// drwaing primitives
void line(int x_0, int y_0, int x_1, int y_1, TGAImage& image, const TGAColor& color);

void line(const Vec2i& t_0, const Vec2i& t_1, TGAImage& image, const TGAColor& color);

void triangle(const Vec2i& t_0, const Vec2i& t_1, const Vec2i& t_2, TGAImage& image,
              const TGAColor& color);

void triangle(const std::array<tr::Vec3f, 3>& pts, std::vector<int>& z_buffer,
              TGAImage& image, const TGAColor& color);

void draw_with_z_buffer(const std::array<Vec3f, 3>& pts, std::vector<int>& z_buffer,
                        tr::Vec3f& P, tr::Vec3f& screen, TGAImage& image,
                        const TGAColor& color);

}  // namespace tr

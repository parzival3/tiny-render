#pragma once
#include <optional>

#include "geometry.hpp"
#include "model.hpp"
#include "tga-lib/tgaimage.hpp"

namespace tr {
void say_hello();

void line(int x_0, int y_0, int x_1, int y_1, TGAImage& image,
          const TGAColor& color);

void line(const Vec2i& t_0, const Vec2i& t_1, TGAImage& image,
          const TGAColor& color);

void triangle(const Vec2i& t_0, const Vec2i& t_1, const Vec2i& t_2,
              TGAImage& image, const TGAColor& color);

void line_render(int width, int height, const tr::Model& model,
                 const TGAColor& color, const std::string& tga_filename);

void triangle_render_0(int width, int height, const TGAColor& color,
                       const std::string& tga_filename);

void triangle_render_1(int width, int height, const TGAColor& color,
                       const std::string& tga_filename);

void triangle(const std::array<Vec2i, 3>& pts, TGAImage& image,
              const TGAColor& color);

void color_obj(int width, int height, const tr::Model& model,
               const std::string& tga_filename);

std::optional<Vec3f> barycentric(const std::array<Vec2i, 3>& pts,
                                 const Vec2i& p);

std::array<Vec2i, 2> get_bbox(const std::array<Vec2i, 3>& pts, TGAImage& image);

void color_obj(TGAImage& image, const Model& model);
void light_mono_obj(TGAImage& image, const Model& model);
}  // namespace tr

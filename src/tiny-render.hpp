#pragma once

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

void triangle_render(int width, int height, const TGAColor& color,
                     const std::string& tga_filename);
}  // namespace tr

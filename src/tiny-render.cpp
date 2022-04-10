#include "tiny-render.hpp"

#include <iostream>

void tr::line(int x_0, int y_0, int x_1, int y_1, TGAImage& image,
              const TGAColor& color) {
  bool steep{};
  if (std::abs(x_0 - x_1) < std::abs(y_0 - y_1)) {
    std::swap(x_0, y_0);
    std::swap(x_1, y_1);
    steep = true;
  }

  if (x_0 > x_1) {
    std::swap(x_0, x_1);
    std::swap(y_0, y_1);
  }

  for (int x{x_0}; x <= x_1; ++x) {
    double t = (x - x_0) / static_cast<double>(x_1 - x_0);
    int y = static_cast<int>(y_0 * (1.0 - t) + y_1 * t);
    if (steep)
      image.set(y, x, color);
    else
      image.set(x, y, color);
  }
}

void tr::line(const Vec2i& t_0, const Vec2i& t_1, TGAImage& image,
              const TGAColor& color) {
  line(t_0.coords.p.x, t_0.coords.p.y, t_1.coords.p.x, t_1.coords.p.y, image,
       color);
}

void tr::triangle(const tr::Vec2i& t_0, const tr::Vec2i& t_1,
                  const tr::Vec2i& t_2, TGAImage& image,
                  const TGAColor& color) {
  line(t_0, t_1, image, color);
  line(t_1, t_2, image, color);
  line(t_2, t_0, image, color);
}

void tr::triangle_render(int width, int height, const TGAColor& color,
                         const std::string& tga_filename) {
  TGAImage image(width, height, TGAImage::RGB);
  tr::Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
  tr::Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
  tr::Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
  tr::triangle(t0[0], t0[1], t0[2], image, color);
  tr::triangle(t1[0], t1[1], t1[2], image, color);
  tr::triangle(t2[0], t2[1], t2[2], image, color);
  image.write_tga_file(tga_filename);
}

void tr::line_render(int width, int height, const tr::Model& model,
                     const TGAColor& color, const std::string& tga_filename) {
  TGAImage image(width, height, TGAImage::RGB);
  for (size_t i{}; i < model.faces_number(); ++i) {
    std::vector<size_t> face = model.face(i);
    for (size_t j{}; j < 3; ++j) {
      tr::Vec3f v_0 = model.vert(face[j]);
      tr::Vec3f v_1 = model.vert(face[(j + 1) % 3]);
      int x_0 = static_cast<int>((v_0.coords.t.x + 1) * width / 2.0f);
      int y_0 = static_cast<int>((v_0.coords.t.y + 1) * height / 2.0f);
      int x_1 = static_cast<int>((v_1.coords.t.x + 1) * width / 2.0f);
      int y_1 = static_cast<int>((v_1.coords.t.y + 1) * height / 2.0f);
      tr::line(x_0, y_0, x_1, y_1, image, color);
    }
  }

  image.write_tga_file(tga_filename);
}

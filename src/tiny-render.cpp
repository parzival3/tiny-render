#include "tiny-render.hpp"

#include <iostream>

#include "common.hpp"

enum class thalf_e { LOWER, HIGHER };

static auto line_interpolator(int x_0, int y_0, int x_1, int y_1) {
  using namespace ranges;
  return views::for_each(views::iota(x_0, x_1), [=](int x) {
    double t = (x - x_0) / static_cast<double>(x_1 - x_0);
    int y = static_cast<int>(y_0 * (1.0 - t) + y_1 * t);
    return yield(tr::Vec2i(x, y));
  });
}

static void fill_x(int x_0, int x_1, int y, TGAImage& image,
                   const TGAColor& color) {
  ranges::for_each(ranges::views::iota(x_0, x_1),
                   [&](int x) { image.set(x, y, color); });
}

static std::array<tr::Vec2i, 3> sort_triangle(
    const std::array<tr::Vec2i, 3>& vecs) {
  std::array<tr::Vec2i, 3> tri{vecs};
  if (tri[0].coords.p.y > tri[1].coords.p.y) {
    std::swap(tri[0], tri[1]);
  }

  if (tri[0].coords.p.y > tri[2].coords.p.y) {
    std::swap(tri[0], tri[2]);
  }

  if (tri[1].coords.p.y > tri[2].coords.p.y) {
    std::swap(tri[1], tri[2]);
  }

  return tri;
}

static void fill_triangle(std::array<tr::Vec2i, 3> vec, thalf_e half,
                          TGAImage& image, const TGAColor& color) {
  int total_height = vec[2].coords.p.y - vec[0].coords.p.y;
  int segment_height = half == thalf_e::LOWER
                           ? vec[1].coords.p.y - vec[0].coords.p.y + 1
                           : vec[2].coords.p.y - vec[1].coords.p.y + 1;

  int low_y = half == thalf_e::LOWER ? vec[0].coords.p.y : vec[1].coords.p.y;
  int high_y = half == thalf_e::LOWER ? vec[1].coords.p.y : vec[2].coords.p.y;

  using namespace ranges;
  for_each(ranges::views::iota(low_y, high_y), [&](int y) {
    float alpha = (y - vec[0].coords.p.y) / static_cast<float>(total_height);
    float beta = (y - low_y) / static_cast<float>(segment_height);
    tr::Vec2i A = vec[0] + (vec[2] - vec[0]) * alpha;
    tr::Vec2i B = half == thalf_e::LOWER ? vec[0] + (vec[1] - vec[0]) * beta
                                         : vec[1] + (vec[2] - vec[1]) * beta;
    if (A.coords.p.x > B.coords.p.x) std::swap(A, B);
    fill_x(A.coords.p.x, B.coords.p.x, y, image, color);
  });
}

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

  ranges::for_each(line_interpolator(x_0, y_0, x_1, y_1), [&](const auto& vec) {
    if (steep)
      image.set(vec.coords.p.y, vec.coords.p.x, color);
    else
      image.set(vec.coords.p.x, vec.coords.p.y, color);
  });
}

void tr::line(const Vec2i& t_0, const Vec2i& t_1, TGAImage& image,
              const TGAColor& color) {
  line(t_0.coords.p.x, t_0.coords.p.y, t_1.coords.p.x, t_1.coords.p.y, image,
       color);
}

void tr::triangle(const tr::Vec2i& t_0, const tr::Vec2i& t_1,
                  const tr::Vec2i& t_2, TGAImage& image,
                  const TGAColor& color) {
  auto sorted = sort_triangle({t_0, t_1, t_2});

  fill_triangle(sorted, thalf_e::LOWER, image, color);
  fill_triangle(sorted, thalf_e::HIGHER, image, color);
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

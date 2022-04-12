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
  if (tri[0].y() > tri[1].y()) {
    std::swap(tri[0], tri[1]);
  }

  if (tri[0].y() > tri[2].y()) {
    std::swap(tri[0], tri[2]);
  }

  if (tri[1].y() > tri[2].y()) {
    std::swap(tri[1], tri[2]);
  }

  return tri;
}

static void fill_triangle(std::array<tr::Vec2i, 3> vec, thalf_e half,
                          TGAImage& image, const TGAColor& color) {
  int total_height = vec[2].coords.p.y - vec[0].coords.p.y;
  int segment_height = half == thalf_e::LOWER ? vec[1].y() - vec[0].y() + 1
                                              : vec[2].y() - vec[1].y() + 1;

  int low_y = half == thalf_e::LOWER ? vec[0].y() : vec[1].y();
  int high_y = half == thalf_e::LOWER ? vec[1].y() : vec[2].y();

  using namespace ranges;
  for_each(ranges::views::iota(low_y, high_y), [&](int y) {
    float alpha = (y - vec[0].y()) / static_cast<float>(total_height);
    float beta = (y - low_y) / static_cast<float>(segment_height);
    tr::Vec2i A = vec[0] + (vec[2] - vec[0]) * alpha;
    tr::Vec2i B = half == thalf_e::LOWER ? vec[0] + (vec[1] - vec[0]) * beta
                                         : vec[1] + (vec[2] - vec[1]) * beta;
    if (A.x() > B.x()) std::swap(A, B);
    fill_x(A.x(), B.x(), y, image, color);
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
      image.set(vec.y(), vec.x(), color);
    else
      image.set(vec.x(), vec.y(), color);
  });
}

void tr::line(const Vec2i& t_0, const Vec2i& t_1, TGAImage& image,
              const TGAColor& color) {
  line(t_0.x(), t_0.y(), t_1.x(), t_1.y(), image, color);
}

void tr::triangle(const tr::Vec2i& t_0, const tr::Vec2i& t_1,
                  const tr::Vec2i& t_2, TGAImage& image,
                  const TGAColor& color) {
  auto sorted = sort_triangle({t_0, t_1, t_2});

  fill_triangle(sorted, thalf_e::LOWER, image, color);
  fill_triangle(sorted, thalf_e::HIGHER, image, color);
}

void tr::triangle_render_0(int width, int height, const TGAColor& color,
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

void tr::triangle_render_1(int width, int height, const TGAColor& color,
                           const std::string& tga_filename) {
  TGAImage image(width, height, TGAImage::RGB);
  std::array<Vec2i, 3> pts{Vec2i(10, 10), Vec2i(100, 30), Vec2i(190, 160)};
  triangle(pts, image, color);
  // image.flip_vertically();  // to place the origin in the bottom left corner
  // of
  //  the image
  image.write_tga_file(tga_filename);
}

void tr::line_render(int width, int height, const tr::Model& model,
                     const TGAColor& color, const std::string& tga_filename) {
  TGAImage image(width, height, TGAImage::RGB);
  for (size_t i{}; i < model.faces_size(); ++i) {
    std::vector<size_t> face = model.face(i);
    for (size_t j{}; j < 3; ++j) {
      tr::Vec3f v_0 = model.vert(face[j]);
      tr::Vec3f v_1 = model.vert(face[(j + 1) % 3]);
      int x_0 = static_cast<int>((v_0.x() + 1) * width / 2.0f);
      int y_0 = static_cast<int>((v_0.y() + 1) * height / 2.0f);
      int x_1 = static_cast<int>((v_1.x() + 1) * width / 2.0f);
      int y_1 = static_cast<int>((v_1.y() + 1) * height / 2.0f);
      tr::line(x_0, y_0, x_1, y_1, image, color);
    }
  }

  image.write_tga_file(tga_filename);
}

/// What we are trying to do here is to find an equation that satisfies this
/// P(w, u, v) = wA + uB + vC, we can aslo re-write this as
/// P(u, v) = (1 - u - v)A + uB + vC --> w = (1 - u - v) this is because
/// we need to constrain the scalar with w + u + v = 1.
/// A possible interpretation of this equation is that the point P is the
/// barycentric of the trinagle when we have three weights of value (1 - u - v),
/// u and v in the tre vertices of the triangle. What we are trying to find are
/// the barycentric coordinate of a generic point P. So we need to find the
/// scalar u, v that satisfy the equation (1 - u - v) + u + v = 1. Or in other
/// words we need to find a solution for the following euqtion
///
std::optional<tr::Vec3f> tr::barycentric(const std::array<tr::Vec2i, 3>& pts,
                                         const tr::Vec2i& P) {
  tr::Vec3f u =
      tr::Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0],
                pts[0][0] - P[0]) ^
      tr::Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]);

  /* `pts` and `P` have integer value as coordinates
   *  so `abs(u[2])` < 1 means `u[2]` is 0, that means
   *  triangle is degenerate
   */
  if (std::abs(u.z()) < 1) return std::nullopt;

  return tr::Vec3f(1.0f - (u.x() + u.y()) / u.z(), u.y() / u.z(),
                   u.z() / u.z());
}

/// Function for returning the minimum box that is inside the image dimension
/// and contains the trinagle
std::array<tr::Vec2i, 2> tr::get_bbox(const std::array<tr::Vec2i, 3>& pts,
                                      TGAImage& image) {
  std::array<tr::Vec2i, 2> bbox;
  bbox[0] = tr::Vec2i(image.width() - 1, image.height() - 1);
  bbox[1] = tr::Vec2i(0, 0);

  tr::Vec2i clamp(image.width() - 1, image.height() - 1);

  for (size_t i = 0; i < 3; i++) {
    bbox[0].coords.p.x = std::max(0, std::min(bbox[0].x(), pts[i].x()));
    bbox[0].coords.p.y = std::max(0, std::min(bbox[0].y(), pts[i].y()));
    bbox[1].coords.p.x = std::min(clamp.x(), std::max(bbox[1].x(), pts[i].x()));
    bbox[1].coords.p.y = std::min(clamp.y(), std::max(bbox[1].y(), pts[i].y()));
  }

  return bbox;
}

void tr::triangle(const std::array<Vec2i, 3>& pts, TGAImage& image,
                  const TGAColor& color) {
  auto bbox = get_bbox(pts, image);

  tr::Vec2i P;
  for (P.coords.p.x = bbox[0].x(); P.x() <= bbox[1].x(); ++P.coords.p.x) {
    for (P.coords.p.y = bbox[0].y(); P.y() <= bbox[1].y(); ++P.coords.p.y) {
      auto bc_screen = barycentric(pts, P);
      if (bc_screen && bc_screen->x() > 0 && bc_screen->y() > 0 &&
          bc_screen->z() > 0)
        image.set(P.x(), P.y(), color);
    }
  }
}

void tr::color_obj(TGAImage& image, const tr::Model& model) {
  for (size_t i{}; i < model.faces_size(); ++i) {
    std::vector<size_t> face = model.face(i);
    std::array<tr::Vec2i, 3> screen_coords;

    for (size_t j{}; j < 3; ++j) {
      Vec3f world_coords = model.vert(face[j]);
      screen_coords[j] = Vec2i(
          static_cast<int>((world_coords.x() + 1.0f) * image.width() / 2.0f),
          static_cast<int>((world_coords.y() + 1.0f) * image.height() / 2.0f));
    }

    tr::triangle(screen_coords[0], screen_coords[1], screen_coords[2], image,
                 TGAColor(static_cast<std::uint8_t>(rand() % 255),
                          static_cast<std::uint8_t>(rand() % 255),
                          static_cast<std::uint8_t>(rand() % 255), 255));
  }
}

void tr::light_mono_obj(TGAImage& image, const tr::Model& model) {
  Vec3f light_dir(0.0f, 0.0f, -1.0f);  // light direction just an example

  for (size_t i{}; i < model.faces_size(); ++i) {
    std::vector<size_t> face = model.face(i);
    std::array<tr::Vec2i, 3> screen_coords;
    std::array<tr::Vec3f, 3> world_coords;
    for (size_t j{}; j < 3; ++j) {
      Vec3f v = model.vert(face[j]);
      screen_coords[j] =
          Vec2i(static_cast<int>((v.x() + 1.0f) * image.width() / 2.0f),
                static_cast<int>((v.y() + 1.0f) * image.height() / 2.0f));
      world_coords[j] = v;
    }
    Vec3f normal_vector = (world_coords[2] - world_coords[0]) ^
                          (world_coords[1] - world_coords[0]);
    normal_vector.normalize();
    float intensity = normal_vector * light_dir;
    if (intensity > 0) {
      triangle(screen_coords[0], screen_coords[1], screen_coords[2], image,
               TGAColor(static_cast<std::uint8_t>(intensity * 255),
                        static_cast<std::uint8_t>(intensity * 255),
                        static_cast<std::uint8_t>(intensity * 255), 255));
    }
  }
}

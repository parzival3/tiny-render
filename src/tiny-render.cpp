#include "tiny-render.hpp"

#include <iostream>

#include "common.hpp"

static auto line_interpolator(int x_0, int y_0, int x_1, int y_1) {
  using namespace ranges;
  return views::for_each(views::iota(x_0, x_1), [=](int x) {
    double t = (x - x_0) / static_cast<double>(x_1 - x_0);
    int y = static_cast<int>(y_0 * (1.0 - t) + y_1 * t);
    return yield(tr::Vec2i(x, y));
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
std::optional<tr::Vec3f> tr::barycentric(const std::array<tr::Vec3f, 3>& pts,
                                         const tr::Vec3f& P) {
    std::array<Vec3f, 2> s;
    for (size_t i = 2; i--;) {
        s[i][0] = pts[2][i] - pts[0][i];
        s[i][1] = pts[1][i] - pts[0][i];
        s[i][2] = pts[0][i] - P[i];
    }
    // tr::Vec3f u =
    //     tr::Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]) ^
    //     tr::Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]);

    tr::Vec3f u = s[0] ^ s[1];

    /* `pts` and `P` have integer value as coordinates
     *  so `abs(u[2])` < 1 means `u[2]` is 0, that means
     *  triangle is degenerate
     */
    // std::cout << u << '\n';
    if (std::abs(u.z()) < 1) return std::nullopt;

    return tr::Vec3f(1.0f - (u.x() + u.y()) / u.z(), u.y() / u.z(), u.x() / u.z());
}

/// Function for returning the minimum box that is inside the image dimension
/// and contains the trinagle
std::array<tr::Vec2f, 2> tr::get_bbox(const std::array<tr::Vec3f, 3>& pts,
                                      TGAImage& image) {
    tr::Vec2f bboxmin(std::numeric_limits<float>::max(),
                      std::numeric_limits<float>::max());
    tr::Vec2f bboxmax(-std::numeric_limits<float>::max(),
                      -std::numeric_limits<float>::max());
    tr::Vec2f clamp(image.width() - 1, image.height() - 1);

    for (size_t i{}; i < 3; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }

    return {bboxmin, bboxmax};
}

void tr::draw_with_z_buffer(const std::array<Vec3f, 3>& pts, std::vector<int>& z_buffer,
                            tr::Vec3f& P, tr::Vec3f& screen, TGAImage& image,
                            const TGAColor& color) {
    P.coords.t.z = 0;
    for (size_t i{}; i < 3; ++i) {
        P.coords.t.z += pts[i][2] * screen[i];
    }

    auto z_buffer_index = static_cast<size_t>(P.x() + P.y() * image.width());

    if (z_buffer[z_buffer_index] < P.z()) {
        z_buffer[z_buffer_index] = static_cast<int>(P.z());
        image.set(static_cast<int>(P.x()), static_cast<int>(P.y()), color);
    }
}

void tr::triangle(const std::array<Vec3f, 3>& pts,
                  [[maybe_unused]] std::vector<int>& z_buffer, TGAImage& image,
                  const TGAColor& color) {
    auto bbox = get_bbox(pts, image);
    tr::Vec3f P;
    for (P.coords.t.x = bbox[0].x(); P.x() <= bbox[1].x(); ++P.coords.t.x) {
        for (P.coords.t.y = bbox[0].y(); P.y() <= bbox[1].y(); ++P.coords.t.y) {
            auto bc_screen = barycentric(pts, P);
            if (bc_screen && bc_screen->x() > 0 && bc_screen->y() > 0 &&
                bc_screen->z() > 0) {
                tr::draw_with_z_buffer(pts, z_buffer, P, *bc_screen, image, color);
            }
        }
    }
}

tr::Vec3f tr::normal_vector(std::array<tr::Vec3f, 3> world_coords) {
  return ((world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]))
      .normalize();
}

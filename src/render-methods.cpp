#include "render-methods.hpp"

#include <iostream>

#include "common.hpp"

using namespace tr;

void tr::light_mono_obj(TGAImage& image, const tr::Model& model) {
    Vec3f light_dir(0.0f, 0.0f, -1.0f);  // light direction just an example

    using namespace ranges;
    auto faces = model.faces();
    auto coords = faces | views::transform([&](const auto& n) {
                      return std::array<Vec3f, 3>{
                          model.vert(n[0]),
                          model.vert(n[1]),
                          model.vert(n[2]),
                      };
                  });

    std::vector<int> z_buffer(static_cast<size_t>(image.width() * image.height()),
                              std::numeric_limits<int>::min());

    ranges::for_each(coords, [&](const auto& world_coords) {
        auto screen_coords = tr::map_obj_to_screen<tr::Vec3f>(world_coords, image);
        // std::cout << screen_coords[0].to_string() << screen_coords[1].to_string()
        //           << screen_coords[2].to_string() << '\n';
        auto n = normal_vector(world_coords);

        auto intensity = static_cast<std::uint8_t>(n * light_dir * 255);

        if (intensity > 0)
            tr::triangle(screen_coords, z_buffer, image,
                         TGAColor(intensity, intensity, intensity, 255));
    });
}

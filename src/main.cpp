#include <iostream>
#include <optional>
#include <stdexcept>

#include "model.hpp"
#include "tga-lib/tgaimage.hpp"
#include "tiny-render.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//static constexpr int width_line{800};
//static constexpr int height_line{800};

static constexpr int width{200};
static constexpr int height{200};

int main(int argc, char** argv) {
  std::optional<tr::Model> model;
  if (argc == 2) {
    model = tr::Model(argv[1]);
  } else {
    model = tr::Model("obj/african_head.obj");
  }

  if (!model) {
    throw std::runtime_error("Error: couldn't load the image model");
  }

  std::string image_name_0{"output_0.tga"};
  std::string image_name_1{"output_1.tga"};
  std::string image_name_2{"output_2.tga"};
  std::string image_name_3{"output_3.tga"};

  // tr::line_render(width_line, height_line, *model, white, image_name);
  tr::triangle_render_0(width, height, white, image_name_0);
  std::cout << "Writing image " << image_name_0 << " ...\n";
  tr::triangle_render_1(width, height, white, image_name_1);
  std::cout << "Writing image " << image_name_1 << " ...\n";

  TGAImage image_2(800, 800, TGAImage::RGB);
  tr::color_obj(image_2, *model);
  std::cout << "Writing image " << image_name_2 << " ...\n";
  image_2.write_tga_file(image_name_2);

  TGAImage image_3(800, 800, TGAImage::RGB);
  tr::light_mono_obj(image_3, *model);
  std::cout << "Writing image " << image_name_3 << " ...\n";
  image_3.write_tga_file(image_name_3);
  return 0;
}

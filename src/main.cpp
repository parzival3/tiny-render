#include <iostream>
#include <optional>
#include <stdexcept>

#include "model.hpp"
#include "render-methods.hpp"
#include "tga-lib/tgaimage.hpp"
#include "tiny-render.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

static constexpr int width{800};
static constexpr int height{800};

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

  TGAImage image_0(width, height, TGAImage::RGB);
  tr::light_mono_obj(image_0, *model);
  std::cout << "Writing image " << image_name_0 << " ...\n";
  image_0.write_tga_file(image_name_0);
  return 0;
}

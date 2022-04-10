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

  std::string image_name{"output.tga"};

  // tr::line_render(width_line, height_line, *model, white, image_name);
  tr::triangle_render(width, height, white, image_name);
  std::cout << "Writing image " << image_name << " ...\n";
  return 0;
}

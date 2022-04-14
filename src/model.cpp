#include "model.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

tr::Model::Model(const std::string& filename) {
    std::ifstream f_stream;
    f_stream.open(filename, std::ifstream::in);

    if (f_stream.fail()) {
        throw std::runtime_error("Error: couldn't open the obj file");
    }

    std::string line;
    while (!f_stream.eof()) {
        std::getline(f_stream, line);
        std::istringstream iss(line.c_str());
        char unused;
        if (line.compare(0, 2, "v ") == 0) {
            iss >> unused;  // v is not used
            Vec3f v;
            for (size_t i{}; i < 3; ++i) iss >> v.coords.raw[i];
            verts_.push_back(v);
        } else if (line.compare(0, 2, "f ") == 0) {
            std::vector<size_t> face;
            size_t unused_index, index;
            iss >> unused;  // f is not used
            while (iss >> index >> unused >> unused_index >> unused >> unused_index) {
                --index;  // in wavefront obj all indices start at 1, not zero
                face.push_back(index);
            }
            faces_.push_back(face);
        }
    }
}

tr::Vec3f tr::Model::vert(size_t index) const { return verts_.at(index); }

std::vector<size_t> tr::Model::face(size_t index) const { return faces_.at(index); }

size_t tr::Model::verts_size() const { return verts_.size(); }

size_t tr::Model::faces_size() const { return faces_.size(); }

std::array<tr::Vec3f, 3> tr::Model::face_coords(size_t index) const {
    std::array<tr::Vec3f, 3> world_coords;
    for (size_t i{}; i < 3; ++i) world_coords[i] = vert(face(index)[i]);
    return world_coords;
}

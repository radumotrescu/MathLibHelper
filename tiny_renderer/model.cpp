#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

using namespace MathLib;

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v._data[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> faces;
            faces.reserve(4);
            iss >> trash;
            Vec3i tmp;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                tmp[0]--; // in wavefront obj all indices start at 1, not zero
                tmp[1]--; // in wavefront obj all indices start at 1, not zero
                tmp[2]--; // in wavefront obj all indices start at 1, not zero
                faces.push_back(tmp);
            }
            faces_.push_back(faces);
        }
        else if (!line.compare(0, 4, "vt  "))
        {
            auto vt = Vec2f();
            
            iss >> trash >> trash;
            for (auto i = 0; i < 2; i++)
            {
                iss >> vt._data[i];
            }
            vt_.push_back(vt);
        }
        else if (!line.compare(0, 4, "vn  "))
        {
            auto vn = Vec3f();
            iss >> trash >> trash;
            for (auto i = 0; i < 3; i++)
            {
                iss >> vn._data[i];
            }
            vn_.push_back(vn);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {
}

void Model::loadTexture(const char* filename)
{
    texture.read_tga_file(filename);
    texture.flip_vertically();
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    auto face = std::vector<int>();
    for (auto i = 0; i < faces_.size(); i++)
    {
        face.push_back(faces_[idx][i][0]);
    }
    return face;
}

TGAColor Model::color(Vec2i uv)
{
    return texture.get(uv[0], uv[1]);
}

Vec2f Model::uv(int faceIdx, int nvert)
{
    int faceVert = faces_[faceIdx][nvert][1];
    return Vec2f{ vt_[faceVert][0] * texture.get_width(), vt_[faceVert][1] * texture.get_height() };
}

Vec3f Model::vert(int i) {
    return verts_[i];
}


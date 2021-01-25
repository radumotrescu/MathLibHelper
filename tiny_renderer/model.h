#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <VecN.h>
#include "tgaimage.h"

class Model {
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    MathLib::Vec3f vert(int i);
    std::vector<int> face(int idx);
    void loadTexture(const char* filename);
    TGAColor color(MathLib::Vec2i uv);
    MathLib::Vec2f Model::uv(int faceIdx, int nvert);

private:
    std::vector<MathLib::Vec3f> verts_;
    std::vector<std::vector<MathLib::Vec3i>> faces_;
    std::vector<MathLib::Vec3f> vn_;
    std::vector<MathLib::Vec2f> vt_;
    TGAImage texture;
};

#endif //__MODEL_H__

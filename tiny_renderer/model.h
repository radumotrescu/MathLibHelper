#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <VecN.h>

class Model {
private:
    std::vector<MathLib::Vec3f> verts_;
    std::vector<std::vector<int> > faces_;
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    MathLib::Vec3f vert(int i);
    std::vector<int> face(int idx);
};

#endif //__MODEL_H__

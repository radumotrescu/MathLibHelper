#include "tgaimage.h"
#include "model.h"

using namespace MathLib;

namespace
{
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const TGAColor green = TGAColor(0, 255, 0, 255);
	const int width = 250;
	const int height = 250;
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

void bmw()
{
	TGAImage image(3000, 1000, TGAImage::RGB);

	auto model = Model("obj/bmw27_gpu.obj");
	for (auto i = 0; i < model.nfaces(); i++)
	{
		// each face has 3 lines
		auto face = model.face(i);
		if (face.empty())
			continue;
		for (auto j = 0; j < 3; j++)
		{
			// each line has 3 vertices 
			// what this code does is goes and fetches every line from the face
			// so line 1 and line 2, line 2 and line 3, line 3 and line 1 (0 based)
			Vec3f v0 = model.vert(face[j]);
			Vec3f v1 = model.vert(face[(j + 1) % 3]);

			/* 
			we do the (coord + 1) / (size / 2.) thing because the file
			uses coordinates from [-1, +1], thus the need
			to get rid of the negative sign
			so the coordinates would match [0, 2],
			thus the need to divide by the size/2
			so the coordinates would match [0, 1]
			*/
			int x0 = static_cast<int>((v0.m_data[0]+3.) * width);
			int y0 = static_cast<int>((v0.m_data[1]+1.) * width);

			int x1 = static_cast<int>((v1.m_data[0]+3.) *height);
			int y1 = static_cast<int>((v1.m_data[1]+1.)*height);
			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

void african_head()
{
	TGAImage image(800, 800, TGAImage::RGB);

	auto model = Model("obj/african_head.obj");
	for (auto i = 0; i < model.nfaces(); i++)
	{
		// each face has 3 lines
		auto face = model.face(i);
		if (face.empty())
			continue;
		for (auto j = 0; j < 3; j++)
		{
			// each line has 3 vertices 
			// what this code does is goes and fetches every line from the face
			// so line 1 and line 2, line 2 and line 3, line 3 and line 1 (0 based)
			Vec3f v0 = model.vert(face[j]);
			Vec3f v1 = model.vert(face[(j + 1) % 3]);

			/* 
			we do the (coord + 1) / (size / 2.) thing because the file
			uses coordinates from [-1, +1], thus the need
			to get rid of the negative sign
			so the coordinates would match [0, 2],
			thus the need to divide by the size/2
			so the coordinates would match [0, 1]
			*/
			int x0 = static_cast<int>((v0.m_data[0] + 1.)*width / 2.);
			int y0 = static_cast<int>((v0.m_data[1] + 1.)*height / 2.);

			int x1 = static_cast<int>((v1.m_data[0] + 1.)*width / 2.);
			int y1 = static_cast<int>((v1.m_data[1] + 1.)*height / 2.);
			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = std::abs(dy) * 2;
	int error = 0;

	int y = y0;
	int yIncr = y1 > y0 ? 1 : -1;
	for (auto x = x0; x <= x1; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);
		error += derror;
		if (error > dx)
		{
			y += yIncr;
			error -= dx * 2;
		}
	}
}

struct BoundingBox
{
	int minX, maxX, minY, maxY;
};
using BB = BoundingBox;

BB findBB(Vec2i p1, Vec2i p2, Vec2i p3)
{
	auto minMaxX = std::minmax({ p1.X(), p2.X(), p3.X() });
	auto minMaxY = std::minmax({ p1.Y(), p2.Y(), p3.Y() });
	auto minX = minMaxX.first;
	auto maxX = minMaxX.second;
	auto minY = minMaxY.first;
	auto maxY = minMaxY.second;
	return { minX, maxX, minY, maxY };
}

Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i P)
{
	/*
	Vec3f u = cross(
	Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]),
	Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]));
	*/

	auto u = Vec3f{ {0.+ p3.X() - p1.X(), 0.+ p2.X() - p1.X(), 0.+ p1.X() - P.X()} }.Cross(Vec3f{ 0.+ p3.Y() - p1.Y(),  0.+p2.Y() - p1.Y(),  0.+p1.Y() - P.Y() });
	if (u.Z() < 1)
		return { -1., 1., 1. };
	return Vec3f{ {1. - (u.X() + u.Y()) / u.Z(), static_cast<double>(u.Y()) / u.Z(), static_cast<double>(u.X()) / u.Z()} };
}

void triangle(Vec2i p1, Vec2i p2, Vec2i p3, TGAImage& image, const TGAColor& color)
{
	auto bb = findBB(p1, p2, p3);

	line(bb.minX, bb.minY, bb.maxX, bb.minY, image, white);
	line(bb.minX, bb.minY, bb.minX, bb.maxY, image, white);
	line(bb.maxX, bb.minY, bb.maxX, bb.maxY, image, white);
	line(bb.minX, bb.maxY, bb.maxX, bb.maxY, image, white);

	for (auto x = bb.minX; x < bb.maxX; x++)
	{
		for (auto y = bb.minY; y < bb.maxY; y++)
		{
			auto bc = barycentric(p1, p2, p3, { x, y });
			if (bc.X() < 0 || bc.Y() < 0 || bc.Z() < 0)
				continue;
			image.set(x, y, red);
		}
	}
}

void triangle_tests()
{
	TGAImage image(width, height, TGAImage::RGB);

	auto t1 = std::vector<Vec2i>{ Vec2i{10, 70},   Vec2i{50, 160},  Vec2i{70, 80} };
	//auto t2 = std::vector<Vec2i>{ Vec2i{180, 50},  Vec2i{150, 1},   Vec2i{70, 180} };
	//auto t3 = std::vector<Vec2i>{ Vec2i{180, 150}, Vec2i{120, 160}, Vec2i{130, 180} };

	//triangle(t0[0], t0[1], t0[2], image, white);
	triangle(t1[0], t1[1], t1[2], image, white);
	//triangle(t1[0], t1[1], t1[2], image, white);
	//triangle(t2[0], t2[1], t2[2], image, white);

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

int main()
{
	triangle_tests();
	return 0;
}
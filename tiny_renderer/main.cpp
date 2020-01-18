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

void triangle(Vec2i p1, Vec2i p2, Vec2i p3, TGAImage& image, const TGAColor& color)
{
	if (p1.Y() > p2.Y()) std::swap(p1, p2);
	if (p1.Y() > p3.Y()) std::swap(p1, p3);
	if (p2.Y() > p3.Y()) std::swap(p2, p3);
	line(p1.X(), p1.Y(), p2.X(), p2.Y(), image, green);
	line(p2.X(), p2.Y(), p3.X(), p3.Y(), image, green);
	line(p3.X(), p3.Y(), p1.X(), p1.Y(), image, red);

	auto minMaxX = std::minmax({ p1.X(), p2.X(), p3.X() });
	auto minMaxY = std::minmax({ p1.Y(), p2.Y(), p3.Y() });
	auto minX = minMaxX.first;
	auto maxX = minMaxX.second;
	auto minY = minMaxY.first;
	auto maxY = minMaxY.second;
	auto diffX = maxX - minX;
	auto diffY = maxY - minY;

	line(minX, minY, minX + diffX, minY, image, white);
	line(minX, minY, minX, minY + diffY, image, white);
	line(minX+diffX, minY, minX + diffX, minY + diffY, image, white);
	line(minX, minY + diffY, minX + diffX, minY + diffY, image, white);

	for (auto x = minX; x < minX + diffX; x++)
	{
		for (auto y = minY; y < minY + diffY; y++)
		{
		}
	}
}

void triangle_tests()
{
	TGAImage image(width, height, TGAImage::RGB);

	auto t1 = std::vector<Vec2i>{ Vec2i{10, 70},   Vec2i{50, 160},  Vec2i{70, 80} };
	auto t2 = std::vector<Vec2i>{ Vec2i{180, 50},  Vec2i{150, 1},   Vec2i{70, 180} };
	auto t3 = std::vector<Vec2i>{ Vec2i{180, 150}, Vec2i{120, 160}, Vec2i{130, 180} };

	//triangle(t0[0], t0[1], t0[2], image, white);
	triangle(t1[0], t1[1], t1[2], image, white);
	//triangle(t1[0], t1[1], t1[2], image, white);
	//triangle(t2[0], t2[1], t2[2], image, white);

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
}

int main()
{
	bmw();
	return 0;
}
#include "tgaimage.h"
#include "model.h"

using namespace MathLib;

namespace
{
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const int width = 800;
	const int height = 800;
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

int main()
{
	TGAImage image(width, height, TGAImage::RGB);
	//line(13, 20, 80, 40, image, white);
	//line(20, 13, 40, 80, image, red);
	//line(80, 40, 13, 20, image, red);


	auto model = Model("obj/african_head.obj");
	for (auto i = 0; i < model.nfaces(); i++)
	{
		auto face = model.face(i);
		for (auto j = 0; j < 3; j++)
		{
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
	return 0;
}
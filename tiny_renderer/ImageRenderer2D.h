#ifndef ImageRenderer2D_h_include
#define ImageRenderer2D_h_include

#include "tgaimage.h"
#include "Entities.h"

#include <string>

class ImageRenderer2D
{
public:
	ImageRenderer2D(const uint32_t width, const uint32_t height)
		:m_image(TGAImage(width, height, TGAImage::RGB))
	{
	}

	ImageRenderer2D() = delete;
	ImageRenderer2D(const ImageRenderer2D& other) = default;
	ImageRenderer2D(ImageRenderer2D&& other) = default;
	
	void ExportImage(std::string path)
	{
		m_image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
		m_image.write_tga_file(path.append(".tga").c_str());
	}

private:
	TGAImage m_image;
	uint32_t width;
	uint32_t height;
};

#endif
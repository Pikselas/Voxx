#pragma once
#include<memory>
#include<filesystem>
#include<type_traits>
#include"GDIPlusManager.h"

struct ColorType
{
	unsigned char b = 255;
	unsigned char g = 255;
	unsigned char r = 255;
	unsigned char a = 255;
};

class Image
{
private:
	GDIPlusManager manager;
	std::unique_ptr<Gdiplus::Bitmap> bitmap;
public:
	Image(const std::filesystem::path& file);
	Image(unsigned int width, unsigned height);
	Image(const Image& img);
	unsigned int GetHeight() const;
	unsigned int GetWidth() const;
	ColorType GetPixel(unsigned int x , unsigned int y) const;
	void SetPixel(unsigned int x, unsigned int y, ColorType color);
	void DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ColorType color);
	void Clear(ColorType color = {});
	const ColorType* Raw() const;
public:
	Image& operator=(const Image& img);
};
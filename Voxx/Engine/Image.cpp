#include "Image.h"

GDIPlusManager Image::manager;

Image::Image(const std::filesystem::path& file) : bitmap(std::make_unique<Gdiplus::Bitmap>(file.c_str()))
{
	if (bitmap->GetLastStatus() != Gdiplus::Status::Ok)
	{
		throw std::runtime_error("Failed to load image");
	}
}

Image::Image(unsigned int width,unsigned int height) : bitmap(std::make_unique<Gdiplus::Bitmap>(width,height,PixelFormat32bppARGB))
{}

Image::Image(const Image& img) : Image(img.GetWidth(), img.GetHeight())
{
	*this = img;
}

unsigned int Image::GetHeight() const
{
	return bitmap->GetHeight();
}

unsigned int Image::GetWidth() const 
{
    return bitmap->GetWidth();
}

ColorType Image::GetPixel(unsigned int x, unsigned int y) const
{
	Gdiplus::Color c;
	auto bitmapPtr = const_cast<Gdiplus::Bitmap*>(bitmap.get());
	if (bitmapPtr->GetPixel(x, y, &c) != Gdiplus::Ok)
	{
		return { 0,0,0,0 };
	}
	return ColorType{ c.GetBlue(),c.GetGreen(),c.GetRed(),c.GetAlpha()};
	//return static_cast<ColorType>(c.GetValue());
}

void Image::SetPixel(unsigned int x, unsigned int y, ColorType color)
{
	bitmap->SetPixel(x, y, Gdiplus::Color(color.a, color.r, color.g, color.b));
}

void Image::DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ColorType color)
{
	Gdiplus::Graphics graphics(bitmap.get());
	Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b));
	graphics.DrawLine(&pen, (int)x1, (int)y1, (int)x2, (int)y2);
}

void Image::Clear(ColorType color)
{
	Gdiplus::Graphics graphics(bitmap.get());
	graphics.Clear(Gdiplus::Color(color.a,color.r,color.g,color.b));
}

ColorType* Image::Raw()
{
	Gdiplus::BitmapData data;
	auto bitmapPtr = bitmap.get();
	Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	bitmapPtr->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB , &data);
	bitmapPtr->UnlockBits(&data);
	return static_cast<ColorType*>(data.Scan0);
}

const ColorType* Image::Raw() const
{
	Gdiplus::BitmapData data;
	auto bitmapPtr = bitmap.get();
	Gdiplus::Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	bitmapPtr->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
	bitmapPtr->UnlockBits(&data);
	return static_cast<ColorType*>(data.Scan0);
}

Image& Image::operator=(const Image& img)
{
	bitmap = std::make_unique<Gdiplus::Bitmap>(img.GetWidth(), img.GetHeight(), PixelFormat32bppARGB);
	Gdiplus::Graphics graphics(bitmap.get());
	graphics.DrawImage(img.bitmap.get(), 0, 0, img.GetWidth(), img.GetHeight());
	return *this;
}

void Image::Save(const std::filesystem::path& file) const
{
	CLSID Clsid;
	HRESULT hResult;
	
	if (auto ext = file.extension(); ext == ".png")
	{
		hResult = CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &Clsid);
	}
	else if (ext == ".jpg" || ext == ".jpeg")
	{
		hResult = CLSIDFromString(L"{557CF401-1A04-11D3-9A73-0000F81EF32E}", &Clsid);
	}
	else if (ext == ".bmp")
	{
		hResult = CLSIDFromString(L"{557CF400-1A04-11D3-9A73-0000F81EF32E}", &Clsid);
	}
	else if (ext == ".tif")
	{
		hResult = CLSIDFromString(L"{557CF405-1A04-11D3-9A73-0000F81EF32E}", &Clsid);
	}
	else
	{
		throw std::runtime_error("Invalid file extension");
	}

	if (FAILED(hResult))
	{
		throw std::runtime_error("Failed to get Encoder");
	}

	auto status = bitmap->Save(file.wstring().c_str(),&Clsid);

	if (status != Gdiplus::Status::Ok)
	{
		throw std::runtime_error("Failed to save image");
	}
}

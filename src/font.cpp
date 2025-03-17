#include <font.hpp>
using namespace sglf;
using namespace glm;

#include "util.hpp"

Gdiplus::GdiplusStartupInput *Font::gdiplusStartupInput;
ULONG_PTR Font::gdiplusToken;
Gdiplus::PrivateFontCollection *Font::collection;

void Font::initialize(const char *fonts[])
{
	Font::gdiplusStartupInput = new Gdiplus::GdiplusStartupInput{};
	Gdiplus::GdiplusStartup(&gdiplusToken, gdiplusStartupInput, nullptr);
	Font::collection = nullptr;
	
	// Load font faces at Font::collection
	if (fonts)
	{
		Font::collection = new Gdiplus::PrivateFontCollection();
		for (unsigned int i = 0; fonts[i][0] != ' '; i++)
		{
			wchar_t *w_font = toWideString(fonts[i]);
			Font::collection->AddFontFile(w_font);
			delete[] w_font;
		}
	}
}

void Font::finalize()
{
	//& GDI
	delete Font::collection;
	Gdiplus::GdiplusShutdown(Font::gdiplusToken);
	delete Font::gdiplusStartupInput;
}

Font::Font(const char *familyName, float size, Style style, bool custom)
: size(size), style(style), custom(custom)
{
	this->familyName = new char[std::strlen(familyName) + 1];
	std::strcpy(this->familyName, familyName);
	wchar_t *w_familyName = toWideString(familyName);
	gdiFont = new Gdiplus::Font(w_familyName, size, style, Gdiplus::Unit::UnitPixel, (custom ? Font::collection : nullptr));
	
	delete[] w_familyName;
}

Font::~Font()
{
	delete[] familyName;
	delete gdiFont;
}
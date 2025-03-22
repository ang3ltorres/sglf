#pragma once
#include <pch.hpp>

namespace sglf
{
	struct Font
	{
	
		enum Style
		{
			Regular = Gdiplus::FontStyle::FontStyleRegular,
			Bold    = Gdiplus::FontStyle::FontStyleBold,
			Italic  = Gdiplus::FontStyle::FontStyleItalic,
		};
	
		Font(const char *familyName, float size = 16, Style style = Style::Regular, bool custom = false);
		Font(const Font&) = delete;
		~Font();
	
		static void initialize(const char *fonts[]);
		static void finalize();
	
		static Gdiplus::GdiplusStartupInput *gdiplusStartupInput;
		static ULONG_PTR gdiplusToken;
		static Gdiplus::PrivateFontCollection *collection;
	
		char *familyName;
		float size;
		Style style;
		bool custom;
		Gdiplus::Font *gdiFont;
	};
}

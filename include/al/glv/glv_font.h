#ifndef INC_GLV_FONT_H
#define INC_GLV_FONT_H

/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include "al/core/graphics/al_Mesh.hpp"

namespace glv{

/// Font
class Font{
public:
	Font(float size=8);
	
	virtual ~Font();

	/// Get advance amount (width) of character
	virtual float advance(const char c) const;

	/// Returns total advance width of text string
	virtual float advance(const char *text) const;

	/// Get bounding box of text string
	virtual void getBounds(float& w, float& h, const char * text) const;

	/// Render text string
	virtual void render(const char * text, float x=0, float y=0, float z=0) const;

	virtual void render(al::Mesh& g, const char * text, float x=0, float y=0, float z=0) const;

	/// Set spacing, in ems, between the left and right edges of successive letters
	Font& letterSpacing(float v);

	/// Set spacing, in ems, between lines
	Font& lineSpacing(float v);

	/// Set the font size in pixels
	Font& size(float size);
	
	/// Set number of spaces per tab
	Font& tabSpaces(unsigned spaces);

	float baseline() const;		///< Get absolute position on which glyphs rest
	float cap() const;			///< Get distance from baseline to top of uppercase glyphs
	float xheight() const;		///< Get distance from baseline to top of lowercase glyphs
	float descent() const;		///< Get distance from baseline to lowest position of glyphs

	/// Get letter spacing, in ems
	float letterSpacing() const { return mLetterSpacing; }
	
	/// Get line spacing, in ems
	float lineSpacing() const { return mLineSpacing; }
	
	/// Get scaling factor in x direction
	float scaleX() const { return mScaleX; }
	
	/// Get font size, in pixels
	float size() const { return mSize; }
	
	/// Get number of spaces per tab
	unsigned tabSpaces() const { return mTabSpaces; }

private:
	float mSize;
	float mScaleX, mScaleY;
	float mLetterSpacing;
	float mLineSpacing;
	unsigned mTabSpaces;
};

struct Glyph{
	enum{ Once=(1<<7), Dot1=(1<<5), Dot2=(2<<5), MaskDots=96, MaskSize=31 };
	
	const unsigned char field;
	float x[8], y[8];
	
	static float cap()		{ return 0; }
	static float median()	{ return 3; }
	static float baseline()	{ return 8; }
	static float descent()	{ return 11; }
	static float width()	{ return 8; }
	
	unsigned char dots() const { return (field & MaskDots) >> 5; }
	unsigned char once() const { return (field           ) >> 7; }
	unsigned char size() const { return (field & MaskSize)     ; }
};

struct TextIterator{

	TextIterator(const Font& f_, const char *& s_): x(0),y(0),l(0),t(0),w(0),h(0), s(s_), f(f_){}

	virtual ~TextIterator(){}

	void run(){
		while((*this)()){}
	}

	bool operator()();

	virtual bool onPrintable(char c){ return true; }

	float x,y;			// current raster position
	float l,t,w,h;		// bounding box of current character
	const char *& s;
	const Font& f;
};

bool addCharacter(al::Mesh& g, int c, float dx, float dy, float sx, float sy);

// font rendering styles
//enum
//{
//	GLV_FONT_BITMAP = 0,
//	GLV_FONT_PIXMAP,
//	GLV_FONT_TEXTURE,
//	GLV_FONT_POLYGON,
//	GLV_FONT_OUTLINE,
//	GLV_FONT_EXTRUDE
//};

//namespace FontStyle{
//	enum t{
//		Regular,
//		Bold,
//		Italic
//	};
//}


/*
A font is a particular style of a typeface (roman, boldface, italic)
A typeface is a set of character glyphs with consistent visual appearance

Metrics
Font-wide:
	cap height
	x-height
	ascender height
	descender depth
	bounding box

Glyph:
	bounding box
	advance width (total space for glyph)
	sidebearings (padding on either side)
*/

/*
// From an older version of GLV...
class Font{
public:
	Font(char * typeface, Color * color = 0, unsigned size = 0, int fontStyle = GLV_FONT_BITMAP);
	Font(Color * color, unsigned size = 0, int fontStyle = GLV_FONT_BITMAP);
	Font(unsigned size, int fontStyle);
	Font(int fontStyle);

	Font(unsigned size=8);
	
	~Font(){}
	
	void render(float x, float y, float z, const char * text);
	
	float advance(const char *text);
	float ascender();
	float descender();

	void size(unsigned size);
	unsigned int size() const;

private:

	// function pointer to a void(float, float, char *) method of class Font
	typedef void (Font::* fpFontDraw)(float, float, float, const char *);

	void init(char * fontFace = 0, Color * color = 0, unsigned newSize = 0, int fontStyle = GLV_FONT_BITMAP);

	FTFont * face;
 	Color *	mColor;
	fpFontDraw draw;

	void drawRaster(float x, float y, float z, const char * text);
	void drawVector(float x, float y, float z, const char * text);

	static map<char *, FTGLBitmapFont *>	bitmapfonts;		// Vector of available Fonts.  Made on demand.
	static map<char *, FTGLTextureFont *>	texturefonts;		// Vector of available Fonts.  Made on demand.
	static map<char *, FTGLOutlineFont *>	outlinefonts;		// Vector of available Fonts.  Made on demand.
	static map<char *, FTGLPixmapFont *>	pixmapfonts;		// Vector of available Fonts.  Made on demand.
	static map<char *, FTGLPolygonFont *>	polygonfonts;		// Vector of available Fonts.  Made on demand.
};
*/

} // glv::

#endif

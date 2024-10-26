#pragma once

#define ISFLAG(x, flag)     ( (x & flag) != 0 )

struct ColorRGBA
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct ColorRGB
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct Rect
{
	uint32_t  x1; // left
	uint32_t  y1; // top
	uint32_t  x2; // right
	uint32_t  y2; // bottom
};
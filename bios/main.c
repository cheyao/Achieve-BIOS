#include <stdint.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1024 
#define SCREEN_HEIGHT 768

static inline void outw(uint16_t port, uint16_t val) {
    *((uint16_t *) (0x800000000000ULL + port)) = val;
}

#define rgb(r, g, b) ((uint16_t)(((r) & 0x1F) << 11) + (((g) & 0x3F) << 5) + ((b) & 0x1F))
static inline void put_pixel(uint16_t x, uint16_t y, uint16_t color) {
        outw(x + y * SCREEN_WIDTH, color);
}

void line(uint16_t p0x, uint16_t p0y, uint16_t p1x, uint16_t p1y, const uint16_t color) {
	bool yLonger = false;
	int32_t shortLen = (int32_t) (p1y - p0y);
	int32_t longLen = (int32_t) (p1x - p0x);

	if (shortLen > longLen) {
		int swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}
	int decInc;
	if (longLen == 0)
		decInc = 0;
	else
		decInc = (shortLen << 16) / longLen;

	if (yLonger) {
		if (longLen > 0) {
			longLen += (int32_t) p0y;
			for (int j = (int32_t) (0x8000 + (p0x << 16)); (int32_t) p0y <= longLen; ++p0y) {
				put_pixel(j >> 16, p0y, color);
				j += decInc;
			}
			return;
		}
		longLen += (int32_t) p0y;
		for (int j = (int32_t) (0x8000 + (p0x << 16)); (int32_t) p0y >= longLen; --p0y) {
			put_pixel(j >> 16, p0y, color);
			j -= decInc;
		}
		return;
	}

	if (longLen > 0) {
		longLen += (int32_t) p0x;
		for (int j = (int32_t) (0x8000 + (p0y << 16)); (int32_t) p0x <= longLen; ++p0x) {
			put_pixel(p0x, j >> 16, color);
			j += decInc;
		}
		return;
	}
	longLen += (int32_t) p0x;
	for (int j = (int32_t) (0x8000 + (p0y << 16)); (int32_t) p0x >= longLen; --p0x) {
		put_pixel(p0x, j >> 16, color);
		j -= decInc; 
	}
}

void main(void) {
    // line(5, 5, 5, 20, 0xFFFF);
    put_pixel(5, 5, 0xFFFF);
}

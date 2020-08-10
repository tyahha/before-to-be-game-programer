#include "GameLib/Framework.h"

int pos = 0;
int posAdd = 13;
unsigned color = 0;

namespace GameLib {
	void Framework::update() {
		unsigned* vram = videoMemory();

		int w = width();
		int h = height();
		int max = w * h - 1;

		pos += posAdd;
		color += 55;

		if (pos > max) {
			pos -= max;
		}

		if (color > 0x00ffffff) {
			color -= 0x00ffffff;
		}

		vram[pos] = color;
	}
}
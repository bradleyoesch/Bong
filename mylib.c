/* BRADLEY OESCH
 * 10/28/13
 */

#include "mylib.h"
#include "dma.h"

u16 *videoBuffer = (u16*)0x6000000;

/* Functions */
void drawRect(int r, int c, int width, int height, u16 color) {
	for (int row=0; row<height; row++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(r+row, c, W)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void fillScreen(u16 color) { //or u16 *img
	DMA[3].src = &color; // = img, assuming imgSize=videoBufferSize
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = W | DMA_ON | DMA_SOURCE_FIXED;
}

void waitForVblank() {
    while(SCANLINECOUNTER > H);
    while(SCANLINECOUNTER < H);
}
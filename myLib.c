/* BRADLEY OESCH
 * 10/28/13
 */

#include "myLib.h"
#include "dma.h"

u16 *videoBuffer = (u16*)0x6000000;

/* Functions */
BALL resetBall(BALL ball) {
	ball.r = H / 2 - ball.s / 2;
	ball.c = W / 2 - ball.s / 2;
	return ball;
}
void drawRect(int r, int c, int width, int height, u16 color) {
	for (int row=0; row<height; row++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(r+row, c, W)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void setPixel(int r, int c, u16 color) {
	videoBuffer[OFFSET(r, c, W)] = color;
}

void fillScreen(u16 color) { //or u16 *img
	DMA[3].src = &color; // = img, assuming imgSize=videoBufferSize
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = W | DMA_ON | DMA_SOURCE_FIXED;
}

//only called when ball is on correct column
int hitsPaddle(BALL ball, PADDLE paddle) {
	int rangeLow = paddle.r - ball.s;
	int rangeHigh = paddle.r + paddle.h + ball.s;
	if (ball.r >= rangeLow && ball.r <= rangeHigh)
		return -1;
	else
		return 1;
}

int checkScoreCondition(BALL ball) {
	if (ball.c <= ball.leftBound)
		return -1;
	else if (ball.c >= ball.rightBound)
		return 1;
	else
		return 0;
}

void waitForVblank() {
    while(SCANLINECOUNTER > H);
    while(SCANLINECOUNTER < H);
}
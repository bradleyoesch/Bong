/* BRADLEY OESCH
 * 10/23/13
 */

#include "myLib.h"
#include "dma.h"
#include "text.h"
#include "play.h"
#include "win.h"
#include "lose.h"
#include "coin.h"
#include <stdio.h>

void delay(int n);
void newGame();

u16 bgColor = BLACK;
int scoreCondition;
char buffer[41];
BALL ball;
PADDLE user;
PADDLE comp;

int main(void) 
{
	REG_DISPCNT = MODE3 | BG_ENABLE;

	// drawRect(0,0,W,H,bgColor);
	// sprintf(buffer, "PRESS ENTER TO PLAY!");
	// drawString(H / 2 - 4, W/2-60, buffer, WHITE);
	// drawPicture(play);
	// fillPicture(play);
	DMA[3].src = play;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (W*H) | DMA_ON;
	while (! KEY_DOWN_NOW(BUTTON_START));

	newGame();

	/* TODO */
	// add coin
	while(1) 
	{
		/*for (int row=0; row<COIN_HEIGHT; row++) {
			DMA[3].src = coin;
			DMA[3].dst = &videoBuffer[OFFSET(row+2, 1000, 5)];
			DMA[3].cnt = COIN_WIDTH | DMA_ON | DMA_DESTINATION_INCREMENT;
		}*/

		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			newGame();
		}

		scoreCondition = checkScoreCondition(ball);
		if (scoreCondition != 0) {
			if (scoreCondition == 1) user.score++;
			if (scoreCondition == -1) comp.score++;
			drawRect(ball.r, ball.c, ball.s, ball.s, bgColor); //draw over old position
			ball = resetBall(ball);
			drawRect(ball.r, ball.c, ball.s, ball.s, WHITE);
			drawRect(comp.r, comp.c, comp.w, comp.h, bgColor); //draw over old position
			comp.r = (ball.s / 2) + ball.r - (comp.h / 2);
			comp = realignPaddle(comp);
			drawRect(comp.r, comp.c, comp.w, comp.h, comp.color); //draw new computer paddle

			drawRect(0, 0, W, TOP_MARGIN, bgColor); //draw over score
			sprintf(buffer, "%d      %d", user.score, comp.score);
			drawString(2, W/2-24, buffer, WHITE);
			if (user.score >= 5) {
				// drawRect(TOP_MARGIN,0,W,H,bgColor);
				// sprintf(buffer, "YOU WIN!");
				// drawString(H / 2 - 4, W / 2-24, buffer, YELLOW);
				// fillPicture(win);
				DMA[3].src = win;
				DMA[3].dst = videoBuffer;
				DMA[3].cnt = (W*H) | DMA_ON;
				while (! KEY_DOWN_NOW(BUTTON_SELECT));
				newGame();
			}
			if (comp.score >= 5) {
				// drawRect(TOP_MARGIN,0,W,H,bgColor);
				// sprintf(buffer, "YOU LOSE");
				// drawString(H / 2 - 4, W / 2-24, buffer, RED);
				// fillPicture(lose);
				DMA[3].src = lose;
				DMA[3].dst = videoBuffer;
				DMA[3].cnt = (W*H) | DMA_ON;
				while (! KEY_DOWN_NOW(BUTTON_SELECT));
				newGame();
			}
			delay(25);
		}

		// sets old r/c to curr r/c to save it somewhere when we keydown
		ball.rOld = ball.r;
		ball.cOld = ball.c;
		user.rOld = user.r;
		user.cOld = user.c;
		comp.rOld = comp.r;
		comp.cOld = comp.c;

		if(KEY_DOWN_NOW(BUTTON_UP)) {
			user.r -= user.rD; //decrement because 0,0 is top left
			if (user.r <= user.topBound)
				user.r = user.topBound;
		}

		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			user.r += user.rD; //increment because 0,0 is top left
			if (user.r >= user.botBound)
				user.r = user.botBound;
		}

		waitForVblank();

		ball.r += ball.rD;
		ball.c += ball.cD;
		if (ball.r >= ball.botBound || ball.r <= ball.topBound)
			ball.rD *= -1;
		if (ball.c == user.c+user.w)
			ball.cD *= hitsPaddle(ball, user);
		if (ball.c == comp.c-ball.s+1)
			ball.cD *= hitsPaddle(ball, comp);
		drawRect(ball.rOld, ball.cOld, ball.s, ball.s, bgColor); //draw over old position
		drawRect(ball.r, ball.c, ball.s, ball.s, ball.color); //draw new ball

		drawRect(user.rOld, user.cOld, user.w, user.h, bgColor); //draw over old position
		drawRect(user.r, user.c, user.w, user.h, user.color); //draw new user paddle

		int chance = getRandomNumber(0,10);
		int ballDirection = ball.r - ball.rOld; //if -1, moving down, if 1, moving up
		comp.rD = (chance == 0) ? ballDirection * -1 : ballDirection;
		comp.r += comp.rD;
		// comp.r = (ball.s / 2) + ball.r - (comp.h / 2); //always puts the computer paddle directly centered on ball. you can't win.
		comp = realignPaddle(comp);
		drawRect(comp.rOld, comp.cOld, comp.w, comp.h, bgColor); //draw over old position
		drawRect(comp.r, comp.c, comp.w, comp.h, comp.color); //draw new computer paddle


	} // While loop
	return 0;
}

/**
 * Restarts or starts game fresh.
 */
void newGame() {
	drawRect(0,0,W,H,bgColor);

	ball.s = 5;
	ball.r = H / 2 - ball.s / 2;
	ball.rOld = ball.r;
	ball.rD = 2;
	ball.c = W / 2 - ball.s / 2;
	ball.cOld = ball.c;
	ball.cD = -2;
	ball.color = WHITE;
	ball.leftBound = 0 - ball.s;
	ball.rightBound = W;
	ball.topBound = 0 + TOP_MARGIN;
	ball.botBound = H - ball.s - MARGIN;
	ball = resetBall(ball);

	user.w = 5;
	user.h = 20;
	user.r = H / 2 - user.h / 2;
	user.rOld = user.r;
	user.rD = 2;
	user.c = MARGIN;
	user.cOld = user.c;
	user.color = WHITE;
	user.topBound = 0 + TOP_MARGIN;
	user.botBound = H - user.h - MARGIN;
	user.score = 0;

	comp.w = 5;
	comp.h = 20;
	comp.r = (ball.s / 2) + ball.r - (comp.h / 2);
	comp.rOld = comp.r;
	comp.rD = 2;
	comp.c = W - comp.w - MARGIN;
	comp.cOld = comp.c;
	comp.color = WHITE;
	comp.topBound = 0 + TOP_MARGIN;
	comp.botBound = H - user.h - MARGIN;
	comp.score = 0;
	comp = realignPaddle(comp);
	
	scoreCondition = 0;
	sprintf(buffer, "%d      %d", user.score, comp.score);
	drawString(2, W / 2 - 24, buffer, WHITE);
	drawRect(user.r, user.c, user.w, user.h, user.color);
	drawRect(comp.r, comp.c, comp.w, comp.h, comp.color);
	drawRect(ball.r, ball.c, ball.s, ball.s, ball.color);
	delay(25);
}

void delay(int n)
{
	int i = 0;
	volatile int x=0;
	for(i=0; i<n*10000; i++) {
		x++;
	}
}
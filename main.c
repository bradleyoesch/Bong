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
int margin = 5;
int topMargin = 11;
int scoreCondition;
char buffer[41];
PADDLE user;
PADDLE comp;
BALL ball;

int main(void) 
{
	REG_DISPCNT = MODE3 | BG_ENABLE;

	// fillPicture(&play);
	drawRect(0,0,W,H,bgColor);
	sprintf(buffer, "PRESS ENTER TO PLAY!");
	drawString(H / 2 - 4, W/2-60, buffer, WHITE);
	while (! KEY_DOWN_NOW(BUTTON_START));

	newGame();

	/* TODO */
	// add pauses in certain places
	// https://www.google.com/search?q=generate+random+integers+in+c
	while(1) 
	{
		scoreCondition = checkScoreCondition(ball);
		if (scoreCondition != 0) {
			if (scoreCondition == 1) user.score++;
			if (scoreCondition == -1) comp.score++;
			drawRect(ball.r, ball.c, ball.s, ball.s, bgColor); //draw over old position
			ball = resetBall(ball);
			drawRect(0, 0, W, topMargin, bgColor); //draw over score
			sprintf(buffer, "%d      %d", user.score, comp.score);
			drawString(2, W/2-24, buffer, WHITE);
			if (user.score >= 5) {
				drawRect(topMargin,0,W,H,bgColor);
				sprintf(buffer, "YOU WIN!");
				drawString(H / 2 - 4, W/2-24, buffer, YELLOW);
				// fillPicture(&win);
				while (! KEY_DOWN_NOW(BUTTON_SELECT));
			}
			if (comp.score >= 5) {
				drawRect(topMargin,0,W,H,bgColor);
				sprintf(buffer, "YOU LOSE.");
				drawString(H / 2 - 4, W/2-24, buffer, RED);
				// fillPicture(&lose);
				while (! KEY_DOWN_NOW(BUTTON_SELECT));
			}
			//wait like 2 seconds
		}

		// sets old r/c to curr r/c to save it somewhere when we keydown
		ball.rOld = ball.r;
		ball.cOld = ball.c;
		user.rOld = user.r;
		user.cOld = user.c;
		comp.rOld = comp.r;
		comp.cOld = comp.c;

		if(KEY_DOWN_NOW(BUTTON_UP)) {
			user.r--; //decrement because 0,0 is top left
			if (user.r <= user.topBound)
				user.r = user.topBound;
		}

		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			user.r++; //increment because 0,0 is top left
			if (user.r >= user.botBound)
				user.r = user.botBound;
		}

		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			newGame();
		}

		waitForVblank();

		ball.r += ball.rD;
		ball.c += ball.cD;
		if (ball.r >= ball.botBound || ball.r <= ball.topBound)
			ball.rD *= -1;
		if (ball.c == user.c+user.w)
			ball.cD *= hitsPaddle(ball, user);
		if (ball.c == comp.c-ball.s)
			ball.cD *= hitsPaddle(ball, comp);
		drawRect(ball.rOld, ball.cOld, ball.s, ball.s, bgColor); //draw over old position
		// if (ball.c < 0) {
		// 	drawRect(ball.r, 0, ball.s+ball.c, ball.s, ball.color); //draw new ball
		// } else {
			drawRect(ball.r, ball.c, ball.s, ball.s, ball.color); //draw new ball
		// }

		drawRect(user.rOld, user.cOld, user.w, user.h, bgColor); //draw over old position
		drawRect(user.r, user.c, user.w, user.h, user.color); //draw new user paddle

		comp.r = (ball.s / 2) + ball.r - (comp.h / 2); //always puts the computer paddle directly centered on ball. you can't win.
		if (comp.r <= comp.topBound)
			comp.r = comp.topBound;
		if (comp.r >= comp.botBound)
			comp.r = comp.botBound;
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

	user.w = 5;
	user.h = 20;
	user.r = H / 2 - user.h / 2;
	user.rOld = user.r;
	user.rD = 1;
	user.c = margin;
	user.cOld = user.c;
	user.color = WHITE;
	user.topBound = 0 + topMargin;
	user.botBound = H - user.h - margin;
	user.score = 0;

	comp.w = 5;
	comp.h = 20;
	comp.r = H / 2 - comp.h / 2;
	comp.rOld = comp.r;
	comp.rD = 1;
	comp.c = W - comp.w - margin;
	comp.cOld = comp.c;
	comp.color = WHITE;
	comp.topBound = 0 + topMargin;
	comp.botBound = H - user.h - margin;
	comp.score = 0;

	ball.s = 5;
	ball.r = H / 2 - ball.s / 2;
	ball.rOld = ball.r;
	ball.rD = 1;
	ball.c = W / 2 - ball.s / 2;
	ball.cOld = ball.c;
	ball.cD = -1;
	ball.color = WHITE;
	ball.leftBound = 0;
	ball.rightBound = W - ball.s;
	ball.topBound = 0 + topMargin;
	ball.botBound = H - ball.s - margin;
	
	ball = resetBall(ball);
	scoreCondition = 0;
	sprintf(buffer, "%d      %d", user.score, comp.score);
	drawString(2, W/2-24, buffer, WHITE);
}

/** delays drawing to a speed humans can better deal with */
void delay(int n)
{
	int i = 0;
	volatile int x=0;
	for(i=0; i<n*10000; i++) {
		x++;
	}
}
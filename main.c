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

/* Function Prototypes */
void newGame();
int paused();
void delay(int n);

u16 bgColor = BLACK;
int scoreCondition;
char buffer[41];
BALL ball;
PADDLE user;
PADDLE comp;

/*
TODO:
- make user paddle line up to center of ball at ballReset
*/
int main(void) 
{
	REG_DISPCNT = MODE3 | BG_ENABLE;

	const char *messageStart = "PRESS ENTER TO PLAY GONG!";
	const char *messageAtAnyTime = "AT ANY TIME,";
	const char *messageSelect = "PRESS SELECT TO RESTART";
	const char *messageB = "OR B TO PAUSE.";
	const char *messageWin = "YOU WIN!";
	const char *messageLose = "YOU LOSE.";
	const char *messagePoints = "FIRST TO FIVE POINTS WINS!";

	drawRect(0,0,W,H,bgColor);
	sprintf(buffer, messageStart);
	drawString(H / 2 - 23, centerTextWidth(messageStart), buffer, WHITE);
	sprintf(buffer, messageAtAnyTime);
	drawString(H / 2 - 13, centerTextWidth(messageAtAnyTime), buffer, WHITE);
	sprintf(buffer, messageSelect);
	drawString(H / 2 - 3, centerTextWidth(messageSelect), buffer, WHITE);
	sprintf(buffer, messageB);
	drawString(H / 2 + 7, centerTextWidth(messageB), buffer, WHITE);
	sprintf(buffer, messagePoints);
	drawString(H / 2 + 17, centerTextWidth(messagePoints), buffer, WHITE);

	while (!KEY_DOWN_NOW(BUTTON_START));
	newGame();

	while (1) 
	{
		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			newGame();
		}
		if(KEY_DOWN_NOW(BUTTON_B)) {
			paused();
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
				drawRect(TOP_MARGIN,0,W,H,bgColor);
				sprintf(buffer, messageWin);
				drawString(H / 2 - 8, centerTextWidth(messageWin), buffer, GREEN);
				sprintf(buffer, messageSelect);
				drawString(H / 2 + 2, centerTextWidth(messageSelect), buffer, GREEN);

				while (! KEY_DOWN_NOW(BUTTON_SELECT));
				newGame();
			}
			if (comp.score >= 5) {
				drawRect(TOP_MARGIN,0,W,H,bgColor);
				sprintf(buffer, messageLose);
				drawString(H / 2 - 8, centerTextWidth(messageLose), buffer, RED);
				sprintf(buffer, messageSelect);
				drawString(H / 2 + 2, centerTextWidth(messageSelect), buffer, RED);
				
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
	ball.s = 5;
	ball.r = H / 2 - ball.s / 2;
	ball.rOld = ball.r;
	ball.rD = -2;
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
	user.r = (ball.s / 2) + ball.r - (user.h / 2);
	user.rOld = user.r;
	user.rD = 2;
	user.c = MARGIN;
	user.cOld = user.c;
	user.color = WHITE;
	user.topBound = 0 + TOP_MARGIN;
	user.botBound = H - user.h - MARGIN;
	user.score = 0;
	user = realignPaddle(user);

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
	drawRect(0,0,W,H,bgColor);
	sprintf(buffer, "%d      %d", user.score, comp.score);
	drawString(2, W / 2 - 24, buffer, WHITE);
	drawRect(ball.r, ball.c, ball.s, ball.s, ball.color);
	drawRect(user.r, user.c, user.w, user.h, user.color);
	drawRect(comp.r, comp.c, comp.w, comp.h, comp.color);
	delay(25);
}

int paused() {
	delay(25);
	while (1) {
		if(KEY_DOWN_NOW(BUTTON_B)) {
			return 0;
		}
	}
}

void delay(int n)
{
	int i = 0;
	volatile int x=0;
	for(i=0; i<n*10000; i++) {
		x++;
	}
}
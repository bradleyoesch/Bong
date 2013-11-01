/* BRADLEY OESCH
 * 10/23/13
 */

#include "mylib.h"
#include "dma.h"

void delay(int n);

int main(void) 
{
	REG_DISPCNT = MODE3 | BG_ENABLE;
	u16 bgColor = BLACK;
	drawRect(0,0,W,H,bgColor);

	PADDLE user;
		user.w = 5;
		user.h = 20;
		user.r = 0;
		user.rOld = user.r;
		user.rD = 1;
		user.c = 5;
		user.cOld = user.c;
		user.color = WHITE;
		user.topBound = 0;
		user.botBound = H-user.h;
	
	PADDLE comp;
		comp.w = 5;
		comp.h = 20;
		comp.r = 0;
		comp.rOld = comp.r;
		comp.rD = 1;
		comp.c = W-user.c-comp.w;
		comp.cOld = comp.c;
		comp.color = WHITE;
		comp.topBound = 0;
		comp.botBound = H-comp.h;

	BALL ball;
		ball.r = 0;
		ball.rOld = ball.r;
		ball.rD = 1;
		ball.c = user.c+user.w;
		ball.cOld = ball.c;
		ball.cD = 1;
		ball.s = 5;
		ball.color = WHITE;
		ball.leftBound = user.c+user.w; //can't go farther left than left paddle
		ball.rightBound = comp.c-ball.s;
		ball.topBound = 0;
		ball.botBound = H-ball.s;

	while(1) 
	{
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

		waitForVblank();

		ball.r += ball.rD;
		ball.c += ball.cD;
		if (ball.r >= ball.botBound || ball.r <= ball.topBound)
			ball.rD *= -1;
		if (ball.c >= ball.rightBound || ball.c <= ball.leftBound)
			ball.cD *= -1;
		drawRect(ball.rOld, ball.cOld, ball.s, ball.s, bgColor);
		drawRect(ball.r, ball.c, ball.s, ball.s, ball.color);

		drawRect(user.rOld, user.cOld, user.w, user.h, bgColor);
		drawRect(user.r, user.c, user.w, user.h, user.color);

		comp.r = (ball.s / 2) + ball.r - (comp.h / 2);
		if (comp.r <= comp.topBound)
				comp.r = comp.topBound;
		if (comp.r >= comp.botBound)
				comp.r = comp.botBound;
		drawRect(comp.rOld, comp.cOld, comp.w, comp.h, bgColor);
		drawRect(comp.r, comp.c, comp.w, comp.h, comp.color);

		/*if(KEY_DOWN_NOW(BUTTON_UP))
		{
			size++;
			if(size > 150)
				size = 150;
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{
			size--;
			if(size<3)
				size = 3;
		}
		
		for(i=0; i<NUMOBJS; i++)
		{
			cur = &objs[i];
			oldcur = &oldobjs[i];
			
			cur->row = cur->row + cur->rd;
			cur->col += cur->cd;
			if(cur->row < 0)
			{
				cur->row = 0;
				cur->rd = -cur->rd;
			}
			if(cur->row > SCREENHEIGHT-1-size+1)
			{
				cur->row = SCREENHEIGHT-1-size+1;
				cur->rd = -cur->rd;
				score++;
			}
			if(cur->col < 0)
			{
				cur->col = 0;
				cur->cd = -cur->cd;
			}
			if(cur->col > 239-size+1)
			{
				cur->col = 239-size+1;
				cur->cd = -cur->cd;
			}
		}

		sprintf(buffer, "Score: %d", score);
		waitForVblank();
		for(i=0; i<NUMOBJS; i++)
		{
			oldcur = &oldobjs[i];
			drawRect(oldcur->row, oldcur->col, oldsize, oldsize, BLACK);  // Erase the ball
			
			oldobjs[i] = objs[i];
		}
		for(i=0; i<NUMOBJS; i++)
		{
			cur = &objs[i];
			drawRect(cur->row, cur->col, size, size, cur->color);
		}  
		oldsize = size;
		drawRect(150, 48, 8, 24, BLACK);
		drawString(150, 5, buffer, RED);*/
	} // While loop
	return 0;
}

/** delays drawing to a speed humans can better deal with */
void delay(int n)
{
	int i = 0;
	volatile int x=0;
	for(i=0; i<n*10000; i++)
	{
		x++;
	}
}
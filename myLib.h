/* BRADLEY OESCH
 * 10/30/13
 */

typedef unsigned short u16;
extern u16 *videoBuffer;

/* Objects of sorts */
typedef struct
{
	int w;		// width
	int h;		// height
	int r;		// row
	int rOld;	// old row value for painting over
	int rD;		// change in row
	int c;		// column
	int cOld;	// old col value for painting over
	u16 color;	// color
	
	int topBound;	// least r can be
	int botBound;	// greatest r can be
	int score;
} PADDLE;

typedef struct
{
	int s;		// size (width and height)
	int r;		// row
	int rOld;	// old row value for painting over
	int rD;		// change in row
	int c;		// column
	int cOld;	// old col value for painting over
	int cD;		// change in col
	u16 color;	// color

	int leftBound;	// least c can be
	int rightBound;	// greatest c can be
	int topBound;	// least r can be
	int botBound;	// greatest r can be
} BALL;

#define REG_DISPCNT *(u16 *)0x4000000
#define RGB(r,g,b) ((r) | ((g)<<5) | ((b)<<10))
#define MODE3 3
#define BG_ENABLE (1<<10)

#define W 240
#define H 160
#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define TRUE_WHITE RGB(31,31,31)
#define	WHITE RGB(29,29,29)
#define	TRUE_BLACK RGB(0,0,0)
#define BLACK RGB(2,2,2)
#define DARK_GREY RGB(25,25,25)
#define RED RGB(29,2,2)
#define YELLOW RGB(30,29,4)
#define BROWN RGB(13,11,7)
#define BLUE RGB(0,21,30)
#define DARK_BLUE RGB(1,13,16)

/* Buttons */
#define BUTTON_A		(1<<0) // Z
#define BUTTON_B		(1<<1) // X
#define BUTTON_SELECT	(1<<2) // Backspace
#define BUTTON_START	(1<<3) // Enter
#define BUTTON_RIGHT	(1<<4) // Right
#define BUTTON_LEFT		(1<<5) // Left
#define BUTTON_UP		(1<<6) // Up
#define BUTTON_DOWN		(1<<7) // Down
#define BUTTON_R		(1<<8) // S
#define BUTTON_L		(1<<9) // A
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130

/* Function Prototypes */
BALL resetBall(BALL ball);
void drawRect(int r, int c, int width, int height, u16 color);
void setPixel(int r, int c, u16 color);
void fillScreen(u16 color);
int hitsPaddle(BALL ball, PADDLE paddle);
int checkScoreCondition(BALL ball);
void waitForVblank();
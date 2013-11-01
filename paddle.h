/* BRADLEY OESCH
 * 10/31/13
 */

extern unsigned short u16;

typedef struct
{
	int row;
	int rowD; // change in row location, moving up and down
	int col;
	int width;
	int height;
	u16 color;
} PADDLE;
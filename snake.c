#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <stdio.h>
struct pos {
	int x;
	int y;
	char symbol;
};
struct food {
	struct pos cord;
	int value;
};
const int max_X = 40;
const int max_Y = 30;
bool proverka(struct pos cord, struct pos *snake, int d)
{
	for (int i = 0; i < d; i++)
	{
		if ((cord.x == snake[i].x) && (cord.y == snake[i].y))
			return true;
	}
	return false;
}
void printpos(int y, int x,char f) {
	move(y,x);
	addch(f);
}
int naprav(int ch)
{
	int nap1 = -1;
	switch (ch) {
	case 'w':   nap1 = 2;break;
	case 's':   nap1 = 3;break;
	case 'a':   nap1= 1; break;
	case 'd':   nap1 = 0; break;
	}
	return nap1;
}
void foodcreate(struct food fod,int val, struct pos *snake, int d) {
	do {
		fod.cord.x = rand() % max_X + 1;
		fod.cord.y = rand() % max_Y + 1;
	} while (proverka(fod.cord,snake, d));
	fod.value = val;
	char valc = val;
	printpos(fod.cord.y,fod.cord.x,valc);
	refresh();
}
void drowsnake(struct pos *snake, int d, char f)
{
	for (int i = 0; i < d; i++)
	{
	 printpos(snake[i].y,snake[i].x, f);
	}
}
void rost(struct pos *snake,struct pos headcord, int value, int d)
{
	for (int j = 0; j < value; j++) {
		snake[d] = headcord;
		snake[d].symbol = '#';
		d++;
	}
}
int main()
{
	initscr();

	for (int i = 0; i < max_X + 2; i++) {
		printpos( i,0, '0');
		printpos( i,max_Y + 1, '0');
	}
	for (int i = 0; i < max_Y + 2; i++) {
		printpos( 0,i , '0');
		printpos( max_X + 1,i, '0');
	}
	struct pos snake[max_X * max_Y];
	struct food fod;
	int nextnap[2][4] = {{1,-1,0,0},{0,0,1,-1}};
	int d = 4;
	struct pos headcord;
	headcord.x = 5;
	headcord.y = 15;
	headcord.symbol = '#';
	for (int i = 0; i < d; i++) {
		snake[i].x = 5 + i;
		snake[i].y = 15;
		snake[i].symbol = '#';
		printpos(snake[i].y,snake[i].x, '#');
	}
	int nap1 = 0;
	struct pos nextpos; 
	int nap2 = 0;
	int r = 0;
	int ch;
	bool gameover = false;
	//int score = 0;
	while (!gameover)
	{
		ch = getch();
		nap1 = naprav(ch);
		nextpos.x = headcord.x + nextnap[nap1][0];
		nextpos.y = headcord.y + nextnap[nap1][1];
		gameover = proverka(nextpos,snake, d);
		if (!gameover) {
			drowsnake(snake, d, ' ');
			snake[r] = headcord;
			r++;
			if (r >= d) {
				r = 0;
			}
			drowsnake(snake, d, '#');
			if ((fod.cord.x == headcord.x) && (fod.cord.y == headcord.y))
				rost(snake,headcord, fod.value, d);
			foodcreate(fod,rand()%4+1,snake, d);
		}
		usleep(10000 / d);
	}
	endwin();
return 0;
}

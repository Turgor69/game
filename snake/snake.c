#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
int naprav(int ch, int nap)
{
	switch (ch) {
		case 'w': if(nap != 2) return 3;break;
		case 's': if(nap != 3) return 2;break;
		case 'a': if(nap != 0) return 1; break;
		case 'd': if(nap != 1) return 0; break;
	}
	return nap;
}
struct food foodcreate(struct food fod,int val, struct pos *snake, int d) {
	do {
		fod.cord.x = rand() % max_X + 1;
		fod.cord.y = rand() % max_Y + 1;
	} while (proverka(fod.cord,snake, d));
	fod.value = val;
	char valc = val;
	printpos(fod.cord.y,fod.cord.x,valc+48);
	refresh();
	return fod;
}
void drowsnake(struct pos *snake, int d, char f)
{
	for (int i = 0; i < d; i++)
	{
	printpos(snake[i].y,snake[i].x, f);
	}
	refresh();
}

int main()
{
	initscr();
	noecho();
	start_color();
	curs_set(0);
	nodelay(stdscr, TRUE);
	for (int i = 0; i < max_Y + 2; i++) {
		printpos( i,0, '0');
		printpos( i,max_X + 1, '0');
	}
	for (int i = 0; i < max_X + 2; i++) {
		printpos( 0,i , '0');
		printpos( max_Y + 1,i, '0');
	}
	struct pos snake[max_X * max_Y];
	struct food fod;
	int nextnap[2][4] = {{1,-1,0,0},{0,0,1,-1}};
	int d = 4;
	int ro =0;
	for (int i = 0; i < d; i++) {
		snake[i].x = 5 + i;
		snake[i].y = 15;
		snake[i].symbol = '#';
	}
	drowsnake(snake, d,'#');
	refresh();
	int nap1 = 1;
	struct pos nextpos;
	int r = 3;
	int ch;
	bool gameover = false;
	fod = foodcreate(fod,rand()%4+1,snake, d);
	while (!gameover){
		ch = getch();
		nap1 = naprav(ch, nap1);
		nextpos.x = snake[0].x + nextnap[0][nap1];
		nextpos.y = snake[0].y + nextnap[1][nap1];
		if (nextpos.x > max_X)
			nextpos.x-=max_X;
		if (nextpos.x < 1)
			nextpos.x+=max_X;
		if (nextpos.y > max_Y)
			nextpos.y-=max_Y;
		if (nextpos.y < 1)
			nextpos.y+=max_Y;
		gameover = proverka(nextpos, snake, d);
		if (!gameover) {
			struct pos lastpos;
			lastpos.x = snake[d-1].x;
			lastpos.y = snake[d-1].y;
			drowsnake(snake, d,' ');
			for (int r = d - 1;r>0;r--){
				snake[r].x = snake[r-1].x;
				snake[r].y = snake[r-1].y;
			}
			if (ro>0){
				snake[d].x = lastpos.x;
				snake[d].y = lastpos.y;
				snake[d].symbol = '#';
				d++;
				ro--;
			}
			if ((fod.cord.x == snake[0].x) && (fod.cord.y == snake[0].y)){
				ro+=fod.value;
				fod = foodcreate(fod,rand()%4+1,snake, d);
			}
			snake[0].x = nextpos.x;
			snake[0].y = nextpos.y;
			drowsnake(snake, d, '#');
		}
		usleep(100000);
	}
endwin();
return 0;
}

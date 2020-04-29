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
	if ((snake[0].x = max_X) || (snake[0].y = max_Y))
	return false;
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
	refresh();
}
int naprav(int ch, int nap)
{
	switch (ch) {
	case 'w': if(nap != 3) return 2;break;
	case 's': if(nap != 2) return 3;break;
	case 'a': if(nap != 0) return 1; break;
	case 'd': if(nap != 1) return 0; break;
	}
	return nap;
}
void foodcreate(struct food fod,int val, struct pos *snake, int d) {
	do {
		fod.cord.x = rand() % max_X + 1;
		fod.cord.y = rand() % max_Y + 1;
	} while (proverka(fod.cord,snake, d));
	fod.value = val + 48;
	char valc = val + 48;
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

int rost(struct pos *snake,int x,int y, int value, int d)
{
		snake[d].x = x;
		snake[d].y = y;
		snake[d].symbol = '#';
		return d++;
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
		printpos(snake[i].y,snake[i].x, '#');
	}
	refresh();
	int nap1 = 3;
	struct pos nextpos;
	int ch;
	bool gameover = false;
	while (!gameover)
	{
		ch = getch();
		nap1 = naprav(ch, nap1);
		nextpos.x = snake[0].x + nextnap[nap1][0];
		nextpos.y = snake[0].y + nextnap[nap1][1];
		gameover = proverka(nextpos, snake, d);
		if (!gameover) {
			struct pos lastpos;
			lastpos.x = snake[d-1].x;
			lastpos.y = snake[d-1].y;
			for (int r=d-1;r>0;r--){
				snake[r].x = snake[r-1].x;
				snake[r].y = snake[r-1].y;
			}
			if ((fod.cord.x == snake[0].x) && (fod.cord.y == snake[0].y)){
				ro+=fod.value;
				if (ro>0){
					d=rost(snake,lastpos.x,lastpos.y,ro, d);
					ro--;
				}
				foodcreate(fod,rand()%4+1,snake, d);
			}
			drowsnake(snake, d,' ');

			snake[0].x = nextpos.x;
			snake[0].y = nextpos.y;
			drowsnake(snake, d, '#');
		}
		usleep(100000);
	}
	endwin();
	return 0;
}

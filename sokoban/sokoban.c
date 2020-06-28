#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

struct pos {
	int x;
	int y;
};
	
void printpos(int x, int y, char f) {
	move(y, x);
	addch(f);
	refresh();
}

void Clear(int x, int y) {
	move(y, x);
	addch(' ');
	refresh();
}

struct pos step(struct pos sok, int dx, int dy, char symbvol){
	Clear(sok.x, sok.y);		
	sok.x = dx;
	sok.y = dy;
	printpos(sok.x, sok.y, symbvol);
	refresh();
	return sok;
}

bool comp(struct pos A, struct pos B) {
	return A.x == B.x && A.y == B.y;
}

void draw_all(struct pos *ban, struct pos *stena, struct pos *banplase, int banov, int sten, int plase, struct pos sok) {
	for (int i = 0; i < sten; i++) {
		printpos(stena[i].x, stena[i].y, '#');	
	}	
	for (int i = 0; i < plase; i++) {
		printpos(banplase[i].x, banplase[i].y, '0');		
	}
	for (int i = 0; i < banov; i++) {
		printpos(ban[i].x, ban[i].y, 'b');	
	}		
	printpos(sok.x, sok.y, 's');
	refresh();
}

void clear_all(struct pos *ban, struct pos *stena, struct pos *banplase, int banov, int sten, int plase, struct pos sok) {
	for (int i = 0; i < banov; i++) {
		Clear(ban[i].x, ban[i].y);	
	}	
	for (int i = 0; i < sten; i++) {
		Clear(stena[i].x, stena[i].y);	
	}	
	for (int i = 0; i < plase; i++) {
		Clear(banplase[i].x, banplase[i].y);		
	}
	Clear(sok.x,sok.y);
}

int Col = -1;
int Proverkasok(struct pos *ban, struct pos *stena, struct pos *banplase, int banov, int sten, int plase, struct pos sok, bool  pr) {
	for (int i = 0; i < banov; i++) {
		if (comp(ban[i],sok)) {
			if (pr) Col = i;
			return 1;	
		}		
	}	
	for (int i = 0; i < sten; i++) {
		if (comp(stena[i], sok)) {
			if (pr) Col = i;
 			return 2;	
		}
	}		
	return -1;
}

int naprav(int ch, int nap) {
	switch (ch) {
		case 'a': return 1; break;
		case 'd': return 0; break;
		case 'w': return 3; break;
		case 's': return 2; break;
		case 'r': nap = -1; break;
	}
	return nap; 
}

bool win (struct pos *banplase, int plase, struct pos ban[], int banov) {
	int f = 0;
	for (int i = 0; i < plase; i++){
		for (int j = 0; j < banov; j++){
			if (banplase[i].x == ban[j].x && banplase[i].y == ban[j].y) {
				f++;
			}	
		}	
	}
	if (f == plase)
	return true;
	else
	return false;
}

int main() {
	initscr();
	int count_on_point = 0;
	int count_obj = 0;
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	setlocale(LC_CTYPE, "");
	curs_set(0);
	noecho();
	int nap = 1;
	struct pos sok;
	struct pos ban[5];//box
	struct pos banplase[5];//plase
	struct pos stena[100];

	int banov = 0;
	int plase = 0;
	int sten = 0;
	char c;
	
	int level = 0;
	char str[10];
	sprintf(str, "%d", level);
	char s[100] = "pole";
	strcat(s,".txt");
	
	FILE *in;
	while(nap !=-1) {
		if(in = fopen(s, "rd")){
			int i = 0;
			int j = 0;
			int mx = 0;
			banov = 0;
			plase = 0;
			sten = 0;
			mx = 0;
			count_on_point = 0;
			count_obj = 0;
			while (fscanf(in,"%c", &c) == 1) {
				if (c == 's') { //player
					sok.x = i;
					sok.y = j;			
				}
				if (c == 'b') { //Box
					ban[banov].x = i;
					ban[banov].y = j;
					banov++;		
				}
				if (c == '0'){ //place for Box
					banplase[plase].x = i;
					banplase[plase].y = j;
					plase++;
					count_obj++;			
				}
				if (c == '#') { // walls
					
					stena[sten].x = i;
					stena[sten].y = j;
					sten++;				
				}

				if (c == '\n') {
					j++;
					i = 0;		
				} else {
					i++;		
					if (i > mx) mx = i;	
				}	
			}
			fclose(in);
			draw_all(ban, stena, banplase, banov, sten, plase, sok);
			refresh();
			int dir = -1;
			bool restart = false;
			int nextnap[2][4] = {{1,-1,0,0},{0,0,1,-1}};
			int nap = -1;
			while(!win(banplase, plase, ban, banov)) {
				int ch = getch();		
				nap = naprav(ch, nap);
				if(nap == -1) break;
				struct pos nextpos;
				nextpos.x = sok.x + nextnap[0][nap];
				nextpos.y = sok.y + nextnap[1][nap];
					int proverka1 = Proverkasok(ban, stena, banplase, banov, sten, plase, nextpos, true);
				if (proverka1 == -1){
					sok = step(sok, nextpos.x, nextpos.y, 's');
				}
				if (proverka1 == 1){
					struct pos nextpos1;
					nextpos1.x = nextpos.x + nextnap[0][nap];
					nextpos1.y = nextpos.y + nextnap[1][nap];
					int proverka2 = Proverkasok(ban, stena, banplase, banov, sten, plase, nextpos1, false);
					if (proverka2 == -1){
						sok = step(sok, nextpos.x, nextpos.y, 's');
						ban[Col] = step(ban[Col], nextpos1.x, nextpos1.y, 'b');
					}		
				}
				draw_all(ban, stena, banplase, banov, sten, plase, sok);		
			}
			i = 0;
			refresh();
		}
		if (!win(banplase, plase, ban, banov))
		clear_all(ban, stena, banplase, banov, sten, plase, sok);
		else
		nap = -1;
	}
	endwin();
	printf("Вы помогли кладовщику расставить все коробки\n");
	return 0;
}

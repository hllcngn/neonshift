#include <ncurses.h>
#include <stdlib.h> //malloc
#include <time.h> //CLOCKS_PER_SEC clock
#include <unistd.h> //usleep
#include "struct.h" //structs: curs ptng, constants
#include "modes.h" //constants, macros, functioning

void	title_quit(){ endwin(); exit(EXIT_SUCCESS);}


int	main(int ac, char **av){
initscr(); cbreak(); noecho();
nodelay(stdscr, TRUE);
start_color(); //curs_set(0);
srand(time(NULL)); //random expansion

WINDOW		*win, *wui;
struct ptng	ptng;
struct vect	curs, mov_v, sweep_v;
unsigned char	mov_mod, edt_mod;
char		color, overall_color, q, c;
int		refresh_rate, fps;
clock_t		clk_start, clk;
FILE		*file;

ptng.w = BASE_W; ptng.h = BASE_H; ptng.size = ptng.w*ptng.h;
curs.y = ptng.h-1; curs.x = ptng.w-1;
mov_mod = 2; edt_mod = 0; color = 2; q = 1;
fps = BASE_FPS; refresh_rate = CLOCKS_PER_SEC/fps;

init_pair(10, 14, 0); refresh();	//title screens
attron(COLOR_PAIR(10)|A_BOLD);
mvprintw((ptng.h+2+3+1)/2-6, (ptng.w+2+7+4)/2-25/2,
		"N   E   O   N   S   H   I   F   T");
mvprintw((ptng.h+2+3+1)/2-6+1, (ptng.w+2+7+4)/2-17/2-5,
		"ASCII speedpainting brain interface");
mvprintw((ptng.h+2+3+1)/2-6+3, (ptng.w+2+7+4)/2-2,
		"prototype no1");
mvprintw((ptng.h+2+3+1)/2-6+6, (ptng.w+2+7+4)/2-6,
		"Green           Red");
attron(A_UNDERLINE);
mvaddch((ptng.h+2+3+1)/2-6+6, (ptng.w+2+7+4)/2-6, 'G');
mvaddch((ptng.h+2+3+1)/2-6+6, (ptng.w+2+7+4)/2-6+16, 'R');
move((ptng.h+2+3+1)/2-6+1, (ptng.w+2+7+4)/2-17/2-2+32);
attroff(A_UNDERLINE); attroff(A_BOLD); refresh();
c = 1; while (c&&(c=getch())){ switch(c){
	case 'q': title_quit(); break;
	case 'g': init_pair(1, 10, 0); //black
		init_pair(2, 0, 10); //green
		overall_color = 'g'; c = 0; break;
	case 'r': init_pair(1, 9, 0); //black
		init_pair(2, 0, 9); //red
		overall_color = 'r'; c = 0; break;
	default: break;}}
erase(); attron(COLOR_PAIR(1));
mvprintw((ptng.h+2+3+1)/2-6+3, (ptng.w+2+7+4)/2-2,
		"to darkmage"); refresh();
while ((c=getch())==ERR);
if (c == 'q') title_quit();

win = newwin(ptng.h+2, ptng.w+2, 3, 7);		//setup
wattron(win, COLOR_PAIR(1));
box(win, 0, 0); wrefresh(win); delwin(win);
win = newwin(ptng.h, ptng.w, 4, 8);
wui = newwin(20, 10, 4, 9+ptng.w+5);
wattron(wui, COLOR_PAIR(1));
wprintw(wui, (O(edt_mod)?"\n\nspot \n":"\n\nstroke\n"));
mvwprintw(wui, 8, 0, "fps: %i\n", fps);
mvwprintw(wui, 10, 0, "sweeping:\nreversed");
wattron(wui, COLOR_PAIR(color));
mvwprintw(wui, 5, 0, "c c c\nc c c\n\n");
wrefresh(wui); wattron(wui, COLOR_PAIR(1));

ptng.buf = (char *)malloc(ptng.size);
if ((file=fopen("painting", "r"))) { int i=0;
	while ((c=fgetc(file)) != EOF){
		c = fgetc(file); switch(c){
		case '9': case '4': ptng.buf[i] = 0;
			wattron(win, COLOR_PAIR(1));
			waddch(win, ' '); break;
		case '1': ptng.buf[i] = 1;
			wattron(win, COLOR_PAIR(2));
			waddch(win, ' '); break;
		defautl: break;}  i++;
		fgetc(file); fgetc(file); fgetc(file);
		if (!(i%ptng.w)) fgetc(file);}
	fclose(file); wrefresh(win);}
else for (int i=0; i<ptng.size; i++) ptng.buf[i] = 0;

wattron(win, COLOR_PAIR(color));
clk_start = clock();
/* ========================== */	while (q){	/*====*/

if ((c=getch())!=ERR){ switch(c){
case 27: if (getch()==ERR) q = 0; break; //TODO: add confirm window
case 's': file = fopen("painting", "w");
	for (int i=0; i<ptng.size; i++){ switch(ptng.buf[i]){
		case 0: fputc(3, file);	//black
			if (overall_color == 'g') fprintf(file, "9,1 ");
			else fprintf(file, "4,1 "); break;
		case 1: fputc(3, file); //green/red
			if (overall_color == 'g') fprintf(file, "1,9 ");
			else fprintf(file, "1,4 "); break;
		default: break;}
		if (i && !((i+1)%ptng.w)) fputc('\n', file);}
	fclose(file); break;

case 'c': if (color == 2) color = 1; else color = 2;
	change_color(color, win, wui); break;
case '-': if(fps>1) fps--; refresh_rate = CLOCKS_PER_SEC/fps;
	 mvwprintw(wui, 8, 5, "%i  ", fps);
	 wrefresh(wui); break;
case '+': fps++; refresh_rate = CLOCKS_PER_SEC/fps;
	 mvwprintw(wui, 8, 5, "%i  ", fps);
	 wrefresh(wui); break;

case 'z': toogle_z(&edt_mod, wui); break;
case ';': toogle_o(&edt_mod, wui); break;
case 'p': toogle_i(&edt_mod, wui); break;

case ' ': mov_mod = switchf(mov_mod, MP);
	  mvwprintw(wui, 10, 0, (P(mov_mod)?"*PAUSED* ":"sweeping:"));
	  wrefresh(wui); break;
case 'b': toogle_r(&mov_mod, wui); break;
case 'g': toogle_v(&mov_mod, wui); break;

case 'h': mov_mod = switchf(mov_mod, MH); break;
case 'j': mov_mod = switchf(mov_mod, MJ); break;
case 'k': mov_mod = switchf(mov_mod, MK); break;
case 'l': mov_mod = switchf(mov_mod, ML); break;

// random expansion
case 'w': //random position
	curs.y = rand()%BASE_H;
	curs.x = rand()%BASE_W;
	break;
case 'e': //random sweeping direction
	(rand()%2)?toogle_r(&mov_mod, wui):1;
	(rand()%2)?toogle_v(&mov_mod, wui):1;
	break;
case 'r': //random position & sweeping direction
	(rand()%2)?toogle_r(&mov_mod, wui):1;
	(rand()%2)?toogle_v(&mov_mod, wui):1;
	curs.y = rand()%BASE_H;
	curs.x = rand()%BASE_W;
	break;
case 't': //random spot
	curs.y = rand()%BASE_H;
	curs.x = rand()%BASE_W;
	unsigned char i = edt_mod;
	if (!O(edt_mod))
		toogle_o(&edt_mod, wui);
	if (!Z(edt_mod))
		edt_mod = switchf(edt_mod, EZ);
	edit_pntg(win, wui, &curs, &ptng, &edt_mod, color);
	if (!O(i))
		toogle_o(&edt_mod, wui);
	break;
case 'y': //randomize edit mode
	(rand()%2)?toogle_o(&edt_mod, wui):1;
	(rand()%2)?toogle_i(&edt_mod, wui):1;
	break;
default: break;}}

mov_v = get_mov_v(&mov_mod);
move_curs(&curs, &ptng, mov_v);
edit_pntg(win, wui, &curs, &ptng, &edt_mod, color);

wrefresh(win);
clk = clock()-clk_start;
usleep((refresh_rate-clk)*(1000000/CLOCKS_PER_SEC));
clk_start = clock();	/* ================== */	}	/*====*/

free(ptng.buf);
delwin(win); delwin(wui); endwin();
return 0;}

void toogle_z(unsigned char *edt_mod, WINDOW *wui){
*edt_mod = switchf(*edt_mod, EZ);
mvwprintw(wui, 0, 0, ((Z(*edt_mod) && !O(*edt_mod))?"editing":"       "));
wrefresh(wui);}
void toogle_o(unsigned char *edt_mod, WINDOW *wui){
*edt_mod = switchf(*edt_mod, E1);
mvwprintw(wui, 2, 0, (O(*edt_mod)?"spot  ":"stroke"));
wrefresh(wui);}
void toogle_i(unsigned char *edt_mod, WINDOW *wui){
*edt_mod = switchf(*edt_mod, EI);
mvwprintw(wui, 3, 0, (I(*edt_mod)?"invert":"      "));
wrefresh(wui);}
void toogle_r(unsigned char *mov_mod, WINDOW *wui){
*mov_mod = switchf(*mov_mod, MR);
mvwprintw(wui, 11, 0, (R(*mov_mod)?"reversed":"normal  "));
wrefresh(wui);}
void toogle_v(unsigned char *mov_mod, WINDOW *wui){
*mov_mod = switchf(*mov_mod, MV);
mvwprintw(wui, 12, 0, (V(*mov_mod)?"vertical":"        "));
wrefresh(wui);}

//by d0pelrh

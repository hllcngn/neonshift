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

init_pair(10, 14, 0); refresh();	//title screen
attron(COLOR_PAIR(10)|A_BOLD);
mvprintw((ptng.h+2+3+1)/2-6, (ptng.w+2+7+4)/2-25/2,
		"N   E   O   N   S   H   I   F   T");
mvprintw((ptng.h+2+3+1)/2-6+2, (ptng.w+2+7+4)/2-17/2-5,
		"ASCII speedpainting brain interface");
mvprintw((ptng.h+2+3+1)/2-6+3, (ptng.w+2+7+4)/2-2,
		"prototype no1");
mvprintw((ptng.h+2+3+1)/2-6+4, (ptng.w+2+7+4)/2-3,
		"expanded edition");
mvprintw((ptng.h+2+3+1)/2-6+6, (ptng.w+2+7+4)/2-6,
		"Green           Red");
attron(A_UNDERLINE);
mvaddch((ptng.h+2+3+1)/2-6+6, (ptng.w+2+7+4)/2-6, 'G');
mvaddch((ptng.h+2+3+1)/2-6+6, (ptng.w+2+7+4)/2-6+16, 'R');
move((ptng.h+2+3+1)/2-6+2, (ptng.w+2+7+4)/2-17/2-2+32);
attroff(A_UNDERLINE); attroff(A_BOLD); refresh();
c = 1; while (c&&(c=getch())){ switch(c){
	case 27: if (getch()==ERR) title_quit(); break;
	case 'g': init_pair(1, 10, 0); //black
		init_pair(2, 0, 10); //green
		overall_color = 'g'; c = 0; break;
	case 'r': init_pair(1, 9, 0); //black
		init_pair(2, 0, 9); //red
		overall_color = 'r'; c = 0; break;
	default: break;}}

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
		case '9':
		case '4': ptng.buf[i] = 0;
			  wattron(win, COLOR_PAIR(1));
			  waddch(win, ' '); break;
		case '1': ptng.buf[i] = 1;
			  wattron(win, COLOR_PAIR(2));
			  waddch(win, ' '); break;
		defautl:  break;}  i++;
		fgetc(file); fgetc(file); fgetc(file);
		if (!(i%ptng.w)) fgetc(file);}
	fclose(file); wrefresh(win);}
else for (int i=0; i<ptng.size; i++) ptng.buf[i] = 0;

wattron(win, COLOR_PAIR(color));
clk_start = clock();
/* ========================== */	while (q){	/*====*/

if ((c=getch())!=ERR){ switch(c){
case 27: if (getch()!=ERR) break;
	{WINDOW *qwin = newwin(8, 30, 3+(ptng.h+2)/2-8/2, 7+(ptng.w+2)/2-30/2);
	wattron(qwin, COLOR_PAIR(1)); box(qwin, 0, 0);
	mvwprintw(qwin, 8/2-2, 30/2-20/2, "save before exiting?");
	mvwprintw(qwin, 8/2+1, 30/2-19/2, "(Y/s - n/q/* - ESC)");
	wrefresh(qwin);
	c = 1; while (c&&(c=getch())){ switch(c){
	case 27:  if (getch()==ERR) c = 0; break;
	case ERR: break;
	case '\n':
	case 'y':
	case 's': save_ptng(&ptng, overall_color); c = 0; q = 0; break;
	case 'n':
	case 'q':
	default:  c = 0; q = 0; break;}}
	delwin(qwin);} redraw_ptng(win, &curs, &ptng, color);
	clk_start = clock(); break;
case 's': save_ptng(&ptng, overall_color); break;

case '-': if(fps>1) fps--; refresh_rate = CLOCKS_PER_SEC/fps;
	  mvwprintw(wui, 8, 5, "%i  ", fps);
	  wrefresh(wui); break;
case '+': fps++; refresh_rate = CLOCKS_PER_SEC/fps;
	  mvwprintw(wui, 8, 5, "%i  ", fps);
	  wrefresh(wui); break;

case 'c': if (color == 2) color = 1; else color = 2;
	  change_color(color, win, wui); break;
case 'z': toogle_z(&edt_mod, wui); break;
case ';': toogle_o(&edt_mod, wui); break;
case 'p': toogle_i(&edt_mod, wui); break;

case 'b': toogle_r(&mov_mod, wui); break;
case 'g': toogle_v(&mov_mod, wui); break;
case ' ': mov_mod = switchf(mov_mod, MP);
	  mvwprintw(wui, 10, 0, (P(mov_mod)?"*PAUSED* ":"sweeping:"));
	  wrefresh(wui); break;

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
	{unsigned char i = edt_mod;
	if (!O(edt_mod))
		toogle_o(&edt_mod, wui);
	if (!Z(edt_mod))
		edt_mod = switchf(edt_mod, EZ);
	edit_pntg(win, wui, &curs, &ptng, &edt_mod, color);
	if (!O(i))
		toogle_o(&edt_mod, wui);}
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

//by d0pelrh

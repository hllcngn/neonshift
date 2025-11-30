#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "neonshift.h"
#include "conf.h"

int	change_color(char color, WINDOW *win, WINDOW *wui){
	wattron(win, COLOR_PAIR(color));
	wattron(wui, COLOR_PAIR(color));
	mvwprintw(wui, 5, 0, "c c c\nc c c");
	wrefresh(wui); wattron(wui, COLOR_PAIR(1));
return 0;}

struct vect	get_mov_v(unsigned char *mov_mod){
	struct vect	v;
	if (H(*mov_mod)){
		*mov_mod = switchf(*mov_mod, MH);
		v.y = 0; v.x = -1;}
	else if (J(*mov_mod)){
		*mov_mod = switchf(*mov_mod, MJ);
		v.y = 1; v.x = 0;}
	else if (K(*mov_mod)){
		*mov_mod = switchf(*mov_mod, MK);
		v.y = -1; v.x = 0;}
	else if (L(*mov_mod)){
		*mov_mod = switchf(*mov_mod, ML);
		v.y = 0; v.x = 1;}
	else if (P(*mov_mod)){
		v.y = 0; v.x = 0;}
	else if (!V(*mov_mod) && !R(*mov_mod)){
		v.y = 0; v.x = 1;}
	else if (!V(*mov_mod) && R(*mov_mod)){
		v.y = 0; v.x = -1;}
	else if (V(*mov_mod) && !R(*mov_mod)){
		v.y = 1; v.x = 0;}
	else if (V(*mov_mod) && R(*mov_mod)){
		v.y = -1; v.x = 0;}
return v;}

void	move_curs(struct vect *c, struct ptng *p, struct vect v){
	if (!c->y&&!c->x&&(v.y==-1||v.x==-1)){ c->y=p->h-1; c->x=p->w-1;}
	else if (c->y==p->h-1&&c->x==p->w-1&&(v.y==1||v.x==1)){ c->y=0; c->x=0;}
	else if (!c->y&&v.y==-1){ c->y=p->h-1; c->x--;}
	else if (c->y==p->h-1&&v.y==1){ c->y=0; c->x++;}
	else if (!c->x&&v.x==-1){ c->y--; c->x=p->w-1;}
	else if (c->x==p->w-1&&v.x==1){ c->y++; c->x=0;}
	else if (v.y==-1) c->y--;
	else if (v.y==1) c->y++;
	else if (v.x==-1) c->x--;
	else if (v.x==1) c->x++;}

void	edit_pntg(WINDOW *win, WINDOW *wui, struct vect *c, struct ptng *p,
		unsigned char *edt_mod, char color){
	int	bi = c->y*p->w+c->x;
	wmove(win, c->y, c->x);
	if (Z(*edt_mod)){
		if (O(*edt_mod)) toogle_z(edt_mod, wui);
		if (!I(*edt_mod)){
			if (color == 1) p->buf[bi] = 0;
			else if (color == 2) p->buf[bi] = 1;
			waddch(win, ' ');}
		else{	if (p->buf[bi] == 0){
				p->buf[bi] = 1;
				wattron(win, COLOR_PAIR(2));}
			else if (p->buf[bi] == 1){
				p->buf[bi] = 0;
				wattron(win, COLOR_PAIR(1));}
			waddch(win, ' ');
			wattron(win, COLOR_PAIR(color));}
		wmove(win, c->y, c->x);}}

void	redraw_ptng(WINDOW *win, struct vect *c, struct ptng *p, char color){
	for (int i = 0; i<p->h; i++){
		wmove(win, i, 0);
		for (int j = 0; j<p->w; j++){
			(p->buf[i*p->w+j]?
			wattron(win, COLOR_PAIR(2)):
			wattron(win, COLOR_PAIR(1)));
			waddch(win, ' ');}}
	wattron(win, COLOR_PAIR(color));
	wmove(win, c->y, c->x);
	wrefresh(win);}

char*	get_filename(){
char *filename = malloc(100);
filename[0] = 0;
strcat(filename, getenv("HOME"));
strcat(filename, FILEPATH);
strcat(filename, FILENAME);
return filename;}

void	save_ptng(struct ptng *ptng, char overall_color){
	char *filename = get_filename();
	FILE *file = fopen(filename, "w");
	for (int i=0; i<ptng->size; i++){ switch(ptng->buf[i]){
		case 0: fputc(3, file);	//black
			if (overall_color == 'g') fprintf(file, "9,1 ");
			else fprintf(file, "4,1 "); break;
		case 1: fputc(3, file); //green/red
			if (overall_color == 'g') fprintf(file, "1,9 ");
			else fprintf(file, "1,4 "); break;
		default: break;}
		if (i && !((i+1)%ptng->w)) fputc('\n', file);}
	fclose(file); free(filename);}

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

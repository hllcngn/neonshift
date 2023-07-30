#ifndef STRUCT_H
#define STRUCT_H

#define BASE_W 70
#define BASE_H 20
#define BASE_FPS 30

struct	vect{
	char	y;
	char	x;
};

struct	curs{
	char	y;
	char	x;
	char	c;
	int	fps;
};

struct	ptng{
	short	w;
	short	h;
	int	size;
	short	*buf;
};

#endif

all:
	gcc -o neonshift src/main.c src/func.c -Isrc -lncurses

clean:
	rm -f neonshift

re:	clean all

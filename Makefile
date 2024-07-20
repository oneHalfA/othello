exec: othello.c
	gcc $< -o $@ -Wall 
clean:
	rm -rf *.c exec

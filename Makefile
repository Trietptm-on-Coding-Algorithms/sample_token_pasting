all: sample_token_pasting.c
	gcc -save-temps -o token_pasting sample_token_pasting.c

clean:
	rm -vf *.i *~ *.o *.s 

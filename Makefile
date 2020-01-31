
# Taylor Cox and Shika Pandey

all: boring custom multi

boring: boring.c
	gcc -o boring boring.c

custom: custom.c
	gcc -o custom custom.c

multi: multi.c
	gcc -o multi multi.c

clean:
	rm boring custom

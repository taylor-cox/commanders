
# Taylor Cox and Shika Pandey

all: boring custom

boring: boring.c
	gcc -o boring boring.c

custom: custom.c
	gcc -o custom custom.c

clean:
	rm boring custom

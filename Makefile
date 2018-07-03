all: foo.c
	cc -Wall -Wextra -O2 -o foo foo.c

clean:
	rm foo

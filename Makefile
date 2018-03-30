all:
	cc -std=c99 -O2 -pthread -o dumb dumb.c -lpcap

clean:
	rm -rf dumb *.o *.dSYM

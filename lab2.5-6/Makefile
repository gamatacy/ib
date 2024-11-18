DECRYPT ?= 0

clean:
	rm -f a.out

build: clean
	gcc main.c crypto.c -lm -O2 -Wno-overflow -Wno-multichar -DDECRYPT=$(DECRYPT)

exact: build
	./a.out

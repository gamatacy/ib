LAB_NUM ?= 2.5-6
CLIB_PATH ?= clib
DECRYPT ?= 0

clean:
	rm -f a.out

build: clean
	gcc lab$(LAB_NUM)/main.c $(CLIB_PATH)/crypto.c -I$(CLIB_PATH) -lm -O2 -Wno-overflow -Wno-multichar -DDECRYPT=$(DECRYPT)

exact: build
	./a.out

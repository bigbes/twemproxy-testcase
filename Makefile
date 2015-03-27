all: 
	gcc -c tarantool-c/src/file.c tarantool-c/src/cksum.c \
		tarantool-c/src/session.c tarantool-c/src/console.c \
		tarantool-c/src/utf8.c tarantool-c/src/lex.c \
		tarantool-c/src/third_party/base64.c tarantool-c/src/tp.c \
		tarantool-c/src/third_party/sha1.c -O0 -ggdb3 --std=gnu99
	gcc test.c -o test -O0 -ggdb3 --std=c99\
		file.o cksum.o session.o console.o utf8.o lex.o base64.o sha1.o

.PHONY: all

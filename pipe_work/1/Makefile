.PHONY:all
all:processa processb

processa:test1.c
	gcc -o $@ $^
processb:test2.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f processa processb
test: test.o main.o
	gcc -o $@ $^
test.o: test.c
	gcc -c $^ -o $@
main.o: main.c
	gcc -c $^ -o $@
.PHONY:clean
clean:
	rm ./*.o test

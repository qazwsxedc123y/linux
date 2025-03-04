myLib=libcal.a
CC=gcc

$(myLib): add.o sub.o
	ar -rc -o $(myLib) $^

%.o: %.c
	$(CC) -c $<

.PHONY: clean
clean:
	rm -f $(myLib) ./*.o

.PHONY: output
output:
	mkdir -p mathlib/include
	mkdir -p mathlib/lib
	cp ./*.h mathlib/include
	cp ./*.a mathlib/lib
all: exec

main.o : main.c header.h 
	mkdir build
	gcc -c $< -o build/$@
exec: main.o
	gcc build/$^ -o build/$@
clean:
	rm -r build

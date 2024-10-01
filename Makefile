files = ex_5_14.c #$(wildcard *.cpp)
target := a.out

run: main
	#./$(target)

main: $(files)
	#clear
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)
clean:
	echo "Cleaning"
	rm -f *.out

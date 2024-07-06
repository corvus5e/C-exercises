files = ex_5_5.c #$(wildcard *.cpp)
target := a.out

run: main
	#./$(target)

main: $(files)
	#clear
	gcc --std=c17 -O0 -Wall -pg $(files) -lm -o $(target)
clean:
	echo "Cleaning"
	rm -f *.out

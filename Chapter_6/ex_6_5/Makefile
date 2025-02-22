files = ex_6_5.c
target = ex_6_5

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

#For MacOS leaks detection:
leakcheck:
	leaks --atExit -- ./$(target)

example:
	./$(target)

clean:
	echo "Cleaning"
	rm $(target)

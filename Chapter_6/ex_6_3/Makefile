files = ex_6_3.c
target = ex_6_3

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

#For MacOS leaks detection:
#leakcheck:
#	leaks --atExit -- ./$(target) < text.txt

example:
	./$(target) < text.txt

clean:
	echo "Cleaning"
	rm $(target)

files = ex_6_4.c
target = ex_6_4

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

files = ex_6_6.c
target = ex_6_6

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

#For MacOS leaks detection:
#leakcheck:
#	leaks --atExit -- ./$(target) < text.txt

example:
	./$(target) < sample_program.c

clean:
	echo "Cleaning"
	rm $(target)

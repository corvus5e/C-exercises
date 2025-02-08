files = ex_6_2.c
target = ex_6_2

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

example:
	./$(target) < sample_program.c

clean:
	echo "Cleaning"
	rm $(target)

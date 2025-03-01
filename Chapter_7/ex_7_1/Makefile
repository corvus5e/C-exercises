files = ex_7_1.c
target = upper

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -o $(target)

example:
	./$(target) < sample_program.c

clean:
	echo "Cleaning"
	rm $(target)

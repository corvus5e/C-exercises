files = ex_6_3.c
target = ex_6_3

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

example:
	./$(target) < text.txt

clean:
	echo "Cleaning"
	rm $(target)

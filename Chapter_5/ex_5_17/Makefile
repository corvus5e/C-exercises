files = *.c
target = sort

main: $(files)
	#gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)
	clang -std=c17 -g -pedantic-errors $(files) -o $(target)

example:
	./$(target) 0:20 -df 40:5 -n < orders.txt

clean:
	echo "Cleaning"
	rm $(target)

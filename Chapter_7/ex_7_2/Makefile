files = ex_7_2.c
target = ex_7_2

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -o $(target)

example:
	./$(target) < test.txt

clean:
	echo "Cleaning"
	rm $(target)

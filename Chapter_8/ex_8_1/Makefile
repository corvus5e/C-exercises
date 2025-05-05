files = ex_8_1.c
target = my_cat

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -o $(target)

example:
	./my_cat text.txt
clean:
	echo "Cleaning"
	rm $(target)

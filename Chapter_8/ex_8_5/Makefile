files = ex_8_5.c
target = fsize

main: $(files)
	gcc --std=c17 -O0 -Wall -g $(files) -o $(target)

example:
	$(target)
clean:
	echo "Cleaning"
	rm $(target)

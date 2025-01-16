files = *.c
target = dcl

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

clean:
	echo "Cleaning"
	rm $(target)

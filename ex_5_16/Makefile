files = *.c
target := a.out

main: $(files)
	gcc --std=c17 -O0 -Wall -ggdb $(files) -lm -o $(target)

clean:
	echo "Cleaning"
	rm -f *.out

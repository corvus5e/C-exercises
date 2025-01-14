#include <stdio.h>

int main()
{
	signed char x = -128;
	signed char neg_x = -1 * x;
	printf("bin = %d\nhex = %x\n", x, x);
	printf("bin = %d\nhex = %x", neg_x, neg_x);

}

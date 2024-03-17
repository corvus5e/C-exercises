
unsigned int setbits(unsigned int x, unsigned int pos, unsigned int n, unsigned int y);

int main()
{
	int x, y, p, n;

	int r = (x & ((~0 << (p + 1)) | (~(~0 << (p + 1 - n))))) | ((y & ~(~0 << n)) << (p + 1 - n));
}

unsigned int setbits(unsigned int x, unsigned int pos, unsigned int n, unsigned int y)
{
	unsigned int right_most_n_y_bits = y & ~(~0 << n);
	unsigned int filter = ~(~(~0 << n) << (pos - n + 1));
	x = x & filter;
	filter = filter | right_most_n_y_bits << (pos -n + 1);
	return x | filter;
}

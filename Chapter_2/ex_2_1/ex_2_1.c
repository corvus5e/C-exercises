#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>

int main()
{
	printf("short min = %+d\n", SHRT_MIN);
	printf("short max = %+d\n", SHRT_MAX);
	printf("float min = %e\n", FLT_MIN);
	printf("float max = %e\n", FLT_MAX);
	printf("bits in char %d\n", CHAR_BIT);

	float s = 0;
	printf("size of float %lu\n", sizeof(float));

	float f = 0xFFFFFFFF;
	printf("%f\n",f);
	unsigned char *c = (unsigned char*)(&f);
	for(int i = 0; i < sizeof(f); i++)
	{
		printf("%d\n", c[i]);
	}

}

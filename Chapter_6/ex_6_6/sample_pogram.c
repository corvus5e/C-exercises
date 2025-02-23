#include <stdio.h>

#define N 100
#define COUNT 5// This is 5
#define LOG /*This is comment*/printf("I was here!\n");
#define  /*Comment*/	   LOG2 	  printf( "And I was here" );     

int main()
{
	char* msg = "Hello, \"world!\"";
	printf("%d\n", COUNT);
	int a = 5 / 4*N;	// This is a
	int b=3/2;
	int c=3*COUNT/a;
	/*Comment
		section*/
	// And this is a comment
		// Another comment
	LOG
	LOG2
	printf("#END!\n");
	return 0;
}
// End of src


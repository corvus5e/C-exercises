#include <stdio.h>

#define N 100
#define COUNT 5// This is 5
#define LOG /*This is comment*/printf("I was here!\n");
#define  /*Comment*/	   LOG2 	  printf( "And I was here" );     
#define MSG "Hello from define\n"
#define LOOP /*Lets put code here:*/{for(int i = 0; i < 10/*Ten*/; ++i){printf("%d ", i);}}

static int arr[N];

#define N 7

int main()
{
	char* msg = "Hello, \"world!\"";
	printf("%d %s\n", COUNT, MSG);
	float a = 5.f / 4*N;	// This is a
	int b=3/2;
	int c=3*COUNT/a;
	/*Comment
		section*/
	// And this is a comment
		// Another comment
	LOG
	LOG2
	LOOP;

	printf("#END!\n");

	return 0;
}
// End of src



#include <stdio.h>
#include <stdbool.h> // we must import this library to use boolean in c

int foo (int bar) {
	return bar*2;
}
typedef struct {
	int x;
	int y;
} point;

static void print_point(point p) {
	printf("x: %d \n", p.x);
	printf("y: %d \n", p.y);
}

int main() {
	printf("Hello world");
	bool blah = true;
	if(blah)
	{
		printf("blah is true");
	}
	else
	{
		printf("blah is false");
	}
	int a = 0, b = 1, c = 2, d = 3, e = 4;
	a = b - c + d * e;
	printf("\n %d \n", a); /* will print 1-2+3*4 = 11 */
	
	int numbers[10];
	numbers[0] = 10;
	numbers[1] = 20;
	numbers[2] = 30;
	numbers[3] = 40;
	numbers[4] = 50;
	numbers[5] = 60;
	numbers[6] = 70;

	printf("The 7th number in the array is %d \n", numbers[6]);
	
	int grades[3][4] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2}; // initializes as the same as {{},{},{}}
	for(int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%d", grades[i][j]);
		}
		printf("\n");
	}
	
	printf("foo function %d \n", foo(2));
	point p;
	p.x = 15;
	p.y = 10;
	print_point(p);
}

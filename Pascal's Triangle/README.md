# Pascal's Triangle

Two different approaches to calculating values from Pascal's Triangle; one iterative, one recursive. Also reports the number of iterations or recursive calls required to reach a conclusion.

Full Code:
```c
// James Simmons
// 4/29/2019
// Calculating values from Pascal's Triangle
// Compare the runtime complexities of different approaches

#include <stdio.h>
#include <stdlib.h>

#define RECURSIVE_LIMIT 1000000000

// Global variable to count recursive calls and iterations
unsigned long long count = 0;

// Given values y and x, recursively calculates the value
// y rows down, x positions over, in pascal's triangle.
// Tracks the total recursive calls.
int pascal_recursive(int y, int x)
{
	count++;
	if (x <= 0 || x >= y || count > RECURSIVE_LIMIT )
		return 1;
	return pascal_recursive(y - 1, x) + pascal_recursive (y - 1, x - 1);
}

// Given values y and x, calculates the value
// y rows down, x positions over, in pascal's triangle
// by calculating every value in the rows [0, y].
// Tracks the total iterations.
int pascal_iterative(int y, int x)
{
	int i, j, answer, **triangle = malloc(sizeof(int *) * (y + 1));

	// Create a 2D array to store the triangle
	for (i = 0; i <= y; i++)
	{
		triangle[i] = malloc(sizeof(int) * (i + 1));
		count++;
	}

	// Fill the triangle with values
	for (i = 0; i <= y; i++)
	{
		for (j = 0; j <= i; j++)
		{
			if (j == 0 || j == i)
				triangle[i][j] = 1;
			else
				triangle[i][j] = triangle[i-1][j] + triangle[i-1][j-1];
			count++;
		}
	}

	// Store the answer
	answer = triangle[y][x];

	// Deallocate memory
	for (i = 0; i <= y; i++)
	{
		free(triangle[i]);
		count++;
	}
	free(triangle);

	return answer;
}


int main(int argc, char** argv)
{
	int x, y;
	unsigned long long result;

	if (argc < 3)
	{
		fprintf(stderr, "Two Non-Negative Integers Required\n");
		return 0;
	}

	y = atoi(argv[1]);
	x = atoi(argv[2]);

	if (x < 0 || y < 0)
	{
		fprintf(stderr, "Two Non-Negative Integers Required\n");
		return 0;
	}

	if (x > y)
	{
		fprintf(stderr, "Second Value Must Not Exceed First\n");
		return 0;
	}

	count = 0;
	result = pascal_recursive(y, x);
	if (count > RECURSIVE_LIMIT)
		printf("pascal_recursive(%d, %d) = too much recursion; aborting\n", y, x);
	else
		printf("pascal_recursive(%d, %d) = %llu with %llu recursive calls\n", y, x, result, count);

	fflush(stdout);

	count = 0;
	result = pascal_iterative(y, x);
	printf("pascal_iterative(%d, %d) = %llu with %llu iterations\n", y, x, result, count);

	return 0;
}

```

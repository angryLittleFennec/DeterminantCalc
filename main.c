#include "return_codes.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ERROR 10234567887654432

/*fills the matrix without a specified str and col*/
void get(float** a, int size, int col, float** temp_a)
{
	int di = 0, dj;
	for (int i = 1; i < size; i++)
	{
		dj = 0;
		for (int j = 0; j < size; j++)
		{
			if (j == col)
			{
				continue;
			}
			temp_a[di][dj] = a[i][j];
			dj++;
		}
		di++;
	}
}

/*calcs determinant*/
float det(float** a, int size)
{
	float d = 0;
	int c;
	float** temp_a = malloc(size * sizeof(float*));
	if (!temp_a)
	{
		printf("unable to allocate memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (int i = 0; i < size; i++)
	{
		temp_a[i] = malloc(size * sizeof(float));
		if (!temp_a[i])
		{
			for (int j = 0; j < i; j++)
			{
				free(temp_a[j]);
			}
			free(temp_a);
			printf("unable to allocate memory");
			return MEMORY_ERROR;
		}
	}
	if (size == 2)
	{
		return ((a[0][0] * a[1][1]) - (a[1][0] * a[0][1]));
	}
	else
	{
		for (c = 0; c < size; c++)
		{
			get(a, size, c, temp_a);
			d = d + (pow(-1, c) * a[0][c] * det(temp_a, size - 1));
		}
	}
	return d;
}

/*substitudes the value into the matrix*/
float substitution(int s, float** a, float* b, int size, float** M)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			M[i][j] = a[i][j];
	for (int i = 0; i < size; i++)
		M[i][s] = b[i];

	return det(M, size);
}

int main(int argc, char* argv[])
{
	int size;
	if (argc != 3)
	{
		printf("wrong number of parameters");
		return ERROR_INVALID_PARAMETER;
	}
	FILE* in = fopen(argv[1], "r");
	if (!in)
	{
		printf("you try to open the file that doesn't exist");
		return ERROR_FILE_NOT_FOUND;
	}
	FILE* out = fopen(argv[2], "w");
	if (!out)
	{
		printf("error while trying to open/create the file");
		fclose(in);
		return ERROR_FILE_NOT_FOUND;
	}

	fscanf(in, "%d", &size);

	float** a = malloc(size * sizeof(float*));
	if (!a)
	{
		fclose(in);
		fclose(out);
		printf("unable to allocate memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (int i = 0; i < size; i++)
	{
		a[i] = malloc(size * sizeof(float));
		if (!a[i])
		{
			for (int j = 0; j < i; j++)
			{
				free(a[j]);
			}
			free(a);
			fclose(in);
			fclose(out);
			printf("unable to allocate memory");
			return ERROR_NOT_ENOUGH_MEMORY;
		}
	}

	float* ans = malloc(size * sizeof(float));
	if (!ans)
	{
		for (int i = 0; i < size; i++)
		{
			free(a[i]);
		}
		free(a);
		fclose(in);
		fclose(out);
		printf("unable to allocate memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	float smallest = 0;
	int longest = 1;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fscanf(in, "%f", &a[i][j]);
			int c = 0;
			float s = a[i][j];
			if (s != 0)
			{
				while (fabs(s) - fabs((int)s) < 0.9 && fabs(s) - fabs((int)s) != 0)
				{
					c++;
					s *= 10;
				}
				if (c > longest)
					longest = c;
			}
		}
		fscanf(in, "%g", &ans[i]);
	}
	fclose(in);

	float determinant = det(a, size);
	if (determinant == MEMORY_ERROR)
	{
		for (int i = 0; i < size; i++)
			free(a[i]);
		free(a);
		free(ans);
		fclose(out);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	float** M = malloc(size * sizeof(float*));
	if (!M)
	{
		for (int i = 0; i < size; i++)
			free(a[i]);
		free(a);
		free(ans);
		fclose(out);
		printf("unable to allocate memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (int i = 0; i < size; i++)
	{
		M[i] = malloc(size * sizeof(float));
		if (!M[i])
		{
			for (int j = 0; j < i; j++)
				free(M[j]);
			for (int j = 0; j < size; j++)
				free(a[j]);
			free(M);
			free(a);
			free(ans);
			fclose(out);
			printf("unable to allocate memory");
			return ERROR_NOT_ENOUGH_MEMORY;
		}
	}
	float delta;
	if (longest > 1)
	{
		delta = pow(pow(0.2, longest), 6);
	}
	else
	{
		delta = 0.001;
	}

	if (determinant > -delta && determinant < delta)
	{
		int flag = 0;
		for (int i = 0; i < size; i++)
		{
			float sub = substitution(i, a, ans, size, M);
			if (sub == MEMORY_ERROR)
			{
				for (int j = 0; j < size; j++)
				{
					free(M[j]);
					free(a[j]);
				}
				free(M);
				free(a);
				free(ans);
				fclose(out);
				return ERROR_NOT_ENOUGH_MEMORY;
			}

			if (sub < 0 - delta || sub > 0 + delta)
				flag = 1;
		}
		if (flag == 0)
		{
			fprintf(out, "many solutions");
		}
		else
		{
			fprintf(out, "no solution");
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			float sub = substitution(i, a, ans, size, M);
			if (sub == MEMORY_ERROR)
			{
				for (int j = 0; j < size; j++)
				{
					free(M[j]);
					free(a[j]);
				}
				free(M);
				free(a);
				free(ans);
				fclose(out);
				return ERROR_NOT_ENOUGH_MEMORY;
			}
			fprintf(out, "%g\n", sub / determinant);
		}
	}

	for (int i = 0; i < size; i++)
	{
		free(M[i]);
		free(a[i]);
	}
	free(M);
	free(a);
	free(ans);
	fclose(out);
	return 0;
}

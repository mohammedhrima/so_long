#include <stdio.h>
#include <stdlib.h>

typedef struct variable
{
	int value;
	int i;
	int j;
	struct variable *top;
	struct variable *right;
	struct variable *left;
	struct variable *bottom;
} var;
int ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}
void *ft_calloc(size_t count, size_t size)
{
	void *arr;
	unsigned char *ptr;
	size_t i;
	if (count && size > SIZE_MAX / count)
		return (NULL);
	arr = (void *)malloc(count * size);
	if (!arr)
		return (NULL);
	i = 0;
	ptr = (unsigned char *)arr;
	while (i < count * size)
	{
		ptr[i] = 0;
		i++;
	}
	return (arr);
}
char	*ft_strdup(const char *s)
{
	char	*ptr;
	int		i;

	ptr = (char *)ft_calloc((ft_strlen(s) + 1), sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s && s[i])
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
char **arrdup(char *arr[])
{
	char **res;
	int i = 0;

	while (arr[i])
		i++;
	res = ft_calloc(i + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (arr && arr[i])
	{
		res[i] = ft_strdup(arr[i]);
		i++;
	}
	res[i] = NULL;
	return (res);
}

var *new_var(int value)
{
	var *new;

	new = (var *)malloc(sizeof(var));
	if (!new)
		return (NULL);
	new->value = value;
	new->top = NULL;
	new->right = NULL;
	new->bottom = NULL;
	new->left = NULL;
	return (new);
}

int main(void)
{
	char *arr[] = {
		"1111111111111111111",
		"1000000000000000001",
		"1000000000000000001",
		"1000000000P00000001",
		"1000000000000000001",
		"1000000000000000001",
		"1111111111111111111",
		NULL};

	int max_i = 6;
	int max_j = 18;

	var *Player;
	Player->i = 3;
	Player->j = 10;
	Player->value = arr[Player->i][Player->j];
	Player->top = NULL;
	Player->right = NULL;
	Player->left = NULL;
	Player->bottom = NULL;

	char **was_here = arrdup(arr);
	int i = 0;
	int j = 0;
	while (i < 7)
	{
		j = 0;
		while (j < 19)
		{
			printf("%c", arr[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("\n-> %c\n\n", Player->value);
	i = 0;
	j = 0;
	while (i < 7)
	{
		j = 0;
		while (j < 19)
		{
			printf("%c", was_here[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}

	int I;
	int J;
	int k = 0;
	while (k < 1)
	{
		I = Player->i;
		J = Player->j;
		if (was_here[I - 1][J] != 'P' && was_here[I - 1][J] != '1' && (I != 1 && J != 1)) // top
		{
			Player->top = new_var(was_here[I - 1][J]);
			Player->top->i = I - 1;
			Player->top->j = J;
			was_here[I - 1][J] = 'P';
		}
		if (was_here[I + 1][J] != 'P' && was_here[I + 1][J] != '1' && (I != 1 && J != 1)) // bottom
		{
			Player->bottom = new_var(was_here[I + 1][J]);
			Player->bottom->i = I + 1;
			Player->bottom->j = J;
			was_here[I + 1][J] = 'P';
		}
		if (was_here[I][J - 1] != 'P' && was_here[I][J - 1] != '1' && (I != 1 && J != 1)) // left
		{
			Player->left = new_var(was_here[I][J - 1]);
			Player->left->i = I;
			Player->left->j = J - 1;
			was_here[I][J - 1] = 'P';
		}
		if (was_here[I][J + 1] != 'P' && was_here[I][J + 1] != '1' && (I != 1 && J != 1)) // right
		{
			Player->left = new_var(was_here[I][J + 1]);
			Player->left->i = I;
			Player->left->j = J + 1;
			was_here[I][J + 1] = 'P';
		}
		k++;
	}
	printf("\nAfter while loop:\n");
	i = 0;
	j = 0;
	while (i < 7)
	{
		j = 0;
		while (j < 19)
		{
			printf("%c", was_here[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
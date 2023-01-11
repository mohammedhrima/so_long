#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
	int val;
	int pos;
	int prev;
} var;

var *new_var(int pos)
{
	var *x = malloc(sizeof(var));
	x->pos = pos;
	return (x);
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
void ft_putstr(char *s)
{
	int i;

	i = 0;
	while (s && s[i])
	{
		write(1, s + i, 1);
		i++;
	}
}

int main(void)
{
	char *arr[] = {
		"P0000000",
		"00000000",
		"00000000",
		"00000000",
		"0ABCDEF0",
		NULL};
	char *str = "P00000000000000000000000000000000ABCDEF0";

	int width = 8;
	int height = 5;
	int F = 34;
	int i = 0;

	var *queue = malloc((width * height + 2) * sizeof(var));
	int *visited = ft_calloc(height * width + 2, sizeof(int));

	int left = 0;
	int right = 0;

	// (0,0) -> (3,4)

	queue[left].pos = 0; // P
	queue[left].prev = 0;
	queue[left].val = 'P';

	// visited[left] = 'P';
	ft_putstr("\n========before while loop========\n");

	while (left < height * width)
	{
		// if (left / width + left % width == F)
		//  	printf("\nfound\n");
		left = right;
		int step = 0;
		var curr = queue[left];

		left++;
		right++;

		// visited[left] = curr.val;

		int x = curr.pos % width;
		int y = curr.pos / width;

		if (x < 0 || x >= width || y < 0 || y >= height)
			continue;

		if (x < width && !visited[y * width + x + 1] && arr[y][x + 1] == '0')
		{
			visited[y * width + x + 1] = 'P';
			queue[right] = *new_var(y * width + x + 1);
			right++;

			ft_putstr("right ");
		}
		if (x > 0 && !visited[y * width + x - 1] && arr[y][x - 1] == '0')
		{
			visited[y * width + x - 1] = 'P';

			queue[right] = *new_var(y * width + x - 1);
			right++;

			ft_putstr("left ");
		}
		if (y < height && !visited[(y + 1) * width + x] && arr[y + 1][x] == '0')
		{
			visited[(y + 1) * width + x] = 'P';

			queue[right] = *new_var((y + 1) * width + x);
			right++;

			ft_putstr("down ");
		}
		if (y > 0 && !visited[(y - 1) * width + x] && arr[y - 1][x] == '0')
		{
			visited[(y - 1) * width + x] = 'P';

			queue[right] = *new_var((y - 1) * width + x);
			right++;

			ft_putstr("up ");
		}
		// printf("did move\n");
	}
	ft_putstr("\n========after while loop========\n");
	i = 0;
	while (i < width * height)
	{
		if (i % width == 0)
			printf("\n");
		if (!visited[i])
			ft_putstr("0");
		else
			ft_putstr((char *)(visited + i));
		i++;
	}
}

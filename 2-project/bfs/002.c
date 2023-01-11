#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Libft/libft.h"

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

int main(void)
{
	char *arr[] = {
		"00000000",
		"00000000",
		"00000000",
		"00000000",
		"0ABCDEF0"}; // 39 character width:8 height:5
	char *str = "XXXX00000000000000EEER00000000000ABCDEF0";

	int width = 8;
	int height = 5;
	int F = 34;
	int i = 0;

	var *queue = ft_calloc(width * height + 1, sizeof(var));
	int *visited = ft_calloc(width * height + 1, sizeof(int));

	int left = 11;
	int right = left + 1;

	// (0,0) -> (3,4)

	queue[left].pos = left; // P
	// queue[left].prev = 0;

	// visited[queue[left].pos] = 'P';

	// visited[left] = 'P';
	ft_printf("\n========before while loop========\n");

	while (left < width * height)
	{
		var cur = queue[left];

		// if (right < left)
		// 	right = left;

		// int y = cur.pos / width;
		// int x = cur.pos - y * width;

		if (cur.pos >= width * height && cur.pos / width >= height )
			continue;
		else
		{
			if (!visited[cur.pos + 1] && str[cur.pos + 1] == '0')
			{
				ft_printf("\nR: str[%d]=%c (%d, %d) | ", cur.pos + 1, str[cur.pos + 1], cur.pos % width + 1, cur.pos / width);
				visited[cur.pos + 1] = 'P';
				queue[right] = *new_var(cur.pos + 1);
				right++;
				// ft_printf("right ");
			}
			if (cur.pos + width < width * height - 1 && !visited[cur.pos + width] && str[cur.pos + width] == '0')
			{
				ft_printf("D: str[%d]=%c (%d, %d) | ", cur.pos, str[cur.pos + width], cur.pos % width, cur.pos / width + 1);
				visited[cur.pos + width] = 'P';
				queue[right] = *new_var(cur.pos + width);
				right++;
				// ft_printf("down ");
			}
		}
		left++;
	}
	i = 0;
	int j = 0;
	ft_printf("\n\n");
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			ft_printf("%c", str[i * width + j]);
			j++;
		}
		ft_printf("\n");
		i++;
	}

	ft_printf("\n========after while loop========\n");
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (visited[i * width + j])
				ft_printf("%c", visited[i * width + j]);
			else
				ft_printf("0");
			j++;
		}
		ft_printf("\n");
		i++;
	}
	ft_printf("\n");
}

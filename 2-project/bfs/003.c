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
	x->val = 0;
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
	char *str = ft_strdup("XXXX00000000000000EEER00000000000ABCDEF0");

	int width = 8;
	int height = 5;
	int F = 34;
	int i = 0;

	var *queue = ft_calloc(width * height + 1, sizeof(var));
	int *visited = ft_calloc(width * height + 1, sizeof(int));

	int left = 10;
	int right = left + 1;

	queue[left].pos = left; // P
	queue[left].prev = left;
	queue[left].val = 'P';
	visited[queue[left].pos] = queue[left].val;

	ft_printf("\n========before while loop========\n");

	while (queue[left].val)
	{
		var cur = queue[left];
		int x = cur.pos % width;
		int y = cur.pos / width;
		if (cur.pos >= width * height || cur.pos / width >= height || x >= width)
			continue;
		else
		{
			ft_printf("\ncurr=%d (%d, %d) prev=%d (%d, %d): ", cur.pos, x, y, cur.prev, cur.prev % width, cur.prev / width);
			if (cur.pos % width + 1 < width && !visited[cur.pos + 1] && str[cur.pos + 1] == '0')
			{
				ft_printf("R=%d (%d, %d) | ", cur.pos + 1, (cur.pos + 1) % width, (cur.pos + 1) / width);
				queue[right] = *new_var(cur.pos + 1);
				queue[right].prev = cur.pos;
				queue[right].val = 'P';
				visited[cur.pos + 1] = 'P';
				right++;
			}
			if (cur.pos / width + 1 < height && !visited[cur.pos + width] && str[cur.pos + width] == '0')
			{
				ft_printf("D=%d (%d, %d) | ", cur.pos + width, (cur.pos + width) % width, (cur.pos + width) / width);
				queue[right] = *new_var(cur.pos + width);
				queue[right].prev = cur.pos;
				queue[right].val = 'P';
				visited[cur.pos + width] = 'P';
				right++;
			}
			if (cur.pos % width > 0 && !visited[cur.pos - 1] && str[cur.pos - 1] == '0')
			{
				ft_printf("R=%d (%d, %d) | ", cur.pos - 1, (cur.pos - 1) % width, (cur.pos - 1) / width);
				queue[right] = *new_var(cur.pos - 1);
				queue[right].prev = cur.pos;
				queue[right].val = 'P';
				visited[cur.pos - 1] = 'P';
				right++;
			}
			if (cur.pos / width > 0 && !visited[cur.pos - width] && str[cur.pos - width] == '0')
			{
				ft_printf("D=%d (%d, %d) | ", cur.pos - width, (cur.pos - width) % width, (cur.pos - width) / width);
				queue[right] = *new_var(cur.pos - width);
				queue[right].prev = cur.pos;
				queue[right].val = 'P';
				visited[cur.pos - width] = 'P';
				right++;
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
	ft_printf("%s\n\n", str);
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

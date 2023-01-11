#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Libft/libft.h"

typedef struct v
{
	int pos;
	char *path;
} var;

var *new_var(int pos)
{
	var *x = malloc(sizeof(var));
	x->pos = pos;
	x->path = NULL;
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
	char *str = ft_strdup("XXXX00000000000000EEE0000000000AFABC0EF0");

	int width = 8;
	int height = 5;
	int i = 0;

	var *queue = ft_calloc(width * height + 1, sizeof(var));
	int *visited = ft_calloc(width * height + 1, sizeof(int));

	int left = 10;
	int right = left + 1;

	queue[left].pos = left; // P
	str[left] = 'P';
	visited[left] = 'P';
	char *path;
	// var *winner;

	ft_printf("\n========before while loop========\n");
	while (left < right)
	{
		var cur = queue[left];
		if (cur.pos < width * height && cur.pos / width < height)
		{
			if (cur.pos % width + 1 < width && !visited[cur.pos + 1] && (str[cur.pos + 1] == '0' || str[cur.pos + 1] == 'C'))
			{
				queue[right] = *new_var(cur.pos + 1);
				queue[right].path = ft_strjoin(cur.path, "R");
				if (str[cur.pos + 1] == 'C')
				{
					ft_printf("found by %d from left\n", cur.pos);
					path = queue[right].path;
				}
				visited[cur.pos + 1] = 'P';
				right++;
			}
			if (cur.pos / width + 1 < height && !visited[cur.pos + width] && (str[cur.pos + width] == '0' || str[cur.pos + width] == 'C'))
			{
				queue[right] = *new_var(cur.pos + width);
				queue[right].path = ft_strjoin(cur.path, "D");
				if (str[cur.pos + width] == 'C')
				{
					ft_printf("found by %d from top\n", cur.pos);
					path = queue[right].path;
				}
				visited[cur.pos + width] = 'P';
				right++;
			}
			if (cur.pos % width > 0 && !visited[cur.pos - 1] && (str[cur.pos - 1] == '0' || str[cur.pos - 1] == 'C'))
			{
				queue[right] = *new_var(cur.pos - 1);
				queue[right].path = ft_strjoin(cur.path, "L");
				if (str[cur.pos - 1] == 'C')
				{
					ft_printf("found by %d from right\n", cur.pos);
					path = queue[right].path;
				}
				visited[cur.pos - 1] = 'P';
				right++;
			}
			if (cur.pos / width > 0 && !visited[cur.pos - width] && (str[cur.pos - width] == '0' || str[cur.pos - width] == 'C'))
			{
				queue[right] = *new_var(cur.pos - width);
				queue[right].path = ft_strjoin(cur.path, "U");
				if (str[cur.pos - width] == 'C')
				{
					ft_printf("found by %d from bottom\n", cur.pos);
					path = queue[right].path;
				}
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
	ft_printf("valid path: %s\n", path);
}

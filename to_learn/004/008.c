#include <mlx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define red 0xff0000
#define green 0x0000FF00
#define blue 0x000000FF
#define len 10
#define points_len 5
#define win_width 400;
#define win_height 250

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
	int snack_head[2];
	char *snack;
	int **points;
} t_var;

int draw_rect(t_var *var, int *x_start, int *y_start, int height, int width, int color)
{
	int h;
	int w;
	int x;
	int y;

	w = 0;
	while (w < width)
	{
		h = 0;
		while (h < height)
		{
			if (*x_start >= var->width)
				*x_start = *x_start - var->width;
			if (*x_start < 0)
				*x_start = var->width + *x_start;
			if (*y_start >= var->height)
				*y_start = *y_start - var->height;
			if (*y_start < 0)
				*y_start = var->height + *y_start;
			// printf("limit");
			mlx_pixel_put(var->mlx, var->win, *x_start + w, *y_start + h, color);
			h++;
		}
		w++;
	}
	return (0);
}

int draw_serpent(t_var *var)
{
	int x_zero = 0;
	int y_zero = 0;
	draw_rect(var, &x_zero, &y_zero, var->height, var->width, 0); // draw black rect
	int j = 0;
	while (j < points_len)
	{
		if (!(var->points[j][0] == 0 && var->points[j][1] == 0))
			draw_rect(var, &(var->points[j][0]), &(var->points[j][1]), len, len, green); // draw points
		j++;
	}
	if (var->snack_head[0] >= var->width)
		var->snack_head[0] = var->snack_head[0] - var->width;
	if (var->snack_head[0] < 0)
		var->snack_head[0] = var->width + var->snack_head[0];
	if (var->snack_head[1] >= var->height)
		var->snack_head[1] = var->snack_head[1] - var->height;
	if (var->snack_head[1] < 0)
		var->snack_head[1] = var->height + var->snack_head[1];
	// draw_rect(var, 0, 0, len, len, blue);
	int i = strlen(var->snack);
	int x;
	int y;
	x = var->snack_head[0] /*- len*/;
	y = var->snack_head[1];
	while (i > 0)
	{
		if (var->snack[i] == 'r')
			x -= len;
		if (var->snack[i] == 'l')
			x += len;
		if (var->snack[i] == 'u')
			y += len;
		if (var->snack[i] == 'd')
			y -= len;
		draw_rect(var, &x, &y, len, len, red);
		i--;
	}
	printf("(%d, %d)\n", var->snack_head[0], var->snack_head[1]);
	return (0);
}

char *increase_snack(char *snack)
{
	char to_add = snack[0];
	char *new_snack = malloc(strlen(snack) + 1);

	new_snack[0] = to_add;
	int i = 0;
	while (i < strlen(snack))
	{
		new_snack[i + 1] = snack[i];
		// printf("%d -> %c\n", i, snack[i]);
		i++;
	}
	new_snack[i + 1] = '\0';
	// printf("========================\n");
	free(snack);
	return new_snack;
}

int the_num(int num)
{
	int a = len;

	while (a + len <= num)
		a += len;
	return (a);
}

void generate_points(t_var *var)
{
	int i = 0;

	int fd = open("coordinate.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	while (i < points_len)
	{
		var->points[i][0] = the_num(rand() % var->width);
		var->points[i][1] = the_num(rand() % var->height);
		int j = i - 1;
		while (j > 0)
		{
			if (var->points[j][0] == var->points[i][0] || var->points[i][0] == 0)
			{
				while (var->points[j][0] == var->points[i][0] || var->points[i][0] == 0)
				{
					var->points[i][0] = the_num(rand() % var->width);
					// printf("w7alt 1\n");
				}
			}
			if (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
			{
				while (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
				{
					var->points[i][1] = the_num(rand() % var->height);
					// printf("w7alt 2\n");
				}
			}
			// printf("hana kan loopi\n");
			j--;
		}
		// write(fd, &(var->points[i][0]), sizeof(int));
		// write(fd, " ,", 3);
		// write(fd, &(var->points[i][1]), sizeof(int));
		// write(fd, "\n", 2);
		printf("%d. points(%d, %d)\n", i, var->points[i][0], var->points[i][1]);
		i++;
	}
	close(fd);
}

int all_points_are_eaten(int **points)
{
	int i = 0;
	while (i < points_len)
	{
		if (points[i][0] != 0)
			return (0);
		i++;
	}
	return (1);
}

int clicked_key(int keycode, t_var *var)
{
	static int x = 10;
	static int y = 10;
	int i;
	int x_zero = 0;
	int y_zero = 0;

	draw_rect(var, &x_zero, &y_zero, var->height, var->width, 0); // draw black rect
	if (keycode == 126)
	{
		//	write(1,"Up\n",4);
		var->snack_head[1] -= len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'u';
	}
	if (keycode == 125)
	{
		// write(1,"Down\n",6);
		var->snack_head[1] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'd';
	}
	if (keycode == 123)
	{
		// write(1,"Left\n",6);
		var->snack_head[0] -= len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'l';
	}
	if (keycode == 124)
	{
		//	write(1,"Right\n",7);
		var->snack_head[0] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'r';
	}
	//	printf("(%d, %d) -> (%d, %d)\n", var->snack_head[0], var->snack_head[1], var->points[0], var->points[1]);

	i = 0;
	while (i < points_len)
	{
		if (var->snack_head[0] == var->points[i][0] && var->snack_head[1] == var->points[i][1])
		{
			printf("find points %d from ", i);
			if (keycode == 126)
			{
				printf("126 down\n");
			}
			if (keycode == 125)
			{
				printf("125 top\n");
			}
			if (keycode == 124)
			{
				printf("124 left\n");
			}
			if (keycode == 123)
			{
				printf("123 right\n");
			}
			printf("old len %lu\n", strlen(var->snack));
			var->snack = increase_snack(var->snack);
			printf("new len %lu\n", strlen(var->snack));
			var->points[i][0] = 0;
			var->points[i][1] = 0;
		}
		i++;
	}
	draw_serpent(var);
	if (all_points_are_eaten(var->points))
		generate_points(var);
	// draw_rect(var, 400, 250, len, len, red);
	return (0);
}

int main(void)
{
	t_var var;

	var.width = win_width;
	var.height = win_height;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "Hello world!");
	var.points = malloc(points_len * sizeof(int *));
	int i = 0;
	while (i < points_len)
	{
		var.points[i] = malloc(2 * sizeof(int));
		i++;
	}
	generate_points(&var);
	var.snack = malloc(5);
	var.snack[0] = 'r';
	var.snack[1] = 'r';
	var.snack[2] = 'r';
	var.snack[3] = 'd';
	var.snack[4] = 'r';
	var.snack_head[0] = 80;
	var.snack_head[1] = 40;

	draw_serpent(&var);
	mlx_key_hook(var.win, clicked_key, &var);
	// mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}
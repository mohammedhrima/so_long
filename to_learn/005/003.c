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
#define win_width 400
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
	int keycode;
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
			mlx_pixel_put(var->mlx, var->win, *x_start + w, *y_start + h, color);
			h++;
		}
		w++;
	}
	return (0);
}

int draw_snack(t_var *var)
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
	int i = strlen(var->snack) - 1;
	int x;
	int y;
	x = var->snack_head[0];
	y = var->snack_head[1];
	draw_rect(var, &x, &y, len, len, red);
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
		if (x == var->snack_head[0] && y == var->snack_head[1])
		{
			printf("You did heat yourself!!\n");
			exit(-1);
		}
		draw_rect(var, &x, &y, len, len, red);
		i--;
	}
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
		i++;
	}
	new_snack[i + 1] = '\0';
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
					var->points[i][0] = the_num(rand() % var->width);
			}
			if (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
			{
				while (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
					var->points[i][1] = the_num(rand() % var->height);
			}
			j--;
		}
		i++;
	}
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
	printf("Good Job !!\nhere is more for you\n");
	return (1);
}

int clicked_key(int keycode, t_var *var)
{
	var->keycode = keycode;
	/*int i;
	int x_zero = 0;
	int y_zero = 0;

	draw_rect(var, &x_zero, &y_zero, var->height, var->width, 0); // draw black rect
	printf("%d\n",keycode);
	if (keycode == 126)
	{
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
		var->snack_head[0] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'r';
	}
	i = 0;
	while (i < points_len)
	{
		if (var->snack_head[0] == var->points[i][0] && var->snack_head[1] == var->points[i][1] && var->points[i][0] != 0 && var->points[i][1] != 0)
		{
			printf("hollaaa !! you heat point %d\n", i);
			var->snack = increase_snack(var->snack);
			var->points[i][0] = 0;
			var->points[i][1] = 0;
		}
		i++;
	}
	if (all_points_are_eaten(var->points))
		generate_points(var);
	draw_snack(var);*/
	return (0);
}

int draw_moving_snack(t_var *var)
{
	int i;
	int x_zero = 0;
	int y_zero = 0;

	draw_rect(var, &x_zero, &y_zero, var->height, var->width, 0); // draw black rect
	// printf("%d\n",keycode);
	if (var->keycode == 126)
	{
		var->snack_head[1] -= len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'u';
	}
	if (var->keycode == 125)
	{
		var->snack_head[1] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'd';
	}
	if (var->keycode == 123)
	{
		var->snack_head[0] -= len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'l';
	}
	if (var->keycode == 124)
	{
		var->snack_head[0] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'r';
	}
	i = 0;
	while (i < points_len)
	{
		if (var->snack_head[0] == var->points[i][0] && var->snack_head[1] == var->points[i][1] && var->points[i][0] != 0 && var->points[i][1] != 0)
		{
			printf("hollaaa !! you heat point %d\n", i);
			var->snack = increase_snack(var->snack);
			var->points[i][0] = 0;
			var->points[i][1] = 0;
		}
		i++;
	}
	if (all_points_are_eaten(var->points))
		generate_points(var);
	draw_snack(var);
	return (0);
}

int draw(t_var *var)
{
	//printf("hey 2\n");
	draw_moving_snack(var);
	// draw_snack(&var);
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
	var.keycode = 124;
	//printf("hey 1\n");
	// draw(var);

	mlx_loop_hook(var.mlx, draw, &var);
	mlx_key_hook(var.win, clicked_key, &var);
	mlx_loop(var.mlx);
}
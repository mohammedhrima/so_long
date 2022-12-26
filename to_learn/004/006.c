#include <mlx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define red 0xff0000
#define green 0x0000FF00
#define blue 0x000000FF
#define len 10

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
	int serp_head[2];
	char serp[5];
	int point[3][2];
} t_var;

int draw_rect(t_var *var, int x_start, int y_start, int height, int width, int color)
{
	int h;
	int w;

	w = 0;
	while (w < width)
	{
		h = 0;
		while (h < height)
		{
			mlx_pixel_put(var->mlx, var->win, x_start + w, y_start + h, color);
			h++;
		}
		w++;
	}
	return (0);
}

int draw_serpent(t_var *var)
{
	draw_rect(var, 0, 0, var->height, var->width, 0); // draw black rect
	int j = 0;
	while (j < 3)
	{
		draw_rect(var, var->point[j][0], var->point[j][1], len, len, green); // draw point
		j++;
	}

	// draw_rect(var, 0, 0, len, len, blue);
	int i = strlen(var->serp);
	int x;
	int y;
	x = var->serp_head[0] - len;
	y = var->serp_head[1];
	while (i > 0)
	{
		if (var->serp[i] == 'r')
			x -= len;
		if (var->serp[i] == 'l')
			x += len;
		if (var->serp[i] == 'u')
			y += len;
		if (var->serp[i] == 'd')
			y -= len;
		draw_rect(var, x, y, len, len, red);
		i--;
	}
	return (0);
}

int clicked_key(int keycode, t_var *var)
{
	static int x = 10;
	static int y = 10;
	int i;

	draw_rect(var, 0, 0, var->height, var->width, 0); // draw black rect
	if (keycode == 126)
	{
		//	write(1,"Up\n",4);
		var->serp_head[1] -= len;
		int i = 1;
		while (i < strlen(var->serp) - 1)
		{
			var->serp[i] = var->serp[i + 1];
			i++;
		}
		var->serp[i] = 'u';
	}
	if (keycode == 125)
	{
		// write(1,"Down\n",6);
		var->serp_head[1] += len;
		int i = 1;
		while (i < strlen(var->serp) - 1)
		{
			var->serp[i] = var->serp[i + 1];
			i++;
		}
		var->serp[i] = 'd';
	}
	if (keycode == 123)
	{
		// write(1,"Left\n",6);
		var->serp_head[0] -= len;
		int i = 1;
		while (i < strlen(var->serp) - 1)
		{
			var->serp[i] = var->serp[i + 1];
			i++;
		}
		var->serp[i] = 'l';
	}
	if (keycode == 124)
	{
		//	write(1,"Right\n",7);
		var->serp_head[0] += len;
		int i = 1;
		while (i < strlen(var->serp) - 1)
		{
			var->serp[i] = var->serp[i + 1];
			i++;
		}
		var->serp[i] = 'r';
	}
	//	printf("(%d, %d) -> (%d, %d)\n", var->serp_head[0], var->serp_head[1], var->point[0], var->point[1]);
	draw_serpent(var);
	i = 0;
	while (i < 3)
	{
		if (var->serp_head[0] - len == var->point[i][0] && var->serp_head[1] == var->point[i][1])
			printf("find point %d\n", i);
		i++;
	}
	// draw_rect(var, 400, 250, len, len, red);
	return (0);
}

int the_num(int num)
{
	int a = len;

	while (a + len <= num)
		a += len;
	return (a);
}

int main(void)
{
	t_var var;

	var.width = 400;
	var.height = 250;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "Hello world!");

	int i = 0;

	while (i < 3)
	{
		var.point[i][0] = the_num(rand() % var.width);
		var.point[i][1] = the_num(rand() % var.height);
		int j = i - 1;
		while (j > 0)
		{
			if (var.point[j][0] == var.point[i][0])
			{
				while (var.point[j][0] == var.point[i][0])
				{
					var.point[i][0] = the_num(rand() % var.width);
					printf("w7alt 1\n");
				}
			}
			if (var.point[j][1] == var.point[i][1])
			{
				while (var.point[j][1] == var.point[i][1])
				{
					var.point[i][1] = the_num(rand() % var.height);
					printf("w7alt 2\n");
				}
			}
			printf("hana kan loopi\n");
			j--;
		}
		printf("%d. point(%d, %d)\n", i, var.point[i][0], var.point[i][1]);
		i++;
	}

	var.serp[0] = 'r';
	var.serp[1] = 'r';
	var.serp[2] = 'r';
	var.serp[3] = 'd';
	var.serp[4] = 'r';

	var.serp_head[0] = 80;
	var.serp_head[1] = 40;

	draw_serpent(&var);
	mlx_key_hook(var.win, clicked_key, &var);
	// mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}
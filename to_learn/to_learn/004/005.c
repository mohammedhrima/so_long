#include <mlx.h>
#include <stdio.h>
#include <string.h>

#define red 0xff0000
#define len 10

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
	int serp_head[2];
	char serp[5];

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
	//	printf("draw\n");
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
		printf("Up\n");
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
		printf("Down\n");
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
		printf("Left\n");
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
		printf("Right\n");
		var->serp_head[0] += len;
		int i = 1;
		while (i < strlen(var->serp) - 1)
		{
			var->serp[i] = var->serp[i + 1];
			i++;
		}
		var->serp[i] = 'r';
	}
	draw_serpent(var);
	/*i = 0;
	printf("len -> %lu\n",strlen(var->serp));
	printf("[ ");
	while (i < strlen(var->serp))
	{
		printf("%c, ",var->serp[i]);
		i++;
	}
	printf("]\n--------------------------\n");*/
	return (0);
}

int main(void)
{
	t_var var;

	var.width = 800;
	var.height = 500;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "Hello world!");

	var.serp[0] = 'r';
	var.serp[1] = 'd';
	var.serp[2] = 'r';
	var.serp[3] = 'd';
	var.serp[4] = 'r';

	var.serp_head[0] = 80;
	var.serp_head[1] = 40;

	// draw_rect(&var, 10, 10, 50, 50, 0xff0000);
	draw_serpent(&var);
	mlx_key_hook(var.win, clicked_key, &var);
	// mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}
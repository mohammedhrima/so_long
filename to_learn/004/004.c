#include <mlx.h>
#include <stdio.h>
#define red 0xff0000

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
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

/*int draw(t_var *var)
{
	draw_rect(var, 0, 0, var->width, var->height, 0); // draw black rect
	draw_rect(var, 10, 10, 60, 60, 0xff0000);
	return (0);
}*/

int clicked_key(int keycode, t_var *var)
{
	static int x = 10;
	static int y = 10;
	int rec_height = 50;
	int rec_width = 50;

	// printf("key is %d\n", keycode);
	draw_rect(var, 0, 0, var->height, var->width, 0); // draw black rect
	if (keycode == 126)
	{
		printf("Up\n");
		y -= rec_height;
	}
	if (keycode == 125)
	{
		printf("Down\n");
		y += rec_height;
	}
	if (keycode == 123)
	{
		printf("Left\n");
		x -= rec_width;
	}
	if (keycode == 124)
	{
		printf("Right\n");
		x += rec_width;
	}
	draw_rect(var, x, y, rec_height, rec_width, red);
	return (0);
}

int main(void)
{
	t_var var;

	var.width = 800;
	var.height = 500;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "Hello world!");
	draw_rect(&var, 10, 10, 50, 50, red);
	mlx_key_hook(var.win, clicked_key, &var);
	// mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}
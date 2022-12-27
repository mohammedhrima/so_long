#include <mlx.h>

typedef struct s_img {
	int width;
	int height;
	unsigned int *pixels;
	void *ptr;
} t_img;

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
	int img_width;
	int img_height;
	unsigned int *draw_image;
	void *draw_image_ptr;
	void *img;
} t_var;

int draw_rect(t_var *var, int x_start, int y_start, int x_end, int y_end, int color)
{
	int x, y;

	y = y_start;
	while (y < y_end)
	{
		x = x_start;
		while (x < x_end)
		{
			var->draw_image[y * var->width + x] = color;
			x++;
		}
		y++;
	}
	return (0);
}



int loop(t_var *var)
{
	draw_rect(var, 100, 100, 200, 300, 0xffff);

	//draw_image(var, var->img, 0, 0, var->img_width, var->img_height);
	
	mlx_put_image_to_window(var->mlx, var->win, var->draw_image_ptr, 0, 0);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 20, 20);
	return 0;
}

int	main(void)
{
	t_var var;

	var.height = 400;
	var.width = 400;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "Hello world!");
	var.draw_image_ptr = mlx_new_image(var.mlx, var.width, var.height);
	int bits_per_pixel, size_line, endian;
	var.draw_image = (unsigned int *)mlx_get_data_addr(var.draw_image_ptr, 
									&bits_per_pixel, &size_line, &endian);

	var.img = mlx_xpm_file_to_image(var.mlx, "img/001.xpm", &var.img_width, &var.img_height);
	mlx_loop_hook(var.mlx, loop, &var);
	mlx_loop(var.mlx);
}
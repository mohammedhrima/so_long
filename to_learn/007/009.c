#include <mlx.h>
#include <stdio.h>

typedef struct s_img
{
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
	t_img img;
} t_var;

int draw_rect(t_var *var, int x_start, int y_start, int x_end, int y_end, unsigned int color)
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

void draw_image(t_var *var, t_img *img, int x_start, int y_start, int x_end, int y_end)
{
	int x, y;

	y = y_start;
	while (y < y_end)
	{
		x = x_start;
		while (x < x_end)
		{
			float fx = (float)(x - x_start) / (x_end - x_start);
			float fy = (float)(y - y_start) / (y_end - y_start);
			int img_x = fx * img->width;
			int img_y = fy * img->height;

			unsigned int color = img->pixels[img_y * img->width + img_x];
			var->draw_image[y * var->width + x] = color;
			x++;
		}
		y++;
	}
}

int loop(t_var *var)
{
	// draw_rect(var, 100, 100, 200, 300, 0xffff);

	// draw_image(var, &var->img, 0, 0, var->img.width, var->img.height);

	static int i = 0;
	i += 100;
	draw_rect(var, 0, 0, var->width, var->height, 0xffff);
	draw_image(var, &var->img, i, 0, i + var->img.width / 2, var->img.height / 2);
	// printf("%d\n", i);
	mlx_put_image_to_window(var->mlx, var->win, var->draw_image_ptr, 0, 0);
	// mlx_put_image_to_window(var->mlx, var->win, var->img, i, 0);

	return 0;
}

int main(void)
{
	t_var var;

	var.height = 270;
	var.width = 480;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "Hello world!");
	var.draw_image_ptr = mlx_new_image(var.mlx, var.width, var.height);
	int bits_per_pixel, size_line, endian;
	var.draw_image = (unsigned int *)mlx_get_data_addr(var.draw_image_ptr,
													   &bits_per_pixel, &size_line, &endian);

	var.img.ptr = mlx_xpm_file_to_image(var.mlx, "img/006.xpm", &var.img.width, &var.img.height);
	var.img.pixels = (unsigned int *)mlx_get_data_addr(var.img.ptr,
													   &bits_per_pixel, &size_line, &endian);
	mlx_loop_hook(var.mlx, loop, &var);
	printf("image width: %d\n", var.img.width);
	printf("image height %d\n", var.img.height);
	mlx_loop(var.mlx);
}
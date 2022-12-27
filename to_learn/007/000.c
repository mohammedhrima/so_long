#include <mlx.h>
#define red 0xff0000
#define green 0x0000FF00
#define blue 0x000000FF

typedef struct s_data
{
	void *mlx;
	void *win;
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_data;

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char *dst;
	// [11111111 1111111 1111111]
	int bytes_per_pixel = data->bits_per_pixel / 8;

	dst = data->addr + (y * data->line_length + x * bytes_per_pixel);
	/*
	- dst->addr points on first element in the array of pixels
	- bits_per_pixel = how much bits my color (pixel) contains
	- we devide data->bits_per_pixel by 8 to get how much bytes my color contains
	- data->addr is a 2D array points on first color (pixel), because image pixels are stored
	  as a 2D array
	- data->line_length >= width_image * bytes_per_pixel , //how much pixel in the width of image + (0 or some bytes)
	*/
}

int main(void)
{
	t_data var;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 500, 800, "my image");
	;
	var.img = mlx_new_image(var.mlx, 500, 800);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length, &var.endian);
	my_mlx_pixel_put(&var, 5, 5, red);
	mlx_put_image_to_window(var.mlx, var.win, var.img, 0, 0);
	mlx_loop(var.mlx);
}
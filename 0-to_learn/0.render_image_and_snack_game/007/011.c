#include <mlx.h>
#include <stdio.h>
#define red 0xff0000
#define green 0x0000FF00
#define blue0 0x000000FF
#define blue1 0xffff
#define right 124
#define down 125
#define left 123
#define up 126

typedef struct s_img
{
	int width;
	int height;
	unsigned int *pixels;
	int bits_per_pixel;
	int line_length;
	int endian;
	void *ptr;
} t_img;

typedef struct s_var
{
	void *mlx;
	void *win;
	int win_height;
	int win_width;
	int bits_per_pixel;
	int line_length;
	int endian;
	unsigned int *img_arr;
	void *img_ptr;
	int keycode;
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
			var->img_arr[y * var->win_width + x] = color;
			x++;
		}
		y++;
	}
	return (0);
}

void my_mlx_pixel_put(t_var *data, int x, int y, int color)
{
	char *dst;
	int bytes_per_pixel = data->bits_per_pixel / 8;

	dst = (char *)(data->img_arr) + (y * data->line_length + x * bytes_per_pixel);
	/*- dst->addr points on first element in the array of pixels
	- bits_per_pixel = how much bits my color (pixel) contains
	- we devide data->bits_per_pixel by 8 to get how much bytes my color contains
	- data->addr is a 2D array points on first color (pixel), because image pixels are stored
	  as a 2D array
	- data->line_length >= width_image * bytes_per_pixel , //how much pixel in the width of image + (0 or some bytes)
	*/
}
/*
void put_pixel(t_var *var, t_img *img, int x, int y)
{
	float fx = (float)(x - min_x) / width;
	float fy = (float)(y - min_y) / height;
}*/

void draw_image(t_var *var, t_img *img, int x_start, int y_start, /*int x_end, int y_end,*/ int scale) // change scale to float
{

	int width = var->img.width / scale;
	int height = var->img.height / scale;

	int min_x = x_start;
	int min_y = y_start;
	int x_end = x_start + width;

	/*if(y_start )*/

	/*if (x_end > var->win_width)
		x_end = var->win_width;*/

	int x, y;
	y = y_start;

	while (y < y_start + height)
	{
		x = x_start;	
		while (x < x_end)
		{
			float fx = (float)(x - min_x) / width;
			float fy = (float)(y - min_y) / height;

			int img_x = fx * img->width; // because we will take pixels from img
			int img_y = fy * img->height; // because we will take pixels from img

			int index1 = img_y * img->width + img_x;
			//if();
			int modulo_y = y % var->win_height;
			int modulo_x = x % var->win_width;
			if(y < 0)
				modulo_y = y + var->win_height * ((-y + var->win_height - 1) / var->win_height);
			if (x < 0)
				modulo_x = x + var->win_width * ((-x + var->win_width - 1) / var->win_width);
			int index2 = modulo_y * var->win_width + modulo_x;
			//if(index2 > var->line_length * var->bits_per_pixel)
			//	printf("out\n");
			var->img_arr[index2] = img->pixels[index1];

		//	printf("%d, line lenght: %d, bits per pixel: %d, index2: %d\n", x, var->line_length, var->bits_per_pixel, index2);
			x++;
		}
		y++;
	}
}

int clicked_key(int keycode, t_var *var)
{
	var->keycode = keycode;
	printf("%d\n",keycode);
	//right 124
	//down 125
	//left 123
	//up 126

	return (0);
}

int loop(t_var *var)
{
	static int x = 0;
	static int y = 0;
	int i = 5;
	if(var->keycode == right)
		x += i;
	if(var->keycode == left)
		x -= i;
	if(var->keycode == up)
		y -= i;
	if(var->keycode == down)
		y += i;
	draw_rect(var, 0, 0, var->win_width, var->win_height, blue1); // background
	
	draw_image(var, &var->img, x, y, 2); // we draw the_made_image where we will put the image

	mlx_put_image_to_window(var->mlx, var->win, var->img_ptr, 0, 0);
	// printf("%d. line lenght: %d, bits per pixel: %d, endian: %d\n",i, var->line_length, var->bits_per_pixel, var->endian);
	//i += 2;
	return 0;
}

int main(void)
{
	t_var var;

	var.win_height = 100;
	var.win_width = 200;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.win_width, var.win_height, "my image");
	var.keycode = 124; //right

	// rectangle where we will put the image
	var.img_ptr = mlx_new_image(var.mlx, var.win_width, var.win_height);
	var.img_arr = (unsigned int *)mlx_get_data_addr(var.img_ptr, &var.bits_per_pixel, &var.line_length, &var.endian);

	// put image in rectangle
	var.img.ptr = mlx_xpm_file_to_image(var.mlx, "img/006.xpm", &var.img.width, &var.img.height);
	var.img.pixels = (unsigned int *)mlx_get_data_addr(var.img.ptr, &var.img.bits_per_pixel, &var.img.line_length, &var.img.endian);

	mlx_loop_hook(var.mlx, loop, &var);
	mlx_key_hook(var.win, clicked_key, &var); //use mlx_hook check documentation
	printf("image width: %d\n", var.img.width);
	printf("image height %d\n", var.img.height);
	mlx_loop(var.mlx);
}
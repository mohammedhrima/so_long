#include <mlx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

// COLORS
#define RED 0xff0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define BLUE1 0xffff

// SETTING
#define LEN 10
#define POINTS_LEN 4
#define SCALE 2

// TIMING
#define TIME_TO_SLEEP 150
#define INCREASE_SPEED 50

// WINDOW SETTING
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 250

// KEYS
#define RIGHT 124
#define DOWN 125
#define LEFT 123
#define UP 126

typedef struct s_img
{
	char *name;
	void *mlx;
	void *win;
	void *ptr;
	/////////////
	int width;
	int height;
	/////////////
	unsigned int *pixels;
	int bits_per_pixel;
	/////////////
	int line_length;
	int endian;
	/////////////
	int keycode;
	////////////
} t_img;

typedef struct s_var
{
	char *name;
	void *mlx;
	void *win;
	void *ptr;
	/////////////
	int width;
	int height;
	/////////////
	unsigned int *pixels;
	int bits_per_pixel;
	/////////////
	int line_length;
	int endian;
	/////////////
	int keycode;
	////////////
	t_img img;
} t_var;

/*int draw_rect(t_var *var, int *x_start, int *y_start, int height, int width, int color)
{
	int h;
	int w;

	w = 0;
	while (w < width)
	{
		h = 0;
		while (h < height)
		{
			if (*x_start >= WINDOW_WIDTH)
				*x_start = *x_start - WINDOW_WIDTH;
			if (*x_start < 0)
				*x_start = WINDOW_WIDTH + *x_start;
			if (*y_start >= WINDOW_HEIGHT)
				*y_start = *y_start - WINDOW_HEIGHT;
			if (*y_start < 0)
				*y_start = WINDOW_HEIGHT + *y_start;
			var->pixels[(*y_start + h) * WINDOW_WIDTH + (*x_start + w)] = color;
			h++;
		}
		w++;
	}
	return (0);
}*/
int draw_rect(t_var *var, int x_start, int y_start, int x_end, int y_end, unsigned int color)
{
	int x, y;

	y = y_start;
	while (y < y_end)
	{
		x = x_start;
		while (x < x_end)
		{
			var->pixels[y * WINDOW_WIDTH + x] = color;
			x++;
		}
		y++;
	}
	return (0);
}

void draw_image(t_var *to, t_img *from, int x_start, int y_start, float scale) // change scale to float
{
	int width = from->width * scale;
	int height = from->height * scale;
	int min_x = x_start;
	int min_y = y_start;
	int x_end = x_start + width;
	int x, y;
	y = y_start;

	while (y < y_start + height)
	{
		x = x_start;
		while (x < x_end)
		{
			float fx = (float)(x - min_x) / width;
			float fy = (float)(y - min_y) / height;

			int img_x = fx * from->width;  // because we will take pixels from img
			int img_y = fy * from->height; // because we will take pixels from img

			int index1 = img_y * from->width + img_x;
			int modulo_y = y % WINDOW_HEIGHT;
			int modulo_x = x % WINDOW_WIDTH;

			while (modulo_y < 0)
				modulo_y += WINDOW_HEIGHT;
			while (modulo_x < 0)
				modulo_x += WINDOW_WIDTH;

			int index2 = modulo_y * WINDOW_WIDTH + modulo_x;
			unsigned int alpha = (from->pixels[index1] >> 24) & 0xFF;
			unsigned int r = alpha;
			unsigned int b = alpha;
			unsigned int g = alpha;
			unsigned int color2 = (r << 16) | (g << 8) | b; // r, g, b, a

			if (!alpha) // linear interpolation  // A B  t (0 -> 1), lerp(a, t, b) = (a + b) * t
				to->pixels[index2] = color2;
			x++;
		}
		y++;
	}
}

int clicked_key(int keycode, t_var *var)
{
	var->keycode = keycode;
	return (0);
}

static int x = 0;
int draw(t_var *var)
{
	
	static int y = 0;

	draw_rect(var, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BLUE1); // background
	draw_image(var, &var->img, x, y, SCALE);				 // we draw the_made_image where we will put the image
	
	mlx_put_image_to_window(var->mlx, var->win, var->ptr, 0, 0);
	return (0);
}

int put(int key, t_var *var)
{
	x += 50;

}

int main(void)
{
	t_var var;

	var.name = "rectangle";
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Letters");

	// rectangle where we will put the image
	var.ptr = mlx_new_image(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	var.pixels = (unsigned int *)mlx_get_data_addr(var.ptr, &var.bits_per_pixel, &var.line_length, &var.endian);
	printf("Rectangle:\n  Height: %d,  Width: %d\n", var.height, var.width);

	// put image in rectangle
	var.img.name = "image";
	var.img.ptr = mlx_xpm_file_to_image(var.mlx, "A.xpm", &var.img.width, &var.img.height);
	var.img.pixels = (unsigned int *)mlx_get_data_addr(var.img.ptr, &var.img.bits_per_pixel, &var.img.line_length, &var.img.endian);
	printf("Image:\n  Height: %d,  Width: %d\n", var.img.height, var.img.width);

	mlx_loop_hook(var.mlx, draw, &var);
	mlx_hook(var.win, 2,0, put, &var);
	mlx_loop(var.mlx);
}
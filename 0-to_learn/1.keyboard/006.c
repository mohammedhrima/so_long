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
#define SCALE 1

// TIMING
#define TIME_TO_SLEEP 150
#define INCREASE_SPEED 50

// WINDOW SETTING
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

// KEYS
#define RIGHT 124
#define DOWN 125
#define LEFT 123
#define UP 126

#define KEY_BOARD_LEN 500

// ALPHABET
#define A 0
#define B 11
#define C 8
#define D 2
#define E 14
#define F 3
#define G 5
#define H 4
#define I 34
#define J 38
#define K 40
#define L 37
#define M 46
#define N 45
#define O 31
#define P 35
#define Q 12
#define R 15
#define S 1
#define T 17
#define U 32
#define V 9
#define W 13
#define X 7
#define Y 16
#define Z 6
#define NEW_LINE 36
#define DELETE 51

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
	char *filename;
} t_img;

typedef struct s_var
{
	char *name;
	void *mlx;
	void *win;
	void *ptr;
	/////////////
	char **filesname;
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

int draw_rect(t_var *var, int x_start, int y_start, int x_end, int y_end, unsigned int color)
{
	int x0, y0;

	y0 = y_start;
	while (y0 < y_end)
	{
		x0 = x_start;
		while (x0 < x_end)
		{
			var->pixels[y0 * WINDOW_WIDTH + x0] = color;
			x0++;
		}
		y0++;
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
	int x0, y0;
	y0 = y_start;

	while (y0 < y_start + height)
	{
		x0 = x_start;
		while (x0 < x_end)
		{
			float fx = (float)(x0 - min_x) / width;
			float fy = (float)(y0 - min_y) / height;

			int img_x = fx * from->width;  // because we will take pixels from img
			int img_y = fy * from->height; // because we will take pixels from img

			int index1 = img_y * from->width + img_x;
			int modulo_y = y0 % WINDOW_HEIGHT;
			int modulo_x = x0 % WINDOW_WIDTH;

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
			x0++;
		}
		y0++;
	}
}

int clicked_key(int keycode, t_var *var)
{
	var->keycode = keycode;
	return (0);
}

static int x0 = 0;
static int y0 = 0;
int draw(t_var *var)
{
	// draw_rect(var, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BLUE1); // background
	if (var->img.ptr && var->img.pixels)
		draw_image(var, &var->img, x0, y0, SCALE); // we draw the_made_image where we will put the image

	mlx_put_image_to_window(var->mlx, var->win, var->ptr, 0, 0);
	return (0);
}

int put(int key, t_var *var)
{
	if(y0 < 0)
		y0 == 0;
	printf("%d\n", key);
	if (key == NEW_LINE)
	{
		x0 = 2 * SCALE;
		y0 += (var->img.height + 2) * SCALE;
		free(var->img.ptr);
		free(var->img.pixels);
		var->img.ptr = NULL;
		var->img.pixels = NULL;
	}
	if (key == DELETE)
	{
		printf("DELETE x=%d, y=%d\n", x0, y0);
		if (x0 != 0 || y0 != 0)
		{
			draw_rect(var, x0, y0, x0 + var->img.width, y0 + var->img.height, RED);
			free(var->img.ptr);
			free(var->img.pixels);
			var->img.ptr = NULL;
			var->img.pixels = NULL;
			x0 = x0 % WINDOW_WIDTH - var->img.width;

			if (x0 <= 2)
			{
				y0 -= (var->img.height + 2) * SCALE;
				x0 = WINDOW_WIDTH - var->img.width * SCALE;
			}
		}
	}
	else if (var->filesname[key])
	{
		x0 += (var->img.width + 2) * SCALE;
		if (x0 > WINDOW_WIDTH - var->img.height)
		{
			y0 += (var->img.height + 2) * SCALE;
			x0 = 0;
		}
		
		// printf("%d\n", key);
		free(var->img.ptr);
		free(var->img.pixels);
		var->img.ptr = mlx_xpm_file_to_image(var->mlx, var->filesname[key], &var->img.width, &var->img.height);
		var->img.pixels = (unsigned int *)mlx_get_data_addr(var->img.ptr, &var->img.bits_per_pixel, &var->img.line_length, &var->img.endian);
		draw(var);
	}
	return (0);
}

int main(void)
{
	t_var var;

	var.name = "rectangle";
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Letters");
	var.filesname = (char **)malloc(KEY_BOARD_LEN * sizeof(char *));
	int i = 0;
	while (i < KEY_BOARD_LEN)
	{
		var.filesname[i] = NULL;
		i++;
	}
	var.filesname[A] = "alphabet/A.xpm";
	var.filesname[B] = "alphabet/B.xpm";
	var.filesname[C] = "alphabet/C.xpm";
	var.filesname[D] = "alphabet/D.xpm";
	var.filesname[E] = "alphabet/E.xpm";
	var.filesname[F] = "alphabet/F.xpm";
	var.filesname[H] = "alphabet/H.xpm";
	var.filesname[I] = "alphabet/I.xpm";
	var.filesname[J] = "alphabet/J.xpm";
	var.filesname[K] = "alphabet/K.xpm";
	var.filesname[L] = "alphabet/L.xpm";
	var.filesname[M] = "alphabet/M.xpm";
	var.filesname[N] = "alphabet/N.xpm";
	var.filesname[O] = "alphabet/O.xpm";
	var.filesname[P] = "alphabet/P.xpm";
	var.filesname[Q] = "alphabet/Q.xpm";
	var.filesname[R] = "alphabet/R.xpm";
	var.filesname[S] = "alphabet/S.xpm";
	var.filesname[T] = "alphabet/T.xpm";
	var.filesname[U] = "alphabet/U.xpm";
	var.filesname[V] = "alphabet/V.xpm";
	var.filesname[W] = "alphabet/W.xpm";
	var.filesname[X] = "alphabet/X.xpm";
	var.filesname[Y] = "alphabet/Y.xpm";
	var.filesname[Z] = "alphabet/Z.xpm";

	// rectangle where we will put the image
	var.ptr = mlx_new_image(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	var.pixels = (unsigned int *)mlx_get_data_addr(var.ptr, &var.bits_per_pixel, &var.line_length, &var.endian);
	printf("Rectangle:\n  Height: %d,  Width: %d\n", var.height, var.width);

	// put image in rectangle
	// var.img.name = "image";
	// var.img.ptr = mlx_xpm_file_to_image(var.mlx, var.filesname[0], &var.img.width, &var.img.height);
	// var.img.pixels = (unsigned int *)mlx_get_data_addr(var.img.ptr, &var.img.bits_per_pixel, &var.img.line_length, &var.img.endian);
	// printf("Image:\n  Height: %d,  Width: %d\n", var.img.height, var.img.width);

	draw_rect(&var, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BLUE1); // background
	mlx_loop_hook(var.mlx, draw, &var);
	mlx_hook(var.win, 2, 0, put, &var);
	mlx_loop(var.mlx);
}
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
#define increase_speed 4;
#define the_scale 2

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
	int speed;
	int isclick;
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

void draw_image(t_var *var, t_img *img, int x_start, int y_start, float scale) // change scale to float
{
	int width = var->img.width * scale;
	int height = var->img.height * scale;
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

			int img_x = fx * img->width;  // because we will take pixels from img
			int img_y = fy * img->height; // because we will take pixels from img

			int index1 = img_y * img->width + img_x;
			int modulo_y = y % var->win_height;
			int modulo_x = x % var->win_width;

			while (modulo_y < 0)
				modulo_y += var->win_height;
			while (modulo_x < 0)
				modulo_x += var->win_width;

			int index2 = modulo_y * var->win_width + modulo_x;
			unsigned int alpha = (img->pixels[index1] >> 24) & 0xFF; // [00000010] [001000000] [000000010] [010000000]
																	//									   [111111111]
																	// 

			unsigned int r = alpha;
			unsigned int b = alpha;
			unsigned int g = alpha;
			unsigned int color2 = (r << 16) | (g << 8) | b; // r, g, b, a
			//printf("%d\n", alpha);

			//printf("%d %d %d %d\n", img->pixels[index1] & 0xFF, (img->pixels[index1] >> 8) & 0xFF, (img->pixels[index1] >> 16) & 0xFF, (img->pixels[index1] >> 24) & 0xFF);
			if (!alpha) // linear interpolation  // A B  t (0 -> 1), lerp(a, t, b) = (a + b) * t
				var->img_arr[index2] = color2;
			x++;
		}
		y++;
	}
}

int speed_up(int keycode, t_var *var)
{
	static int i = 0;
	// printf("%d. keycode: %d\n", i, keycode);
	i++;
	static int speed = 0;

	var->keycode = keycode;
	if (keycode == right || keycode == left || keycode == up || keycode == down)
		speed += 1;

	var->speed = speed;
	return (0);
}

int draw(t_var *var)
{
	static int x = 0;
	static int y = 0;
	static int j = 0;
	static int old_speed = 0;
	static int old_keycode = right;
	int i = 2;

	draw_rect(var, 0, 0, var->win_width, var->win_height, blue1); // background
	draw_image(var, &var->img, x, y, the_scale);						  // we draw the_made_image where we will put the image
	mlx_put_image_to_window(var->mlx, var->win, var->img_ptr, 0, 0);
	old_speed = var->speed;
	old_keycode = var->keycode;
	return 0;
}

int main(void)
{
	t_var var;

	var.win_height = 600;
	var.win_width = 1000;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.win_width, var.win_height, "my image");
	var.keycode = right; // right
	var.speed = 0;

	// rectangle where we will put the image
	var.img_ptr = mlx_new_image(var.mlx, var.win_width, var.win_height);
	var.img_arr = (unsigned int *)mlx_get_data_addr(var.img_ptr, &var.bits_per_pixel, &var.line_length, &var.endian);

	// put image in rectangle
	var.img.ptr = mlx_xpm_file_to_image(var.mlx, "A1.xpm", &var.img.width, &var.img.height);
	var.img.pixels = (unsigned int *)mlx_get_data_addr(var.img.ptr, &var.img.bits_per_pixel, &var.img.line_length, &var.img.endian);
	printf("Height : %d, Width: %d, Bits per pixel: %d\n", var.img.height, var.img.width, var.img.bits_per_pixel);

	mlx_loop_hook(var.mlx, draw, &var);
	mlx_hook(var.win, 2, 0, speed_up, &var);
	mlx_loop(var.mlx);
};
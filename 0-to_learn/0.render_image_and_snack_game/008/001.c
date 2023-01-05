#include <mlx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define red 0xff0000
#define green 0x0000FF00
#define blue 0x000000FF
#define len 10
#define points_len 4
#define win_width 400
#define win_height 250
#define right 124
#define down 125
#define left 123
#define up 126
#define increase_speed 2;

typedef struct s_var
{
	void *mlx;
	void *win;
	int height;
	int width;
	int snack_head[2];
	char *snack;
	int **points;
	int keycode;
	void *img_ptr;
	float time_var;
	int speed;
	int bits_per_pixel;
	int line_length;
	int endian;
	unsigned int *draw_image;
} t_var;

int draw_rect(t_var *var, int *x_start, int *y_start, int height, int width, int color)
{
	int h;
	int w;
	int x;
	int y;

	w = 0;
	while (w < width)
	{
		h = 0;
		while (h < height)
		{
			while (*x_start >= var->width)
				*x_start = *x_start - var->width;
			while (*x_start < 0)
				*x_start = var->width + *x_start;
			while (*y_start >= var->height)
				*y_start = *y_start - var->height;
			while (*y_start < 0)
				*y_start = var->height + *y_start;
			// mlx_pixel_put(var->mlx, var->win, *x_start + w, *y_start + h, color);
			var->draw_image[(*y_start + h) * var->width + (*x_start + w)] = color;
			h++;
		}
		w++;
	}
	return (0);
}

int draw_snack(t_var *var)
{
	int x_zero = 0;
	int y_zero = 0;
	draw_rect(var, &x_zero, &y_zero, var->height, var->width, 0); // draw black rect
	int j = 0;
	while (j < points_len)
	{
		if (!(var->points[j][0] == 0 && var->points[j][1] == 0))
			draw_rect(var, &(var->points[j][0]), &(var->points[j][1]), len, len, green); // draw points
		j++;
	}
	if (var->snack_head[0] >= var->width)
		var->snack_head[0] = var->snack_head[0] - var->width;
	if (var->snack_head[0] < 0)
		var->snack_head[0] = var->width + var->snack_head[0];
	if (var->snack_head[1] >= var->height)
		var->snack_head[1] = var->snack_head[1] - var->height;
	if (var->snack_head[1] < 0)
		var->snack_head[1] = var->height + var->snack_head[1];
	int i = strlen(var->snack) - 1;
	int x;
	int y;
	x = var->snack_head[0];
	y = var->snack_head[1];
	draw_rect(var, &x, &y, len, len, red);
	while (i > 0)
	{
		if (var->snack[i] == 'r')
			x -= len;
		if (var->snack[i] == 'l')
			x += len;
		if (var->snack[i] == 'u')
			y += len;
		if (var->snack[i] == 'd')
			y -= len;
		if (x == var->snack_head[0] && y == var->snack_head[1])
		{
			printf("You did heat yourself!!\n");
			exit(-1);
		}
		draw_rect(var, &x, &y, len, len, red);
		i--;
	}
	return (0);
}

char *increase_snack(char *snack)
{
	char to_add = snack[0];
	char *new_snack = malloc(strlen(snack) + 2);

	new_snack[0] = to_add;
	int i = 0;
	while (snack[i])
	{
		new_snack[i + 1] = snack[i];
		i++;
	}
	new_snack[i + 1] = '\0';
	// free(snack);
	return new_snack;
}

int the_num(int num)
{
	int a = len;

	while (a + len <= num)
		a += len;
	return (a);
}

void generate_points(t_var *var)
{
	int i = 0;

	while (i < points_len)
	{
		var->points[i][0] = the_num(rand() % var->width);
		var->points[i][1] = the_num(rand() % var->height);
		int j = i - 1;
		while (j > 0)
		{
			if (var->points[j][0] == var->points[i][0] || var->points[i][0] == 0)
			{
				while (var->points[j][0] == var->points[i][0] || var->points[i][0] == 0)
					var->points[i][0] = the_num(rand() % var->width);
			}
			if (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
			{
				while (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
					var->points[i][1] = the_num(rand() % var->height);
			}
			j--;
		}
		i++;
	}
}

int all_points_are_eaten(int **points)
{
	int i = 0;
	while (i < points_len)
	{
		if (points[i][0] != 0)
			return (0);
		i++;
	}
	printf("Good Job !!\nhere is more for you\n");
	return (1);
}

int clicked_key(int keycode, t_var *var)
{
	var->keycode = keycode;
	return (0);
}

int draw_moving_snack(t_var *var)
{
	int i;

	if (var->keycode == 126)
	{
		var->snack_head[1] -= len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'u';
	}
	if (var->keycode == 125)
	{
		var->snack_head[1] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'd';
	}
	if (var->keycode == 123)
	{
		var->snack_head[0] -= len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'l';
	}
	if (var->keycode == 124)
	{
		var->snack_head[0] += len;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'r';
	}
	i = 0;
	while (i < points_len)
	{
		if (var->snack_head[0] == var->points[i][0] && var->snack_head[1] == var->points[i][1] && var->points[i][0] != 0 && var->points[i][1] != 0)
		{
			printf("hollaaa !! you heat point %d\n", i);
			char *tmp;
			tmp = increase_snack(var->snack);
			free(var->snack);
			var->snack = tmp;
			var->points[i][0] = 0;
			var->points[i][1] = 0;
		}
		i++;
	}
	if (all_points_are_eaten(var->points))
	{
		generate_points(var);
		var->time_var -= 5;
	}
	draw_snack(var);
	return (0);
}

int mssleep(long miliseconds)
{
	struct timespec rem;
	struct timespec req = {
		(int)(miliseconds / 1000),	   /* secs (Must be Non-Negative) */
		(miliseconds % 1000) * 1000000 /* nano (Must be in range of 0 to 999999999) */
	};
	return nanosleep(&req, &rem);
}

int speed_up(int keycode, t_var *var)
{
	static int i = 0;
	// printf("%d. keycode: %d\n", i, keycode);
	i++;
	static int speed = 0;

	var->keycode = keycode;
	if (keycode == right || keycode == left || keycode == up || keycode == down)
	{
		printf("%d. speed up\n",i);
		speed += 1;
	}

	var->speed = speed;
	return (0);
}

clock_t t;

int draw(t_var *var)
{
	int x_zero = 0;
	int y_zero = 0;

	draw_rect(var, &x_zero, &y_zero, var->height, var->width, 0); // draw black rect

	static int old_speed = 0;
	static int old_keycode = right;
	int i = 1;

	if (old_speed != var->speed && old_keycode == var->keycode)
		old_speed = var->speed;
	else
		old_speed = 0;
	

	// static float time_var = 40;
	
	// printf("hey 2\n");
	if (var->keycode == right)
		var->snack_head[0] += (1 + old_speed);
	if (var->keycode == left)
		var->snack_head[0] -= (1 + old_speed);
	if (var->keycode == up)
		var->snack_head[1] -= (1 + old_speed);
	if (var->keycode == down)
		var->snack_head[1] += (1 + old_speed);
	
	draw_moving_snack(var);
	static int f = 0;
	// printf("before sleep %d\n", f);
	mssleep(var->time_var);
	// time_var -= 0.05;
	// printf("after sleep %d\n", f++);
	mlx_put_image_to_window(var->mlx, var->win, var->img_ptr, 0, 0);
	// draw_snack(&var);

	old_speed = var->speed;
	old_keycode = var->keycode;
	return (0);
}

int main(void)
{
	t_var var;

	var.width = win_width;
	var.height = win_height;
	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, var.width, var.height, "snack game");
	var.points = malloc(points_len * sizeof(int *));
	int i = 0;
	while (i < points_len)
	{
		var.points[i] = malloc(2 * sizeof(int));
		i++;
	}
	var.speed = 0;
	generate_points(&var);
	var.snack = malloc(6);
	var.snack[0] = 'r';
	var.snack[1] = 'r';
	var.snack[2] = 'r';
	var.snack[3] = 'r';
	var.snack[4] = 'r';
	var.snack[5] = '\0';
	var.snack_head[0] = 80;
	var.snack_head[1] = 40;
	var.keycode = 124;
	var.time_var = 100;
	// printf("hey 1\n");
	//  draw(var);
	t = clock();

	var.img_ptr = mlx_new_image(var.mlx, var.width, var.height);

	var.draw_image = (unsigned int *)mlx_get_data_addr(var.img_ptr, &var.bits_per_pixel, &var.line_length, &var.endian);
	mlx_loop_hook(var.mlx, draw, &var);
	mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	mlx_hook(var.win, 2, 0, speed_up, &var);
	mlx_loop(var.mlx);
}
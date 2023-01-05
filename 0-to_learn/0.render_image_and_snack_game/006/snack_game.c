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

// SETTING
#define LEN 10
#define POINTS_LEN 4

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

typedef struct s_var
{
	void *mlx;
	void *win;

	int snack_head[2];
	char *snack;
	int **points;
	int keycode;
	void *img_ptr;
	float time_var;
	int bits_per_pixel;
	int line_length;
	int endian;
	unsigned int *img_arr;
} t_var;

int draw_rect(t_var *var, int *x_start, int *y_start, int height, int width, int color)
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
			// mlx_pixel_put(var->mlx, var->win, *x_start + w, *y_start + h, color);
			var->img_arr[(*y_start + h) * WINDOW_WIDTH + (*x_start + w)] = color;
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
	draw_rect(var, &x_zero, &y_zero, WINDOW_HEIGHT, WINDOW_WIDTH, 0); // draw black rect
	int j = 0;
	while (j < POINTS_LEN)
	{
		if (!(var->points[j][0] == 0 && var->points[j][1] == 0))
			draw_rect(var, &(var->points[j][0]), &(var->points[j][1]), LEN, LEN, GREEN); // draw points
		j++;
	}
	if (var->snack_head[0] >= WINDOW_WIDTH)
		var->snack_head[0] = var->snack_head[0] - WINDOW_WIDTH;
	if (var->snack_head[0] < 0)
		var->snack_head[0] = WINDOW_WIDTH + var->snack_head[0];
	if (var->snack_head[1] >= WINDOW_HEIGHT)
		var->snack_head[1] = var->snack_head[1] - WINDOW_HEIGHT;
	if (var->snack_head[1] < 0)
		var->snack_head[1] = WINDOW_HEIGHT + var->snack_head[1];
	int i = strlen(var->snack) - 1;
	int x;
	int y;
	x = var->snack_head[0];
	y = var->snack_head[1];
	draw_rect(var, &x, &y, LEN, LEN, RED);
	while (i > 0)
	{
		if (var->snack[i] == 'r')
			x -= LEN;
		if (var->snack[i] == 'l')
			x += LEN;
		if (var->snack[i] == 'u')
			y += LEN;
		if (var->snack[i] == 'd')
			y -= LEN;
		if (x == var->snack_head[0] && y == var->snack_head[1])
		{
			printf("You did heat yourself!!\n");
			exit(-1);
		}
		draw_rect(var, &x, &y, LEN, LEN, RED);
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
	//free(snack);
	return new_snack;
}

int the_num(int num)
{
	int a = LEN;

	while (a + LEN <= num)
		a += LEN;
	return (a);
}

void generate_points(t_var *var)
{
	int i = 0;

	while (i < POINTS_LEN)
	{
		var->points[i][0] = the_num(rand() % WINDOW_WIDTH);
		var->points[i][1] = the_num(rand() % WINDOW_HEIGHT);
		int j = i - 1;
		while (j > 0)
		{
			if (var->points[j][0] == var->points[i][0] || var->points[i][0] == 0)
			{
				while (var->points[j][0] == var->points[i][0] || var->points[i][0] == 0)
					var->points[i][0] = the_num(rand() % WINDOW_WIDTH);
			}
			if (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
			{
				while (var->points[j][1] == var->points[i][1] || var->points[i][1] == 0)
					var->points[i][1] = the_num(rand() % WINDOW_HEIGHT);
			}
			j--;
		}
		i++;
	}
}

int all_points_are_eaten(int **points)
{
	int i = 0;
	while (i < POINTS_LEN)
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
		var->snack_head[1] -= LEN;
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
		var->snack_head[1] += LEN;
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
		var->snack_head[0] -= LEN;
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
		var->snack_head[0] += LEN;
		int i = 1;
		while (i < strlen(var->snack) - 1)
		{
			var->snack[i] = var->snack[i + 1];
			i++;
		}
		var->snack[i] = 'r';
	}
	i = 0;
	while (i < POINTS_LEN)
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
		var->time_var -= INCREASE_SPEED;
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

clock_t t;

int draw(t_var *var)
{
	int x_zero = 0;
	int y_zero = 0;
	// static float time_var = 40;
	draw_rect(var, &x_zero, &y_zero, WINDOW_HEIGHT, WINDOW_WIDTH, 0); // draw black rect
	// printf("hey 2\n");
	draw_moving_snack(var);
	static int f = 0;
	// printf("before sleep %d\n", f);
	mssleep(var->time_var);
	// time_var -= 0.05;
	// printf("after sleep %d\n", f++);
	mlx_put_image_to_window(var->mlx, var->win, var->img_ptr, 0, 0);
	// draw_snack(&var);
	return (0);
}

int main(void)
{
	t_var var;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Hello world!");
	var.points = malloc(POINTS_LEN * sizeof(int *));
	int i = 0;
	while (i < POINTS_LEN)
	{
		var.points[i] = malloc(2 * sizeof(int));
		i++;
	}
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
	var.time_var = TIME_TO_SLEEP;
	// printf("hey 1\n");
	//  draw(var);
	t = clock();

	var.img_ptr = mlx_new_image(var.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);

	var.img_arr = (unsigned int *)mlx_get_data_addr(var.img_ptr, &var.bits_per_pixel, &var.line_length, &var.endian);
	mlx_loop_hook(var.mlx, draw, &var);
	mlx_key_hook(var.win, clicked_key, &var); //use mlx_hook check documentation
	mlx_loop(var.mlx);
}
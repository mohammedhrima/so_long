#include <mlx.h>
#include <math.h>
#include <unistd.h>
#include "Libft/libft.h"
#include <fcntl.h>
#include <stdio.h>

// COLORS
#define RED 0xff0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define ORANGE 0xed6a12
#define YELLOW 0xFFFF00
#define BACKGROUND 0X0cf3ef
#define WHITE 0xFFFFFF

// // KEYS
#define RIGHT 124
#define DOWN 125
#define LEFT 123
#define UP 126
#define ESC 53
#define DESTROY 17

#define PI 3.14159
// static int KEY = RIGHT;

// SETTING
static double SCALE = 20;

// WINDOW / SCREEN SETTINGS
// static int Collected_in_move;

// ERRORS
enum e_map_error
{
	ERR0,
	ERROR_IN_WALLS,
	ERROR_UNKNOWN_CHARACTERS,
	ERROR_MORE_THAN_ONE_PLAYER,
	ERROR_MORE_THAN_ONE_EXIT,
	ERROR_READING_MAP,
	ERROR_RECTANGULAR,
	ERROR_NEW_LINE_AT_BEGENNING,
	NOT_ENOUGH_COLLECTIBLE,
	NO_PLAYER,
	NO_EXIT,
};

// STRUCTURES
typedef struct s_game
{
	int Player;
	int Exit;
	int *Collectible;
	int *Enemies;
	int *EnemDir;
	int Collectlen;
	int Enemlen;
	char *map;
} t_game;

typedef struct s_var
{
	void *img;
	char *addr;
	void *mlx;
	void *win;
	int keycode;
	int win_width;
	int win_height;
	int bits_per_pixel;
	int line_length;
	int endian;
	t_game game;
} t_var;

/*====================UTILS====================*/
void ft_bzero(void *s, size_t n)
{
	size_t i;
	unsigned char *ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (ptr && i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
char *read_line(int fd)
{
	char *str;
	char *tmp;
	char buffer[2];

	ft_bzero(buffer, 2);
	str = NULL;
	int r = read(fd, buffer, 1);
	while (r > 0 && buffer[0] != '\n')
	{
		buffer[r] = '\0';
		tmp = ft_strjoin(str, buffer);
		ft_bzero(buffer, 2);
		free(str);
		str = tmp;
		r = read(fd, buffer, 1);
	}
	if (r > 0 && !str)
		str = ft_strdup(buffer);
	return (str);
}
int *join_2D(int *arr, int to_add, int len)
{
	int *res;
	int i;

	res = malloc((len + 1) * sizeof(int));
	if (!res)
	{
		ft_printf("Error in 2D joinning\n");
		exit(-1);
	}
	i = 0;
	while (i < len - 1)
	{
		res[i] = arr[i];
		i++;
	}
	res[i] = to_add;
	free(arr);
	return (res);
}

char *ERROR(enum e_map_error n)
{
	char *arr[20];
	arr[0] = "";
	arr[1] = "Error: map is invalid, verify walls\n";
	arr[2] = "Error: map is invalid, it contains unknown characters\n";
	arr[3] = "Error: map is invalid, it contains more than one Player\n";
	arr[4] = "Error: map is invalid, it contains more than one Exit\n";
	arr[5] = "Error: something went wrong reading map\n";
	arr[6] = "Error: map is invalid, all lines, doesn't have the same width\n";
	arr[7] = "Error: map is invalid, line starts with new line\n";
	arr[8] = "Error: map is invalid, there isn't enough Collectible\n";
	arr[9] = "Error: map is invalid, there isn't any Player\n";
	arr[10] = "Error: map is invalid, there isn't any Exit\n";

	return (arr[n]);
}
int pow2(int num)
{
	return (num * num);
}
int abs(int num)
{
	if (num < 0)
		return -num;
	return num;
}
float rad_to_deg(float rad)
{
	return ((rad / 180) * PI);
}
int Exit()
{
	exit(0);
}
/*================MAP FUNCTIONS================*/
void Reset_var(t_var *var)
{
	if (var)
	{
		var->img = NULL;
		var->addr = NULL;
		var->mlx = NULL;
		var->win = NULL;
		var->win_width = 0;
		var->win_height = 0;
	}
}

void Reset_game(t_game *game)
{
	game->Player = 0;
	game->Exit = 0;
	game->Collectible = NULL;
	game->Collectlen = 0;
	game->Enemies = NULL;
	game->EnemDir = NULL;
	game->Enemlen = 0;
	game->map = NULL;
}
int map_contains_allowed_chars(char *str, t_game *game, int width, int height)
{
	int i = 0;
	if ((int)ft_strlen(str) != width)
		return (ERROR_RECTANGULAR);
	if (str[0] == '\n')
		return (ERROR_NEW_LINE_AT_BEGENNING);
	if (str[0] != '1')
		return (1);
	while (str[i] && str[i] != '\n')
	{
		if (!ft_strchr("01PCEX", str[i]))
			return (2);
		if (str[i] == 'P')
		{
			if (game->Player)
				return (ERROR_MORE_THAN_ONE_PLAYER);
			game->Player = height * width + i;
		}
		if (str[i] == 'C')
		{
			game->Collectlen++;
			game->Collectible = join_2D(game->Collectible, height * width + i, game->Collectlen);
		}
		if (str[i] == 'X')
		{
			game->Enemlen++;
			game->Enemies = join_2D(game->Enemies, height * width + i, game->Enemlen);
		}
		if (str[i] == 'E')
		{
			if (game->Exit)
				return (ERROR_MORE_THAN_ONE_EXIT);
			game->Exit = height * width + i;
		}
		i++;
	}
	if (str[i - 1] != '1')
		return (ERROR_IN_WALLS);
	return (0);
}
int is_wall(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (ERROR_READING_MAP);
	while (str && str[i] == '1')
		i++;
	if (i > 0 && (str[i] == '\n' || str[i] == '\0'))
		return (0);
	else
		return (ERROR_IN_WALLS);
}

void is_map_valid(t_game *game, int fd)
{
	int isError = 0;
	if (game->Player == 0)
	{
		ft_printf("%s", ERROR(NO_PLAYER));
		isError = 1;
	}
	if (game->Collectlen == 0)
	{
		ft_printf("%s", ERROR(NOT_ENOUGH_COLLECTIBLE));
		isError = 1;
	}
	if (game->Exit == 0)
	{
		ft_printf("%s", ERROR(NO_EXIT));
		isError = 1;
	}
	if (isError)
	{
		exit(-1);
		close(fd);
		free(game->map);
	}
}

t_game get_map_from_file(int fd, t_var *var)
{
	char *str;
	int n;
	char *tmp;
	t_game game;
	Reset_game(&game);

	str = read_line(fd);
	n = is_wall(str);
	if (n)
	{
		free(str);
		ft_printf("%s", ERROR(n));
		exit(-1);
	}
	var->win_width = ft_strlen(str);
	while (str)
	{
		n = map_contains_allowed_chars(str, &game, var->win_width, var->win_height);
		if (n)
		{
			ft_printf("%s", ERROR(n));
			free(game.Collectible);
			free(game.map);
			free(str);
			close(fd);
			exit(-1);
		}
		tmp = ft_strjoin(game.map, str);
		free(game.map);
		free(str);
		game.map = tmp;
		str = read_line(fd);
		var->win_height++;
	}
	n = is_wall(game.map + var->win_height * var->win_width - var->win_width);
	if (n)
	{
		ft_printf("%s", ERROR(ERROR_IN_WALLS));
		free(game.map);
		close(fd);
		exit(-1);
	}
	is_map_valid(&game, fd);
	return (game);
}

int check_valid_path(int Player, t_game game, int w, int h, char **was_here)
{
	int n = 0;
	static int Collected_in_move; // to verify

	if ((*was_here)[Player] == 'C')
		Collected_in_move++;
	if ((*was_here)[Player] == 'E')
		(*was_here)[Player] = 'F';
	else
		(*was_here)[Player] = 'P';
	if ((*was_here)[game.Exit] != 'F')
		n = -1;
	if (Collected_in_move != game.Collectlen)
		n = -1;
	if ((*was_here)[Player] == 'P' && ft_strchr("0CEX", (*was_here)[Player + 1]) && n < 0)
		n = check_valid_path(Player + 1, game, w, h, was_here);
	if ((*was_here)[Player] == 'P' && ft_strchr("0CEX", (*was_here)[Player - 1]) && n < 0)
		n = check_valid_path(Player - 1, game, w, h, was_here);
	if ((*was_here)[Player] == 'P' && ft_strchr("0CEX", (*was_here)[Player + w]) && n < 0)
		n = check_valid_path(Player + w, game, w, h, was_here);
	if ((*was_here)[Player] == 'P' && ft_strchr("0CEX", (*was_here)[Player - w]) && n < 0)
		n = check_valid_path(Player - w, game, w, h, was_here);
	return (n);
}

void get_to_the_target(t_var *var, int fd)
{
	char *map_copy;
	int n;

	map_copy = ft_strdup(var->game.map);
	n = check_valid_path(var->game.Player, var->game, var->win_width, var->win_height, &map_copy);
	free(map_copy);
	if (n < 0)
	{
		ft_printf("Path is not valid\n");
		close(fd);
		free(var->game.map);
		exit(-1);
	}
	else
		ft_printf("Path is valid\n");
}

/*==============DRAWING FUNCTIONS==============*/
void put_one_pixel(t_var *var, int x, int y, int color)
{
	char *dst;
	dst = var->addr + (y * var->line_length + x * (var->bits_per_pixel / 8));

	*(unsigned int *)dst = color;
}
void put_pixels(t_var *var, int x_start, int y_start, int width, int height, int color)
{
	int x;
	int y;
	int x_end = x_start + width;
	int y_end = y_start + height;

	if (x_start < 0)
		x_start = 0;
	if (x_end > var->win_width * SCALE)
		x_end = var->win_width * SCALE;
	if (y_start < 0)
		y_start = 0;
	if (y_end > var->win_height * SCALE)
		y_end = var->win_height * SCALE;
	y = y_start;
	while (y < y_end)
	{
		x = x_start;
		while (x < x_end)
		{
			put_one_pixel(var, x, y, color);
			x++;
		}
		y++;
	}
}
int circle(t_var *var, int x_center, int y_center, int radius, int color)
{
	int x;
	int y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			if (pow2(x_center - x) + pow2(y_center - y) <= pow2(radius))
				put_one_pixel(var, x, y, color);
			y++;
		}
		x++;
	}
	return (0);
}
// PACMAN
int draw_pacman(t_var *var, int x_center, int y_center, int radius, int dest)
{
	int x;
	int y;
	static int angle = 20;
	static int q = 15;

	if (angle < 1 || angle > 100)
		q = -q;
	angle += q;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			if (pow2(x_center - x) + pow2(y_center - y) <= pow2(radius))
			{
				if ((dest == UP && y <= y_center) || (dest == DOWN && y >= y_center) ||
					(dest == RIGHT && x >= x_center) || (dest == LEFT && x <= x_center))
					put_one_pixel(var, x, y, BACKGROUND);
				else
					put_one_pixel(var, x, y, YELLOW);
				if (((dest == UP || dest == DOWN) && (abs(x_center - x) >= sin(rad_to_deg(angle)) * abs(y_center - y))) ||
					((dest == RIGHT || dest == LEFT) && abs(y_center - y) >= sin(rad_to_deg(angle)) * abs(x_center - x)))
					put_one_pixel(var, x, y, YELLOW);
			}
			y++;
		}
		x++;
	}
	return (0);
}
// ENEMIES
int head(t_var *var, int x_center, int y_center, int radius, int color)
{
	int x;
	int y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center)
		{
			if (pow2(x_center - x) + pow2(y_center - y) <= pow2(radius) && x >= 0 && y >= 0 && x < var->win_width * SCALE && y < var->win_height * SCALE)
				put_one_pixel(var, x, y, color);
			y++;
		}
		x++;
	}
	return (0);
}

static float z = 4;
float ii = -0.2;
void draw_enemy(t_var *var, int x, int y, int radius)
{
	// ft_printf("(%d, %d) ", x / SCALE, y / SCALE);
	z += ii;
	if (z == 4)
		ii = -0.2;
	if (z < -4)
		ii = 0.2;
	head(var, x + radius, y + radius, radius, YELLOW);
	put_pixels(var, x, y + radius, radius * 2, radius, YELLOW);
	//  eyes
	circle(var, x + radius * 0.6, y + radius / 2, radius / 4, WHITE);	  // left eye
	circle(var, x + radius * 1.4, y + radius / 2, radius / 4, WHITE);	  // right eye
	circle(var, x + radius * 0.6 + z, y + radius / 2 + 2, radius / 8, 0); // left eye
	circle(var, x + radius * 1.4 + z, y + radius / 2 + 2, radius / 8, 0); // right eye
}

// MAP
static float R = 1;
float r = 0.01;
void draw_map(t_var *var)
{
	int i = 0;
	int j = 0;
	float speed = 0.2;
	static float px;
	static float py;
	static float ex[32];
	static float ey[32];
	static int first = 1;
	if (first)
	{
		px = var->game.Player % var->win_width;
		py = var->game.Player / var->win_width;
		for (int k = 0; k < var->game.Enemlen; k++)
		{
			ex[k] = var->game.Enemies[k] % var->win_width;
			ey[k] = var->game.Enemies[k] / var->win_width;
		}
		first = 0;
	}

	R += r;
	if (R >= 1.1 || R <= 0.9)
		r = -r;
	while (i < var->win_height)
	{
		j = 0;
		while (j < var->win_width)
		{
			if (var->game.map[i * var->win_width + j] == '1')
				put_pixels(var, j * SCALE, i * SCALE, SCALE, SCALE, BLUE);
			if (var->game.map[i * var->win_width + j] == 'P')
			{
				px += (j - px) * speed;
				py += (i - py) * speed;
				draw_pacman(var, px * SCALE + SCALE / 2, py * SCALE + SCALE / 2, SCALE / 2, var->keycode);
			}
			if (var->game.map[i * var->win_width + j] == 'E')
			{
				if (var->game.Collectlen)
					put_pixels(var, j * SCALE, i * SCALE, SCALE, SCALE, RED);
				else
					put_pixels(var, j * SCALE + SCALE * 0.1, i * SCALE + SCALE * 0.1, SCALE * 0.8, SCALE * 0.8, GREEN);
			}
			if (var->game.map[i * var->win_width + j] == 'X')
			{

				for (int k = 0; k < var->game.Enemlen; k++)
				{
					if (var->game.Enemies[k] == i * var->win_width + j)
					{
						ex[k] += (j - ex[k]) * 0.2;
						ey[k] += (i - ey[k]) * 0.2;
						draw_enemy(var, ex[k] * SCALE, ey[k] * SCALE, SCALE / 2);
						// break;
					}
				}

				// put_pixels(var, ex * SCALE, ey * SCALE, SCALE, SCALE, 0);
			}
			if (var->game.map[i * var->win_width + j] == 'C')
				circle(var, j * SCALE + SCALE / 2, i * SCALE + SCALE / 2, R * SCALE / 4, ORANGE);
			j++;
		}
		i++;
	}
}

/*=================ANIMATION===================*/
int clicked_key(int keycode, t_var *var)
{
	if (var->game.map[var->game.Player] == 'X')
	{
		ft_printf("You did touch enemie\n");
		exit(1);
	}
	// ft_printf("%d ",keycode);
	if (keycode == ESC)
		exit(0);
	if (keycode != UP && keycode != RIGHT && keycode != LEFT && keycode != DOWN)
		return (0);

	var->keycode = keycode;
	var->game.map[var->game.Player] = '0';
	if (keycode == RIGHT && (ft_strchr("0C", var->game.map[var->game.Player + 1]) || (var->game.Collectlen == 0 && var->game.map[var->game.Player + 1] == 'E')))
		var->game.Player++;
	else if (keycode == LEFT && (ft_strchr("0C", var->game.map[var->game.Player - 1]) || (var->game.Collectlen == 0 && var->game.map[var->game.Player - 1] == 'E')))
		var->game.Player--;
	else if (keycode == UP && (ft_strchr("0C", var->game.map[var->game.Player - var->win_width]) || (var->game.Collectlen == 0 && var->game.map[var->game.Player - var->win_width] == 'E')))
		var->game.Player -= var->win_width;
	else if (keycode == DOWN && (ft_strchr("0C", var->game.map[var->game.Player + var->win_width]) || (var->game.Collectlen == 0 && var->game.map[var->game.Player + var->win_width] == 'E')))
		var->game.Player += var->win_width;

	if (var->game.map[var->game.Player] == 'E')
	{
		ft_printf("See ya\n");
		exit(0);
	}
	if (var->game.map[var->game.Player] == 'C')
	{
		var->game.Collectlen--;
		ft_printf("Good job: %d\n", var->game.Collectlen);
		if (var->game.Collectlen == 0)
			ft_printf("Got all Collectible\n");
	}
	var->game.map[var->game.Player] = 'P';

	return (0);
}

typedef struct s_enem
{
	int pos;
	char dir;
} t_enem;

void find_Player(t_var *var)
{
	t_enem *queue = ft_calloc(var->win_width * var->win_height * 10, sizeof(var));
	int *visited = ft_calloc(var->win_width * var->win_height * 10, sizeof(int));

	int first = 1;
	int left = 0;
	int right = 0;
	//	char *map;
	//	map = ft_strdup(var->game.map);

	queue[right].pos = var->game.Player;
	right++;
	visited[var->game.Player] = 'X';

	while (left < right)
	{
		// ft_printf("search ");
		t_enem cur = queue[left];
		left++;
		if (var->game.map[cur.pos] == 'X')
		{
			int i = 0;
			while (i < var->game.Enemlen)
			{
				if (var->game.Enemies[i] == cur.pos)
				{
					var->game.EnemDir[i] = cur.dir;
					break;
				}
				i++;
			}
		}
		if (!ft_strchr("C1E", var->game.map[cur.pos]))
		{
			if (!visited[cur.pos + 1])
			{
				queue[right].pos = cur.pos + 1;
				queue[right].dir = RIGHT;
				visited[cur.pos + 1] = 'X';
				right++;
			}
			if (!visited[cur.pos + var->win_width])
			{
				queue[right].pos = cur.pos + var->win_width;
				queue[right].dir = DOWN;
				visited[cur.pos + var->win_width] = 'X';
				right++;
			}
			if (!visited[cur.pos - 1])
			{
				queue[right].pos = cur.pos - 1;
				queue[right].dir = LEFT;
				visited[cur.pos - 1] = 'X';
				right++;
			}
			if (!visited[cur.pos - var->win_width])
			{

				queue[right].pos = cur.pos - var->win_width;
				queue[right].dir = UP;
				visited[cur.pos - var->win_width] = 'X';
				right++;
			}
		}
	}

	first = 0;
}

void move_enemie(t_var *var)
{
	find_Player(var);
	int i = 0;
	// ft_printf("(%d, %d) %c |", var->game.Enemies[0] % var->win_width, var->game.Enemies[0] / var->win_width, var->game.map[var->game.Enemies[0]]);
	while (i < var->game.Enemlen)
	{
		if (var->game.map[var->game.Player] == 'X')
		{
			ft_printf("Enemy did heat you\n");
			exit(1);
		}
		// ft_printf("loop ");
		if (var->game.EnemDir[i] == UP)
		{
			var->game.map[var->game.Enemies[i]] = '0';
			var->game.map[var->game.Enemies[i] + var->win_width] = 'X';
			var->game.Enemies[i] += var->win_width;
			var->game.EnemDir[i] = 999;
		}
		if (var->game.EnemDir[i] == DOWN)
		{
			var->game.map[var->game.Enemies[i]] = '0';
			var->game.map[var->game.Enemies[i] - var->win_width] = 'X';
			var->game.Enemies[i] -= var->win_width;
			var->game.EnemDir[i] = 999;
		}
		if (var->game.EnemDir[i] == RIGHT)
		{
			var->game.map[var->game.Enemies[i]] = '0';
			var->game.map[var->game.Enemies[i] - 1] = 'X';
			var->game.Enemies[i]--;
			var->game.EnemDir[i] = 999;
		}
		if (var->game.EnemDir[i] == LEFT)
		{
			var->game.map[var->game.Enemies[i]] = '0';
			var->game.map[var->game.Enemies[i] + 1] = 'X';
			var->game.Enemies[i]++;
			var->game.EnemDir[i] = 999;
		}

		i++;
	}
}

static float q = 0;
int draw(t_var *var)
{
	put_pixels(var, 0, 0, var->win_width * SCALE, var->win_height * SCALE, BACKGROUND);

	draw_map(var);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	// sleep(1);
	if (q > 5)
	{
		move_enemie(var);
		q = 0;
	}
	q += 0.2;
	// ft_printf("(%d, %d) ", var->game.Enemies[0] % var->win_width, var->game.Enemies[0] / var->win_width);
	return (0);
}

/*====================MAIN=====================*/
int main(int argc, char **argv)
{
	int fd;
	t_var var;

	if (argc != 2)
	{
		ft_printf("you need to have one argument, whish it's the path to the map\n");
		exit(1);
	}
	// read map from file
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		ft_printf("%s", ERROR(ERROR_READING_MAP));
		exit(-1);
	}

	// build 2D string from readen map
	Reset_var(&var);
	var.game = get_map_from_file(fd, &var);
	var.game.EnemDir = ft_calloc((var.game.Enemlen + 1), sizeof(int));
	var.keycode = RIGHT;
	int i = 0;
	ft_printf("\nCollectible: ");
	while (i < var.game.Collectlen)
		ft_printf("%d ", var.game.Collectible[i++]);
	ft_printf("\nCollectible len: %d\n", var.game.Collectlen);
	i = 0;
	ft_printf("\nEnemies: ", var.game.Enemlen);
	while (i < var.game.Enemlen)
		ft_printf("%d ", var.game.Enemies[i++]);
	ft_printf("\nEnemies len: %d\n", var.game.Enemlen);
	ft_printf("Player: %d\nExit: %d\n", var.game.Player, var.game.Exit);
	ft_printf("Height: %d\nWidth: %d\n\n", var.win_height, var.win_width);

	get_to_the_target(&var, fd);

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, SCALE * var.win_width, SCALE * var.win_height, "Pacman");
	var.img = mlx_new_image(var.mlx, SCALE * var.win_width, SCALE * var.win_height);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length, &var.endian);

	mlx_loop_hook(var.mlx, draw, &var);
	mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	mlx_hook(var.win, DESTROY, 0, Exit, &var);

	mlx_loop(var.mlx);
}
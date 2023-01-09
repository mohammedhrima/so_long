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

// KEYS
#define RIGHT 124
#define DOWN 125
#define LEFT 123
#define UP 126
static int KEY = RIGHT;

// SETTING
#define RADIUS 20
static double SCALE = 40;

// WINDOW SETTING
static int  WINDOW_WIDTH;
static int WINDOW_HEIGHT;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// STRUCTURES
typedef struct s_game
{
	int Player;
	int Exit;
	int *Collectible;
	int *Enemies;
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
	int bits_per_pixel;
	int line_length;
	int endian;
	t_game game;
} t_var;

// MAP functions
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
		exit(-1);
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
int map_contains_allowed_chars(char *str, t_game *game, int w, int h)
{
	int i = 0;
	if (str[i] != '1')
		return (-1);
	while (str[i] && str[i] != '\n')
	{
		if (!ft_strchr("01PCEX", str[i]))
			return (-2);
		if (str[i] == 'P')
		{
			if (game->Player)
				return (-3);
			game->Player = h * w + i;
		}
		if (str[i] == 'C')
		{
			game->Collectlen++;
			game->Collectible = join_2D(game->Collectible, h * w + i, game->Collectlen);
		}
		if (str[i] == 'X')
		{
			game->Enemlen++;
			game->Enemies = join_2D(game->Enemies, h * w + i, game->Enemlen);
		}
		if (str[i] == 'E')
		{
			if (game->Exit)
				return (-4);
			game->Exit = h * w + i;
		}
		i++;
	}
	if (str[i - 1] != '1')
		return (-1);
	return (0);
}
int is_wall(char *str)
{
	int i;

	i = 0;
	while (str && str[i] && str[i] == '1')
		i++;
	if (i > 0 && (str[i] == '\n' || str[i] == '\0'))
		return (0);
	else
		return (-1);
}
t_game get_map(int fd)
{
	t_game game;
	game.Player = 0;
	game.Exit = 0;
	game.Collectible = NULL;
	game.Collectlen = 0;
	game.Enemies = NULL;
	game.Enemlen = 0;
	game.map = NULL;
	char *tmp;

	char *str = read_line(fd);
	WINDOW_WIDTH = 0;
	WINDOW_HEIGHT = 0;
	char *arr[] = {
		"",
		"map is invalid, verify walls\n",
		"map is invalid, it contains unknown characters\n",
		"map is invalid, it contains more than one Player\n"
		"map is invalid, it contains more than one Exit\n"};
	if (!str)
	{
		free(str);
		printf("something wrong with the map\n");
		exit(-1);
	}
	if (is_wall(str) < 0)
	{
		printf("map is invalid, verify first line if is wall\n");
		free(str);
		exit(-1);
	}
	WINDOW_WIDTH = ft_strlen(str);
	while (str && ft_strlen(str))
	{
		if (ft_strlen(str) != WINDOW_WIDTH)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, all lines, doesn't have the same width\n");
			exit(-1);
		}
		int n = map_contains_allowed_chars(str, &game, WINDOW_WIDTH, WINDOW_HEIGHT);
		if (n == -1 || n == -2 || n == -3 || n == -4)
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			ft_printf("%s",arr[-n]);
			exit(-1);
		}
		tmp = ft_strjoin(game.map, str);
		free(game.map);
		free(str);
		game.map = tmp;
		str = read_line(fd);
		if (str && str[0] == '\n')
		{
			free(game.Collectible);
			free(game.map);
			free(str);
			printf("map is invalid, there is two followed new lines\n");
			exit(-1);
		}
		WINDOW_HEIGHT++;
	}

	if (is_wall(game.map + WINDOW_HEIGHT * WINDOW_WIDTH - WINDOW_WIDTH) < 0)
	{
		printf("map is invalid, verify last line if is wall\n");
		free(game.map);
		close(fd);
		exit(-1);
	}
	if (game.Collectlen == 0)
	{
		printf("map is invalid, there isn't enough Collectible\n");
		close(fd);
		free(game.map);
		exit(-1);
	}
	if (game.Player == 0)
	{
		printf("map is invalid, there isn't any Player\n");
		close(fd);
		free(game.map);
		exit(-1);
	}
	if (game.Exit == 0)
	{
		printf("map is invalid, there isn't any Exit\n");
		close(fd);
		free(game.map);
		exit(-1);
	}
	return (game);
}

int is_access(char c, int is_equal)
{
	return (c == '0' || c == 'C' || c == 'E' || c == 'X');
}
int m = 0;
int move(int Player, t_game game, int w, int h, char **was_here)
{
	int x = Player % w;
	int y = Player / w;
	int n = 0;
	int i = 0;

	if ((*was_here)[Player] == 'C')
		m++;
	if ((*was_here)[Player] == 'E')
		(*was_here)[Player] = 'F';
	else
		(*was_here)[Player] = 'P';
	if ((*was_here)[game.Exit] != 'F')
		n = -1;
	if (m != game.Collectlen)
		n = -1;
	if ((*was_here)[Player] == 'P' && is_access((*was_here)[y * w + x + 1], m == game.Collectlen) && n < 0)
		n = move(y * w + x + 1, game, w, h, was_here);
	if ((*was_here)[Player] == 'P' && is_access((*was_here)[y * w + x - 1], m == game.Collectlen) && n < 0)
		n = move(y * w + x - 1, game, w, h, was_here);
	if ((*was_here)[Player] == 'P' && is_access((*was_here)[(y + 1) * w + x], m == game.Collectlen) && n < 0)
		n = move((y + 1) * w + x, game, w, h, was_here);
	if ((*was_here)[Player] == 'P' && is_access((*was_here)[(y - 1) * w + x], m == game.Collectlen) && n < 0)
		n = move((y - 1) * w + x, game, w, h, was_here);
	return (n);
}
char *get_to_the_target(t_game game, int fd)
{
	// printf("\n=====in get to the target=====\n");
	char *map_copy;
	int i, j;

	map_copy = ft_strdup(game.map);
	int n = 0;
	n = move(game.Player, game, WINDOW_WIDTH, WINDOW_HEIGHT, &map_copy);
	if (n < 0)
	{
		printf("Path is not valid\n");
		free(map_copy);
		return (NULL);
	}
	else
	{
		printf("Path is valid\n");
		return (map_copy);
	}
}

// DRAWING FUNCTIONS
void put_one_pixel(t_var *var, int x, int y, int color)
{
	char *dst;
	dst = var->addr + (y * var->line_length + x * (var->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
void put_pixels(t_var *var, int x_start, int y_start, int width, int height, int color)
{
	char *dst;
	int x_end = x_start + width;
	int y_end = y_start + height;

	if (x_start < 0) x_start = 0;
	if (x_end > SCREEN_WIDTH)
		x_end = SCREEN_WIDTH;
	if (y_start < 0) y_start = 0;
	if (y_end > SCREEN_HEIGHT)
		y_end = SCREEN_HEIGHT;
	// width--;
	//height--;

	for (int y = y_start; y < y_end; y++)
	{
		for (int x = x_start; x < x_end; x++)
		{
			put_one_pixel(var, x, y, color);
		}
	
	}
}
int clicked_key(int keycode, t_var *var)
{
	int x = var->game.Player % WINDOW_WIDTH;
	int y = var->game.Player / WINDOW_WIDTH; // -2 for walls left and right
	KEY = keycode;
	if (keycode == RIGHT && ((var->game.map[y * WINDOW_WIDTH + x + 1] != '1' && var->game.map[y * WINDOW_WIDTH + x + 1] != 'E') || (var->game.Collectlen == 0 && var->game.map[y * WINDOW_WIDTH + x + 1] == 'E')))
	{
		if (var->game.map[y * WINDOW_WIDTH + x + 1] == 'X')
		{
			printf("You did touch enemie\n");
			exit(1);
		}
		if (var->game.map[y * WINDOW_WIDTH + x + 1] == 'E')
		{
			printf("See ya\n");
			exit(0);
		}
		var->game.map[var->game.Player] = '0';
		var->game.Player = y * WINDOW_WIDTH + x + 1;
	}
	if (keycode == LEFT && ((var->game.map[y * WINDOW_WIDTH + x - 1] != '1' && var->game.map[y * WINDOW_WIDTH + x - 1] != 'E') || (var->game.Collectlen == 0 && var->game.map[y * WINDOW_WIDTH + x - 1] == 'E')))
	{
		if (var->game.map[y * WINDOW_WIDTH + x - 1] == 'X')
		{
			printf("You did touch enemie\n");
			exit(1);
		}
		if (var->game.map[y * WINDOW_WIDTH + x - 1] == 'E')
		{
			printf("See ya\n");
			exit(0);
		}
		var->game.map[var->game.Player] = '0';
		var->game.Player = y * WINDOW_WIDTH + x - 1;
	}
	if (keycode == UP && ((var->game.map[(y - 1) * WINDOW_WIDTH + x] != '1' && var->game.map[(y - 1) * WINDOW_WIDTH + x] != 'E') || (var->game.Collectlen == 0 && var->game.map[(y - 1) * WINDOW_WIDTH + x] == 'E')))
	{
		if (var->game.map[(y - 1) * WINDOW_WIDTH + x] == 'X')
		{
			printf("You did touch enemie\n");
			exit(1);
		}
		if (var->game.map[(y - 1) * WINDOW_WIDTH + x] == 'E')
		{
			printf("See ya\n");
			exit(0);
		}
		var->game.map[var->game.Player] = '0';
		var->game.Player = (y - 1) * WINDOW_WIDTH + x;
	}
	if (keycode == DOWN && ((var->game.map[(y + 1) * WINDOW_WIDTH + x] != '1' && var->game.map[(y + 1) * WINDOW_WIDTH + x] != 'E') || (var->game.Collectlen == 0 && var->game.map[(y + 1) * WINDOW_WIDTH + x] == 'E')))
	{
		if (var->game.map[(y + 1) * WINDOW_WIDTH + x] == 'X')
		{
			printf("You did touch enemie\n");
			exit(1);
		}
		if (var->game.map[(y + 1) * WINDOW_WIDTH + x] == 'E')
		{
			printf("See ya\n");
			exit(0);
		}
		var->game.map[var->game.Player] = '0';
		var->game.Player = (y + 1) * WINDOW_WIDTH + x;
	}
	if (var->game.map[var->game.Player] == 'C')
	{
		var->game.Collectlen--;
		printf("Good job: %d\n", var->game.Collectlen);
		if (var->game.Collectlen == 0)
			printf("Got all Collectible\n");
	}

	var->game.map[var->game.Player] = 'P';
	return (0);
}
// Pacman
int abs(int num)
{
	if (num < 0)
		return -num;
	return num;
}

int pow2(int num)
{
	return (num * num);
}
int face(t_var *var, int x_center, int y_center, int radius, int color, float frequence)
{
	int x;
	int y;

	x = x_center - radius;
	static int last_key;

	if (KEY != RIGHT && KEY != LEFT && KEY != UP && KEY != DOWN)
	{
		if (last_key == RIGHT || last_key == LEFT || last_key == UP || last_key == DOWN)
			KEY = last_key;
		else
			KEY = RIGHT;
	}
	if (KEY == RIGHT)
	{

		while (x < x_center + radius)
		{
			y = y_center - radius;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && (x - x_center) < 0)
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x++;
		}

		x = x_center;
		while (x < x_center + radius)
		{
			y = y_center - radius;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && abs(y - y_center) >= abs(x - x_center) * frequence) // better not to do Y/X because X can be 0
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x++;
		}
	}
	if (KEY == LEFT)
	{
		while (x < x_center + radius)
		{
			y = y_center - radius;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && (x - x_center) > 0)
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x++;
		}
		x = x_center;
		while (x > x_center - radius)
		{
			y = y_center - radius;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && abs(y - y_center) >= abs(x - x_center) * frequence) // better not to do Y/X because X can be 0
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x--;
		}
	}
	if (KEY == DOWN)
	{
		while (x < x_center + radius)
		{
			y = y_center - radius;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && (y - y_center) < 0)
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x++;
		}

		x = x_center - radius;
		while (x < x_center + radius)
		{
			y = y_center;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && abs(x - x_center) >= abs(y - y_center) * frequence) // better not to do Y/X because X can be 0
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x++;
		}
	}

	if (KEY == UP)
	{
		while (x < x_center + radius)
		{
			y = y_center - radius;
			while (y < y_center + radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && (y - y_center) > 0)
					put_pixels(var, x, y, 1, 1, color);
				y++;
			}
			x++;
		}
		x = x_center - radius;
		while (x < x_center + radius)
		{
			y = y_center;
			while (y > y_center - radius)
			{
				if (pow2(x - x_center) + pow2(y - y_center) < pow2(radius) && abs(x - x_center) >= abs(y - y_center) * frequence) // better not to do Y/X because X can be 0
					put_pixels(var, x, y, 1, 1, color);
				y--;
			}
			x++;
		}
	}
	last_key = KEY;
	return (0);
}

int circle(t_var *var, int x_center, int y_center, int radius, int color)
{
	int x;
	int y;
	int X;
	int Y;

	x = x_center - radius;
	while (x < x_center + radius)
	{
		y = y_center - radius;
		while (y < y_center + radius)
		{
			X = x_center - x;
			Y = y_center - y;
			if (X * X + Y * Y <= radius * radius)
				put_pixels(var, x, y, 1, 1, color);
			y++;
		}
		x++;
	}
	return (0);
}

static float px;
static float py;
static float q = 0.01;
void move_enemie(t_var *var)
{
	int i = 0;
	int x;
	int y;
	int just_move = rand() % 4;
	int did_move[4];
	int Player_x = var->game.Player % WINDOW_WIDTH;
	int Playe_y = var->game.Player / WINDOW_HEIGHT;

	while (i < var->game.Enemlen)
	{
		if (q <= 0.5)
		{
			did_move[0]--;
			did_move[1]--;
			did_move[2]--;
			did_move[3]--;
		}
		if (q > 1)
		{
			x = var->game.Enemies[i] % WINDOW_WIDTH;
			y = var->game.Enemies[i] / WINDOW_WIDTH;
			var->game.map[y * WINDOW_WIDTH + x] = '0';
			if (Player_x >= x && (var->game.map[y * WINDOW_WIDTH + x + 1] == 'P' || var->game.map[y * WINDOW_WIDTH + x + 1] == '0'))
			{
				var->game.Enemies[i] = y * WINDOW_WIDTH + x + 1;
				var->game.map[y * WINDOW_WIDTH + x + 1] = 'X';
				did_move[0]--;
				did_move[1]++;
				did_move[2]++;
				did_move[3]++;
			}
			else if ((var->game.map[(y - 1) * WINDOW_WIDTH + x] == 'P' || var->game.map[(y - 1) * WINDOW_WIDTH + x] == '0'))
			{
				var->game.Enemies[i] = (y - 1) * WINDOW_WIDTH + x;
				var->game.map[(y - 1) * WINDOW_WIDTH + x] = 'X';
				did_move[0]--;
				did_move[1]--;
				did_move[2]++;
				did_move[3]++;
			}
			else if ((var->game.map[y * WINDOW_WIDTH + x - 1] == 'P' || var->game.map[y * WINDOW_WIDTH + x - 1] == '0'))
			{
				var->game.Enemies[i] = y * WINDOW_WIDTH + x - 1;
				var->game.map[y * WINDOW_WIDTH + x - 1] = 'X';
				just_move = 'u';
				did_move[0]++;
				did_move[1]++;
				did_move[2]--;
				did_move[3]++;
			}
			else if ((var->game.map[(y + 1) * WINDOW_WIDTH + x] == 'P' || var->game.map[(y + 1) * WINDOW_WIDTH + x] == '0') && did_move[3])
			{
				var->game.Enemies[i] = (y + 1) * WINDOW_WIDTH + x;
				var->game.map[(y + 1) * WINDOW_WIDTH + x] = 'X';
				did_move[0]++;
				did_move[1]++;
				did_move[2]--;
				did_move[3]--;
			}
			else
			{
				var->game.Enemies[i] = y * WINDOW_WIDTH + x;
				var->game.map[y * WINDOW_WIDTH + x] = 'X';
			}
		}
		i++;
	}
	if (q >= 1)
		q = 0.01;
	q += 0.04;
}

static float frequence = 0.5;
static float fr = 0.1;
void draw_pacman(t_var *var, int x, int y, int radius)
{
	frequence += fr;
	if (frequence > 1 || frequence < 0)
		fr = -fr;



// face(var, x, y, radius, BACKGROUND, 0);
// printf("%d %d %f %f\n", x, y, px, py);
#if 0
	face(var, px + radius, py + radius, radius, YELLOW, frequence);
#else
	face(var, x + radius, y + radius, radius, YELLOW, frequence);
#endif
}
// MAP
static float R = 1;
float r = 0.01;
void draw_map(t_var *var)
{
	int i = 0;
	int j = 0;
	int pac_i;
	int pac_j;

	R += r;
	if (R >= 1.1 || R <= 0.9)
		r = -r;


	while (i < WINDOW_HEIGHT)
	{
		j = 0;
		while (j < WINDOW_WIDTH)
		{
			if (var->game.map[i * WINDOW_WIDTH + j] == 'P')
			{
				// put_pixels(var, j * SCALE, i * SCALE, SCALE, SCALE, BACKGROUND);
				pac_i = i;
				pac_j = j;
			}
			j++;
		}
		i++;
	}

	
	float speed = 1.0 / 60 * 10;

	
	px +=  (pac_j - px) * speed;
	py += (pac_i - py) * speed;



	int pac_x = SCREEN_WIDTH / 2;
	int pac_y = SCREEN_HEIGHT / 2;


	for (int ci = pac_i - 20; ci < pac_i + 20; ci++)
	{
		for (int cj = pac_j - 20; cj < pac_j + 20; cj++)
		{
			int s = SCALE - 1;
			float x = pac_x + (cj - pac_j) * SCALE;
			float y = pac_y + (ci - pac_i) * SCALE;


			if (ci < 0 || cj < 0 || ci >= WINDOW_HEIGHT || cj >= WINDOW_WIDTH)
				put_pixels(var, x, y, s, s, 0);
			else if (var->game.map[ci * WINDOW_WIDTH + cj] == '1')
				put_pixels(var, x, y, s, s, BLUE);
			else if (var->game.map[ci * WINDOW_WIDTH + cj] == 'E')
			{
				if (var->game.Collectlen)
					put_pixels(var, x, y, s, s, RED);
				else
				{
					put_pixels(var, x, y, s, s, RED);
					//put_pixels(var, x * s + s * 0.1, y + s * 0.1, s * 0.8, s * 0.8, GREEN);
				}
			}
			else if (var->game.map[ci * WINDOW_WIDTH + cj] == 'X')
				put_pixels(var, x, y, s, s, 0);
			else if (var->game.map[ci * WINDOW_WIDTH + cj] == 'C')
			{
				circle(var, x + SCALE / 2, y + SCALE / 2, R * SCALE / 4, ORANGE);
			}
			//else
			//	put_pixels(var, x, y, s, s, 0);
		}
	}
	draw_pacman(var, pac_x, pac_y, SCALE / 2);
}
int draw(t_var *var)
{
	// mlx_clear_window(var->mlx, var->win);
	put_pixels(var, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,0);

	draw_map(var);
	mlx_put_image_to_window(var->mlx, var->win, var->img, 0, 0);
	// move_enemie(var);
	return (0);
}
int main(void)
{
	t_var var;
	// t_game game;
	int fd;
	char *str;

	// open map and get the index of wanted elements
	fd = open("./map.ber", O_RDONLY);
	var.game = get_map(fd);
	int i = 0;
	printf("\nCollectible: ");
	while (i < var.game.Collectlen)
		printf("%d ", var.game.Collectible[i++]);
	printf("\nCollectible len: %d\n", var.game.Collectlen);
	i = 0;
	printf("\nEnemies: ");
	while (i < var.game.Enemlen)
		printf("%d ", var.game.Enemies[i++]);
	printf("\nEnemies len: %d\n", var.game.Enemlen);
	printf("Player: %d\nExit: %d\n", var.game.Player, var.game.Exit);
	printf("Height: %d\nWidth: %d\n\n", WINDOW_HEIGHT, WINDOW_WIDTH);
	// printf("Readen map is: %s\n", var.game.map);
	//  return(0);
	//   check if there is valid path
	str = get_to_the_target(var.game, fd);
	close(fd);
	if (!str)
		exit(-1);
	// printf("After getting to the target: %s\n", str);
	i = 0;
	int j = 0;
	while (j < WINDOW_HEIGHT)
	{
		i = 0;
		while (i < WINDOW_WIDTH)
		{
			write(1, str + j * WINDOW_WIDTH + i, 1);
			i++;
		}
		write(1, "\n", 1);
		j++;
	}

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Pacman");
	var.img = mlx_new_image(var.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	var.addr = mlx_get_data_addr(var.img, &var.bits_per_pixel, &var.line_length,
								 &var.endian);

	mlx_loop_hook(var.mlx, draw, &var);
	mlx_hook(var.win, 2, 0, clicked_key, &var); // use mlx_hook check documentation
	mlx_loop(var.mlx);
}
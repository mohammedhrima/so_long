#include <stdio.h>
#include <stdlib.h>

typedef struct variable
{
	int value;
	int i;
	int j;
	// int max_bot;
	// int max_ri;
	struct variable *top;
	struct variable *right;
	struct variable *left;
	struct variable *bottom;
} var;
int ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}
void *ft_calloc(size_t count, size_t size)
{
	void *arr;
	unsigned char *ptr;
	size_t i;
	if (count && size > SIZE_MAX / count)
		return (NULL);
	arr = (void *)malloc(count * size);
	if (!arr)
		return (NULL);
	i = 0;
	ptr = (unsigned char *)arr;
	while (i < count * size)
	{
		ptr[i] = 0;
		i++;
	}
	return (arr);
}
char *ft_strdup(const char *s)
{
	char *ptr;
	int i;

	ptr = (char *)ft_calloc((ft_strlen(s) + 1), sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s && s[i])
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
char **arrdup(char *arr[])
{
	char **res;
	int i = 0;

	while (arr[i])
		i++;
	res = ft_calloc(i + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (arr && arr[i])
	{
		res[i] = ft_strdup(arr[i]);
		i++;
	}
	res[i] = NULL;
	return (res);
}

var *new_var(int i, int j)
{
	var *new;

	new = (var *)malloc(sizeof(var));
	if (!new)
		return (NULL);
	new->i = i;
	new->j = j;
	new->value = 0;
	// new->top = NULL;
	// new->right = NULL;
	// new->bottom = NULL;
	// new->left = NULL;
	return (new);
}

// var *move(var *Player, char ***was_here)
// {
// 	// verify memory leaks
// 	int I;
// 	int J;
// 	if (!Player)
// 		return (NULL);
// 	I = Player->i;
// 	J = Player->j;
// 	var *new_Player = new_var(I, J);
// 	if (I > 0 && (*was_here)[I - 1][J] != 'P' && (*was_here)[I - 1][J] != '1') // top
// 	{
// 		printf("move top\n");
// 		(*was_here)[I - 1][J] = 'P';
// 	}
// 	if (I < Player->max_bot && (*was_here)[I + 1][J] != 'P' && (*was_here)[I + 1][J] != '1') // bottom
// 	{
// 		printf("move bottom\n");
// 		(*was_here)[I + 1][J] = 'P';
// 	}
// 	if (J > 0 && (*was_here)[I][J - 1] != 'P' && (*was_here)[I][J - 1] != '1') // left
// 	{
// 		printf("move left\n");
// 		(*was_here)[I][J - 1] = 'P';
// 	}
// 	if (J < Player->max_ri && (*was_here)[I][J + 1] != 'P' && (*was_here)[I][J + 1] != '1') // right
// 	{
// 		printf("move right\n");
// 		(*was_here)[I][J + 1] = 'P';
// 	}
// 	return (new_Player);
// }
// void While_loop(var *Player, char ***was_here)
// {
// 	if (!Player)
// 		return;
// 	if (Player->top)
// 	{
// 		Player->top = move_top(Player->top, was_here);
// 		// move_bottom(Player->top, was_here);
// 		// move_right(Player->top, was_here);
// 		// move_left(Player->top, was_here);
// 	}
// 	if (Player->right)
// 	{
// 		// move_top(Player->right, was_here);
// 		// move_bottom(Player->right, was_here);
// 		Player->right = move_right(Player->right, was_here);
// 		// move_left(Player->right, was_here);
// 	}
// 	if (Player->left)
// 	{
// 		// move_top(Player->left, was_here);
// 		// move_bottom(Player->left, was_here);
// 		// move_right(Player->left, was_here);
// 		Player->left = move_left(Player->left, was_here);
// 	}
// 	if (Player->bottom)
// 	{
// 		// move_top(Player->bottom, was_here);
// 		Player->bottom = move_bottom(Player->bottom, was_here);
// 		// move_right(Player->bottom, was_here);
// 		// move_left(Player->bottom, was_here);
// 	}
// 	int i = 0;
// 	int j = 0;
// 	printf("\n");
// 	while (i < Player->max_bot)
// 	{
// 		j = 0;
// 		while (j < Player->max_ri)
// 		{
// 			printf(".%c", was_here[i][j]);
// 			j++;
// 		}
// 		printf("\n");
// 		i++;
// 	}
// 	printf("\n");
// 	// While_loop(Player, was_here);
// }

var *move_top(var *Player, char ***was_here)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (I > 0 && (*was_here)[I - 1][J] != 'P' && (*was_here)[I - 1][J] != '1') // top
	{
		var *new_Player = new_var(I - 1, J);
		printf("move top\n");
		(*was_here)[I - 1][J] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}

var *move_bottom(var *Player, char ***was_here, int max_bot)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (I < max_bot && (*was_here)[I + 1][J] != 'P' && (*was_here)[I + 1][J] != '1') // bottom
	{
		var *new_Player = new_var(I + 1, J);
		printf("move bottom\n");
		(*was_here)[I + 1][J] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}

var *move_right(var *Player, char ***was_here, int max_ri)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (J < max_ri && (*was_here)[I][J + 1] != 'P' && (*was_here)[I][J + 1] != '1') // right
	{
		var *new_Player = new_var(I, J + 1);
		printf("move right\n");
		(*was_here)[I][J + 1] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}
var *move_left(var *Player, char ***was_here)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (J > 0 && (*was_here)[I][J - 1] != 'P' && (*was_here)[I][J - 1] != '1') // left
	{
		var *new_Player = new_var(I, J - 1);
		printf("move left\n");
		(*was_here)[I][J - 1] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}
var *move_top_right(var *Player, char ***was_here, int max_ri)
{
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (I > 0 && J < max_ri && (*was_here)[I - 1][J + 1] != 'P' && (*was_here)[I - 1][J + 1] != '1') // top right
	{
		var *new_Player = new_var(I - 1, J + 1);
		printf("move top\n");
		(*was_here)[I - 1][J + 1] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}
var *move_top_left(var *Player, char ***was_here)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (I > 0 && J > 0 && (*was_here)[I - 1][J - 1] != 'P' && (*was_here)[I - 1][J - 1] != '1') // top
	{
		var *new_Player = new_var(I - 1, J - 1);
		printf("move top\n");
		(*was_here)[I - 1][J - 1] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}
var *move_bottom_right(var *Player, char ***was_here, int max_bot, int max_ri)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (I < max_bot && J < max_ri && (*was_here)[I + 1][J + 1] != 'P' && (*was_here)[I + 1][J + 1] != '1') // bottom
	{
		var *new_Player = new_var(I + 1, J + 1);
		printf("move bottom\n");
		(*was_here)[I + 1][J + 1] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}
var *move_bottom_left(var *Player, char ***was_here, int max_bot)
{
	// verify memory leaks
	int I;
	int J;

	if (!Player)
		return (NULL);
	I = Player->i;
	J = Player->j;

	if (I < max_bot && J > 0 && (*was_here)[I + 1][J - 1] != 'P' && (*was_here)[I + 1][J - 1] != '1') // bottom
	{
		var *new_Player = new_var(I + 1, J - 1);
		printf("move bottom\n");
		(*was_here)[I + 1][J - 1] = 'P';
		return (new_Player);
	}
	else
		return NULL;
}
int *Player_location(char **arr)
{
	int i = 0;
	int j = 0;
	int res[2];
	while (arr && arr[i])
	{
		j = 0;
		while (arr[i] && arr[i][j])
		{
			if (arr[i][j] == 'P')
			{
				res[0] = i;
				res[1] = j;
				return res;
			}
			j++;
		}
		i++;
	}

	return (NULL);
}

int main(void)
{
	char *arr[] = {
		"111111111111111111111",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000P00001",
		"100000000000000000001",
		"100000000000000000001",
		"100000000000000000001",
		"111111111111111111111",
		NULL};

	// int min_i = 0;
	// int min_j = 0;
	int *P = Player_location(arr);
	if (!P)
	{
		printf("Player not found\n");
		exit(-1);
	}
	int Px = P[0];
	int Py = P[1];
	printf("(%d, %d)\n", P[0], P[1]);
	// printf("%c ", arr[min_i][min_j]);
	// printf("%c ", arr[min_i][max_ri]);
	// printf("%c ", arr[max_bot][min_j]);
	// printf("%c ", arr[max_bot][max_ri]);

	// return (0);

	char **was_here = arrdup(arr);
	int i = 0;
	int j = 0;
	while (i < 9)
	{
		j = 0;
		while (j < ft_strlen(arr[0]))
		{
			printf("%c", was_here[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	int I;
	int J;
	int k = 0;
	var *Player;
	Player = new_var(Px, Py);
	int max_bot = 9;
	int max_ri = ft_strlen(arr[0]);
	Player->value = arr[Player->i][Player->j];

	/////////////////////////////////////////////////
	// Player = move(Player, &was_here);
	Player->right = move_right(Player, &was_here, max_ri);
	Player->left = move_left(Player, &was_here);
	Player->top = move_top(Player, &was_here);
	Player->bottom = move_bottom(Player, &was_here, max_bot);

	while (Player->right || Player->left || Player->top || Player->bottom)
	{
		if (Player->right)
		{
			move_top(Player->right, &was_here);
			move_right(Player->right, &was_here, max_ri);
			move_bottom(Player->right, &was_here, max_bot);
			move_top_right(Player->right,&was_here, max_ri);
			mov
			
		}
		if (Player->left)
		{
			Player->left = move_left(Player->left, &was_here);
			move_top(Player->left, &was_here);
			move_bottom(Player->left, &was_here, max_bot);
		}

		if (Player->top)
		{
			Player->top = move_top(Player->top, &was_here);
			move_right(Player->top, &was_here, max_ri);
			move_left(Player->top, &was_here);
		}

		if (Player->bottom)
		{
			Player->bottom = move_bottom(Player->bottom, &was_here, max_bot);
			move_right(Player->bottom, &was_here, max_ri);
			move_left(Player->bottom, &was_here);
		}
	}

	// While_loop(Player, &was_here);

	// while (k < 5)
	// {
	// 	// Player->right = move(Player->right, &was_here, max_bot, max_ri);
	// 	// Player->left = move(Player->left, &was_here, max_bot, max_ri);
	// 	// Player->top = move(Player->top, &was_here, max_bot, max_ri);
	// 	// Player->bottom = move(Player->bottom, &was_here, max_bot, max_ri);
	// 	k++;
	// }
	printf("\nAfter while loop: i_max=%d j_max=%d\n", max_bot, max_ri);
	i = 0;
	j = 0;
	while (i < max_bot)
	{
		j = 0;
		while (j < max_ri)
		{
			printf("%c", was_here[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
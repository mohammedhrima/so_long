#include <stdio.h>
#include <stdlib.h>

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

char *arr[] = {
	"111111111111111111",
	"1010100P0111111111",
	"100001000000101001",
	"100C00000000001001",
	"10000000000000E001",
	"111111111111111111",
	NULL,
};

char *join_char_to_str(char *str, char c)
{
	int i;
	char *res;

	i = 0;
	if (str)
		res = malloc((ft_strlen(str) + 1) * sizeof(char));
	else
		res = malloc(2 * sizeof(char));
	while (str && str[i])
	{
		res[i] = str[i];
		i++;
	}
	res[i] = c;
	i++;
	res[i] = '\0';
	// free(str);
	return (res);
}

int move(int Player[], int Collectible[], int w, int h, char **was_here)
{
	if (Player[0] == Collectible[0] && Player[1] == Collectible[1])
	{
		printf("I got it\n");
		return (0);
	}
	int x = Player[0];
	int y = Player[1];
	int n = -1;

	if (x < w - 1 && !(*was_here)[y * w + x + 1])
	{
		Player[0] = x + 1;
		Player[1] = y;
		(*was_here)[y * w + x + 1] = 1;
		n = move(Player, Collectible, w, h, was_here);
		if (n == 0)
		{
			printf("right ");
			return n;
		}
	}
	if (x > 0 && !(*was_here)[y * w + x - 1])
	{
		Player[0] = x - 1;
		Player[1] = y;
		Player[0] = Player[0] - 1;
		(*was_here)[y * w + x - 1] = 1;
		n = move(Player, Collectible, w, h, was_here);
		if (n == 0)
		{
			printf("left ");
			return n;
		}
	}
	if (y < h - 1 && !(*was_here)[(y + 1) * w + x])
	{
		Player[0] = x;
		Player[1] = y + 1;
		(*was_here)[(y + 1) * w + x] = 1;
		n = move(Player, Collectible, w, h, was_here);
		if (n == 0)
		{
			printf("bottom ");
			return n;
		}
	}
	if (y > 0 && !(*was_here)[(y - 1) * w + x])
	{
		Player[0] = x;
		Player[1] = y - 1;
		Player[1] = Player[1] - 1;
		(*was_here)[(y - 1) * w + x] = 1;
		n = move(Player, Collectible, w, h, was_here);
		if (n == 0)
		{
			printf("top ");
			return n;
		}
	}
	return (n);
}

int get_to_the_target(int w, int h, int Player[], int Collectible[])
{
	char *was_here = ft_calloc(w * h, sizeof(unsigned char));
	int x = Player[0];
	int y = Player[1];
	was_here[y * w + x] = 1;
	int n = 0;
	n = move(Player, Collectible, w, h, &was_here);

	printf("\n\n");
	int i = 0;
	while (i < h - 1)
	{
		int j = 0;
		while (j < w - 1)
		{
			printf("%d ", was_here[i * w + j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("\n\n");
	return (n);
}

int main(void)
{
	int j = 0;
	int i = 0;
	int Player[2];
	int Collectible[2];
	int Exit[2];

	while (arr[j])
	{
		i = 0;
		while (arr[j][i])
		{
			if (arr[j][i] == 'P')
			{
				printf("Player (%d, %d)\n", i, j);
				Player[0] = i;
				Player[1] = j;
			}
			if (arr[j][i] == 'C')
			{
				printf("Collectible (%d, %d)\n", i, j);
				Collectible[0] = i;
				Collectible[1] = j;
			}
			if (arr[j][i] == 'E')
			{
				printf("Exit (%d, %d)\n", i, j);
				Exit[0] = i;
				Exit[1] = j;
			}
			i++;
		}
		j++;
	}
	printf("width: %d\nheight: %d\n", i, j);
	int n = get_to_the_target(i, j, Player, Collectible);
	//	printf("%d\n", n);
}
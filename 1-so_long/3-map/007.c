#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ft_putchar(int c)
{
	write(1, &c, 1);
}

void ft_putstr(char *str)
{
	int i = 0;
	while (str && str[i])
	{
		write(1, str + i, 1);
		i++;
	}
}

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

char *ft_strjoin(char *s1, char *s2)
{
	char *str;
	int i;

	i = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	str = ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	if (!str)
	{
		write(1, "Error in strjoin\n", 18);
		return (NULL);
	}
	while (s1 && *s1)
		str[i++] = *s1++;
	while (s2 && *s2)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}

int move(int Player[], int Collectible[], int w, int h, char **was_here, char *path)
{
	if (Player[0] == Collectible[0] && Player[1] == Collectible[1])
	{
		printf("Player: (%d, %d)\n", Player[0], Player[1]);
		printf("I got it : %s\n", path);
		return (0);
	}
	int x = Player[0];
	int y = Player[1];
	int n = -1;

	if (x < w - 1 && (*was_here)[y * w + x + 1] != '1' && (*was_here)[y * w + x + 1] != 'P')
	{
		Player[0] = x + 1;
		Player[1] = y;
		(*was_here)[y * w + x + 1] = 'P';
		printf("Right (%d, %d) | ", Player[0], Player[1]);
		n = move(Player, Collectible, w, h, was_here, path);
		if (n == 0)
		{
			// char *tmp = ft_strjoin(path, "R");
			// path = ft_strdup(tmp);
			ft_putchar('R');
			
			return n;
		}
	}
	if (x > 1 && (*was_here)[y * w + x - 1] != '1' && (*was_here)[y * w + x - 1] != 'P')
	{
		Player[0] = x - 1;
		Player[1] = y;
		(*was_here)[y * w + x - 1] = 'P';
		printf("Left (%d, %d) | ", Player[0], Player[1]);
		n = move(Player, Collectible, w, h, was_here, path);
		if (n == 0)
		{
			// char *tmp = ft_strjoin(path, "L");
			// path = ft_strdup(tmp);
			ft_putchar('L');
			return n;
		}
	}
	if (y < h - 1 && (*was_here)[(y + 1) * w + x] != '1' && (*was_here)[(y + 1) * w + x] != 'P')
	{
		Player[0] = x;
		Player[1] = y + 1;
		(*was_here)[(y + 1) * w + x] = 'P';
		printf("Down (%d, %d) | ", Player[0], Player[1]);
		n = move(Player, Collectible, w, h, was_here, path);
		if (n == 0)
		{
			// char *tmp = ft_strjoin(path, "R");
			// path = ft_strdup(tmp);
			ft_putchar('D');
			return n;
		}
	}
	if (y > 1 && (*was_here)[(y - 1) * w + x] != '1' && (*was_here)[(y - 1) * w + x] != 'P')
	{
		Player[0] = x;
		Player[1] = y - 1;
		(*was_here)[(y - 1) * w + x] = 'P';
		printf("Up (%d, %d) |", Player[0], Player[1]);
		n = move(Player, Collectible, w, h, was_here, path);
		if (n == 0)
		{
			// char *tmp = ft_strjoin(path, "R");
			// path = ft_strdup(tmp);
			ft_putchar('U');
			return n;
		}
	}
	return (n);
}
char *arr[] = {
	"111111111111111111",
	"1P0000000011111111",
	"100011000000101001",
	"111011100000001001",
	"1010000000C0001001",
	"111111111111111111",
	NULL,
};
char *build_string(char *arr[])
{
	int i = 0;
	char *str = NULL;
	char *tmp = NULL;

	while (arr && arr[i])
	{
		tmp = ft_strjoin(str, arr[i]);
		free(str);
		str = tmp;
		i++;
	}
	return (str);
}

int get_to_the_target(int w, int h, int Player[], int Collectible[])
{
	char *was_here = build_string(arr);
	int x = Player[0];
	int y = Player[1];
	int i = 0;
	while (i < h)
	{
		int j = 0;
		while (j < w)
		{
			ft_putchar(was_here[i * w + j]);
			j++;
		}
		ft_putchar('\n');
		i++;
	}
	ft_putchar('\n');
	int n = 0;
	char *str = ft_strdup("  ");
	n = move(Player, Collectible, w, h, &was_here, str);
	//printf("-> %s\n", str);
	ft_putchar('\n');
	i = 0;
	while (i < h)
	{
		int j = 0;
		while (j < w)
		{
			ft_putchar(was_here[i * w + j]);
			j++;
		}
		ft_putchar('\n');
		i++;
	}
	ft_putstr("\n");
	return (n);
}

int main(void)
{

	int i = 0;
	int Player[2];
	int Collectible[2];
	int Exit[2];
	int j = 0;
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
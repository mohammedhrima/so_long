#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// WINDOW SETTING
static int WINDOW_WIDTH;
static int WINDOW_HEIGHT;

int ft_strlen(char *map)
{
	int i;

	i = 0;
	while (map && map[i])
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
char *ft_strdup(char *s)
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
int c = 0;
int move(int Player, int Collectlen, int Exit, int w, int h, char **was_here, char *map)
{
	int x = Player % w;
	int y = Player / w;
	int n = 0;
	int i = 0;

	(*was_here)[Player] = 'P';

	if ((*was_here)[Exit] != 'P')
		n = -1;
	if (map[y * w + x] == 'C')
		c++;
	if (c != Collectlen)
		n = -1;

	if (x < w - 1 && (*was_here)[y * w + x + 1] != '1' && (*was_here)[y * w + x + 1] != 'P' && n < 0)
		n = move(y * w + x + 1, Collectlen, Exit, w, h, was_here, map);
	if (x > 1 && (*was_here)[y * w + x - 1] != '1' && (*was_here)[y * w + x - 1] != 'P' && n < 0)
		n = move(y * w + x - 1, Collectlen, Exit, w, h, was_here, map);
	if (y < h - 1 && (*was_here)[(y + 1) * w + x] != '1' && (*was_here)[(y + 1) * w + x] != 'P' && n < 0)
		n = move((y + 1) * w + x, Collectlen, Exit, w, h, was_here, map);
	if (y > 1 && (*was_here)[(y - 1) * w + x] != '1' && (*was_here)[(y - 1) * w + x] != 'P' && n < 0)
		n = move((y - 1) * w + x, Collectlen, Exit, w, h, was_here, map);
	return (n);
}
void *ft_memset(void *b, int c, size_t len)
{
	size_t i;
	unsigned char *ptr;

	ptr = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		*ptr = c;
		ptr++;
		i++;
	}
	return (b);
}

int main(int argc, char **argv)
{
	WINDOW_WIDTH = 30;
	WINDOW_HEIGHT = 30;
	int to_find[255];
	int to_find2[255];
	to_find['C'] = 4;
	to_find['E'] = 1;
	to_find['P'] = 1;
	to_find2['C'] = 0;
	to_find2['E'] = 0;
	to_find2['P'] = 0;

	int i = 0;
	int j = 0;
	srand(time(0));
	int ran = rand();
	int stop = ran % (WINDOW_HEIGHT - 2);
	char *map = NULL;
	char *to_draw;
	char *tmp;
	int num;

	if (WINDOW_HEIGHT * WINDOW_WIDTH > 1000)
		to_draw = "01CE01PCE01CEP01CEP01CPE01CPE";
	else if (WINDOW_HEIGHT * WINDOW_WIDTH > 550)
		to_draw = "0100CE01P00CE01CEP0001CEP0001CPE0001C00PE";
	else if (WINDOW_HEIGHT * WINDOW_WIDTH > 100)
		to_draw = "01CE01PCE01CEP01CEP01CPE01CPE";
	else
		to_draw = "000000ECCCPCCEEPPPP00"; // 01CE01PCE01CEP01CEP01CPE01CPE

	map = ft_calloc(WINDOW_HEIGHT * WINDOW_WIDTH + 1, sizeof(char));
	ft_memset(map, '0', WINDOW_HEIGHT * WINDOW_WIDTH);
	while (i < WINDOW_WIDTH)
	{
		map[i] = '1';
		i++;
	}
	j = 1;
	while (j < WINDOW_HEIGHT - 1)
	{
		i = 0;
		map[j * WINDOW_WIDTH + i] = '1';
		i++;
		while (i < WINDOW_WIDTH - 2)
		{
			num = rand() % ft_strlen(to_draw);
			while ((to_draw[num] == 'P' || to_draw[num] == 'E' || to_draw[num] == 'C') && to_find[to_draw[num]] == 0)
				num = rand() % ft_strlen(to_draw);
			if ((to_draw[num] == 'P' || to_draw[num] == 'E' || to_draw[num] == 'C') && to_find[to_draw[num]] && !stop)
			{
				// printf("%c: (%d, %d)\n", to_draw[num], i, j);
				map[j * WINDOW_WIDTH + i] = to_draw[num];
				to_find[to_draw[num]]--;
				stop = ran % j;
				if (to_draw[num] != 'C')
				{
					to_find2[to_draw[num]] = j * WINDOW_WIDTH + i;
					//printf("%c: (%d, %d) ==> to_find2[%d] value: %d | %d\n", to_draw[num], i, j, to_draw[num], to_find2[to_draw[num]], j * WINDOW_WIDTH + i);
				}
				if (to_draw[num] == 'C')
				{
					to_find2[to_draw[num]] = to_find2[to_draw[num]] + 1;
					//printf("%c: (%d, %d) ==> to_find2[%d] value: %d | %d\n", to_draw[num], i, j, to_draw[num], to_find2[to_draw[num]], to_find2[to_draw[num]] + 1);
				}
			}
			else if (to_draw[num] == '0')
				map[j * WINDOW_WIDTH + i] = '0';
			else if (to_draw[num] == '1')
				map[j * WINDOW_WIDTH + i] = '1';
			else
				map[j * WINDOW_WIDTH + i] = '0';
			i++;
		}
		map[j * WINDOW_WIDTH + i] = '1';
		j++;
		stop--;
	}
	i = 0;
	while (i < WINDOW_WIDTH)
	{
		map[j * WINDOW_WIDTH + i] = '1';
		i++;
	}
	if (to_find['E'] == 0 && to_find['P'] == 0 && to_find2['C'] != 0)
	{
		int fd = open("./test.ber", O_RDWR | O_CREAT | O_TRUNC, 0777);
		j = 0;
		while (j < WINDOW_HEIGHT)
		{
			i = 0;
			while (i < WINDOW_WIDTH - 1)
			{
				write(fd, map + j * WINDOW_WIDTH + i, 1);
				i++;
			}
			write(fd, "\n", 1);
			j++;
		}
		close(fd);
		char *map_copy;
		int n;
		map_copy = ft_strdup(map);
		//printf("index of Player: %d\nindex of Exit: %d\nCollectible len: %d\nWindow width: %d\nWindow height: %d\nmap len: %d\nmap is: %s\n", to_find2['P'], to_find2['C'], to_find2['E'], WINDOW_WIDTH, WINDOW_HEIGHT, ft_strlen(map), map);
		n = move(to_find2['P'], to_find2['C'], to_find2['E'], WINDOW_WIDTH, WINDOW_HEIGHT, &map_copy, map);
		if (n < 0)
		{
			free(map_copy);
			printf("Path is not valid\n");
			printf("Regenerate map\n");
		}
		else
		{
			printf("Path is valid\n");
			fd = open("./map.ber", O_RDWR | O_CREAT | O_TRUNC, 0777);
			j = 0;
			while (j < WINDOW_HEIGHT)
			{
				i = 0;
				while (i < WINDOW_WIDTH - 1)
				{
					write(fd, map + j * WINDOW_WIDTH + i, 1);
					i++;
				}
				write(fd, "\n", 1);
				j++;
			}
			close(fd);
		}
		fd = open("./map_expanded.ber", O_RDWR | O_CREAT | O_TRUNC, 0777);
		j = 0;
		while (j < WINDOW_HEIGHT)
		{
			i = 0;
			while (i < WINDOW_WIDTH - 1)
			{
				write(fd, map_copy + j * WINDOW_WIDTH + i, 1);
				i++;
			}
			write(fd, "\n", 1);
			j++;
		}
		close(fd);
	}
	else
	{
		printf("Regenerate map\n");
	}

	return (0);
}
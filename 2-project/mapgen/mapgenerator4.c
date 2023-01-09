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
int m = 0;

int is_access(char c, int is_equal)
{
	return (c == '0' || c == 'C' || c == 'E' || c == 'X');
}

int move(int Player, int Collectlen, int Exit, int w, int h, char **was_here)
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
	if ((*was_here)[Exit] != 'F')
		n = -1;
	if (m != Collectlen)
		n = -1;
	if (n < 0 && is_access((*was_here)[y * w + x + 1], m == Collectlen))
		n = move(y * w + x + 1, Collectlen, Exit, w, h, was_here);
	if (n < 0 && is_access((*was_here)[y * w + x - 1], m == Collectlen))
		n = move(y * w + x - 1, Collectlen, Exit, w, h, was_here);
	if (n < 0 && is_access((*was_here)[(y + 1) * w + x], m == Collectlen))
		n = move((y + 1) * w + x, Collectlen, Exit, w, h, was_here);
	if (n < 0 && is_access((*was_here)[(y - 1) * w + x], m == Collectlen))
		n = move((y - 1) * w + x, Collectlen, Exit, w, h, was_here);
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

int Generate_map(int test, int WINDOW_WIDTH, int WINDOW_HEIGHT, int max_collectible)
{
	printf("Test %d, Generating map...\n", test);
	int to_find[255];
	int to_find2[255];
	to_find['C'] = max_collectible;
	to_find['E'] = 1;
	to_find['P'] = 1;
	to_find['X'] = 10;
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
	char *map_copy;
	int n;
	int fd;

	// if (WINDOW_HEIGHT * WINDOW_WIDTH > 1000)
	// 	to_draw = "01CE01PCE01CEP01CEP01CPE01CPE";
	// else if (WINDOW_HEIGHT * WINDOW_WIDTH > 550)
	// 	to_draw = "0100CE01P00CE01CEP0001CEP0001CPE0001C00PE";
	// else if (WINDOW_HEIGHT * WINDOW_WIDTH > 100)
	// 	to_draw = "01CE01PCE01CEP01CEP01CPE01CPE";
	// else
	// 	to_draw = "01110EC11CC1PC11CEEP11P1PP00"; // 01CE01PCE01CEP01CEP01CPE01CPE
	to_draw = "01C0EC1C0CC1PCE0C1C01CECCP01CC1CEPCC0101CPE01CPE";
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
			while ((to_draw[num] == 'P' || to_draw[num] == 'E' || to_draw[num] == 'C' || to_draw[num] == 'X') && to_find[to_draw[num]] == 0)
				num = rand() % ft_strlen(to_draw);
			if ((to_draw[num] == 'P' || to_draw[num] == 'E' || to_draw[num] == 'C' || to_draw[num] == 'X') && to_find[to_draw[num]] && !stop)
			{
				// printf("%c: (%d, %d)\n", to_draw[num], i, j);
				map[j * WINDOW_WIDTH + i] = to_draw[num];
				to_find[to_draw[num]]--;
				stop = ran % j;
				if (to_draw[num] != 'C')
				{
					to_find2[to_draw[num]] = j * WINDOW_WIDTH + i;
					// printf("%c: (%d, %d) ==> to_find2[%d] value: %d | %d\n", to_draw[num], i, j, to_draw[num], to_find2[to_draw[num]], j * WINDOW_WIDTH + i);
				}
				if (to_draw[num] == 'C')
				{
					to_find2[to_draw[num]] = to_find2[to_draw[num]] + 1;
					// printf("%c: (%d, %d) ==> to_find2[%d] value: %d | %d\n", to_draw[num], i, j, to_draw[num], to_find2[to_draw[num]], to_find2[to_draw[num]] + 1);
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

		map_copy = ft_strdup(map);
		// printf("index of Player: %d\nindex of Exit: %d\nCollectible len: %d\nWindow width: %d\nWindow height: %d\nmap len: %d\nmap is: %s\n", to_find2['P'], to_find2['C'], to_find2['E'], WINDOW_WIDTH, WINDOW_HEIGHT, ft_strlen(map), map);
		n = move(to_find2['P'], to_find2['C'], to_find2['E'], WINDOW_WIDTH, WINDOW_HEIGHT, &map_copy);
		if (n < 0)
		{
			free(map);
			free(map_copy);
			printf("Path is not valid\n");
			if (test < 10)
				printf("Retry...\n");
			return (-1);
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
			printf("Check file \"map.ber\" it contains the map\n");
			printf("Check file \"map_expanded.ber\" it contains till where the Player did expand\n");
		}
	}
	else
	{
		free(map);
		printf("Map is invalid\n");
		if (test < 10)
			printf("Retry...\n");
		return (-1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	int i = 0;
	while (Generate_map(i, 100, 100, 1) < 0 && i < 10)
	{
		sleep(1);
		i++;
	}

	return (0);
}
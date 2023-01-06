#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd = open("map.ber", O_RDWR | O_CREAT | O_TRUNC, 0777);
	int i = 0;
	int j = 0;
	int num;
	int width = 100;
	int height = 100;
	int Collect = 3;
	int Exit = 1;
	int Player = 1;
	time_t seconds;
	char *to_draw = "01P01E01C01";
	int last_width = width;

	while (i < width)
	{
		write(fd, "1", 1);
		i++;
	}
	while (j < height - 2)
	{
		write(fd, "\n1", 2);
		i = 0;
		while (i < width - 2)
		{
			seconds = time(NULL);
			num = (7 * rand() + 5 * seconds + 3 * width + 2 * height) % 12;
			while (to_draw[num] == 'P' && !Player)
			{
				seconds = time(NULL);
				num = (2 * rand() + 3 * seconds + 5 * width + 7 * height) % 12;
			}
			while (to_draw[num] == 'E' && !Exit)
			{
				seconds = time(NULL);
				num = (5 * rand() + 7 * seconds + 3 * width + 2 * height) % 12;
			}
			while (to_draw[num] == 'C' && !Collect)
			{
				seconds = time(NULL);
				num = (3 * rand() + 7 * seconds + 5 * width + 2 * height) % 12;
			}
			if (to_draw[num] == 'P' && Player && last_width != j)
			{
				write(fd, "P", 1);
				Player--;
				last_width = j;
			}
			else if (to_draw[num] == 'E' && Exit && last_width != j)
			{
				write(fd, "E", 1);
				Exit--;
				last_width = j;
			}
			else if (to_draw[num] == 'C' && Collect && last_width != j)
			{
				write(fd, "C", 1);
				Collect--;
				last_width = j;
			}
			else if (to_draw[num] == '0')
				write(fd, "0", 1);
			else if (to_draw[num] == '1')
				write(fd, "1", 1);
			else
				write(fd, "0", 1);
			i++;
		}
		write(fd, "1", 1);
		j++;
	}
	write(fd, "\n", 1);
	i = 0;
	while (i < width)
	{
		write(fd, "1", 1);
		i++;
	}
	close(fd);
}
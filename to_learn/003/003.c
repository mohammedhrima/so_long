#include <mlx.h>
#include <stdio.h>

typedef struct	s_vars {
	void	*mlx;
	void	*win;
}				t_vars;

int func()
{
	static int a;
	a++;
	printf("hello %d\n", a);
	return 0;
}

int draw(t_vars *var)
{
	int x = 100;
	int y;
	while(x < 400)
	{
		y = 200;
		while(y < 400)
		{
			mlx_pixel_put(var->mlx, var->win, x, y, 0xff0000);
			y++;
		}
		x++;
	}
	return 0;
}

int main(void)
{
	t_vars var;

	var.mlx = mlx_init();
	var.win = mlx_new_window(var.mlx, 1000, 600, "test");
	mlx_key_hook(var.win, func, &var);
	mlx_loop_hook(var.mlx, draw, &var);
	mlx_loop(var.mlx);
}
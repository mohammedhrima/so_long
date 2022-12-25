#include <mlx.h>
#include <stdio.h>

typedef struct	s_vars {
	void	*ptr;
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
	while(x < 400)
	{
		mlx_pixel_put(var->ptr, var->win, x, 200, 0xff0000);
		x++;
	}
	return 0;
}

int main(void)
{
	t_vars var;

	var.ptr = mlx_init();
	var.win = mlx_new_window(var.ptr, 1000, 600, "test");
	mlx_key_hook(var.win, func, &var);
	mlx_loop_hook(var.ptr, draw, &var);
	mlx_loop(var.ptr);
}
#include <mlx.h>
#include <stdio.h>

typedef struct	s_vars {
	void	*mlx;
	void	*win;
}				t_vars;

int	key_hook(int keycode, t_vars *vars)
{
	printf("Hello from key_hook!\n");
	return (0);
}

void draw_rect(t_vars *vars, int min_x, int min_y, int max_x, int max_y, int color)
{
	for (int y = min_y; y < max_y; y++)
	{
		for (int x = min_x; x < max_x; x++)
		{
			mlx_pixel_put(vars->mlx, vars->win, x, y, color);
		}
	}
}

/*void draw_circle(t_vars *vars, int cx, int cy, int r, int color)
{

}*/

int loop_hook(t_vars *vars)
{
	static int x = 0;
	static int y = 20;
	draw_rect(vars, 0, 0, 640, 480, 0);
	
	//draw_circle(vars, 100, 100, 20, 0xff00);
	draw_rect(vars, x, y, x + 50, y + 50, 0xff0000);
	x += 10;
	x %= 480;
	return 0;
}

int	main(void)
{
	t_vars	vars;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 640, 480, "Hello world!");
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_loop_hook(vars.mlx, loop_hook, &vars);
	mlx_loop(vars.mlx);
}
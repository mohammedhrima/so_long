
#include "mlx.h"

int main(void)
{
	void *mlx_ptr;
	void *win_ptr;

	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, 300, 300, "hello");
	mlx_pixel_put(mlx_ptr, win_ptr, 250,250,0x2fd59f);
	
	mlx_loop(mlx_ptr);
}
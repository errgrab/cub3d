#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include "mlx.h"

int	keypress(void *mlx)
{
}

int	quit(void *mlx)
{
	mlx_destroy_window(mlx, win);
	mlx_destroy_display(mlx);
	free(mlx);
	exit(0);
	return (0);
}

int main(void)
{
	void	*mlx;
	mlx = mlx_init();
	void	*win;
	win = mlx_new_window(mlx, 500, 500, "title");
	mlx_hook(win, DestroyNotify, StructureNotifyMask, quit, mlx);
	mlx_loop(mlx);
	return (0);
}

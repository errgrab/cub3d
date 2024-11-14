/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:10:01 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/11/11 17:36:08 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>     // malloc, free, size_t, exit, etc...
# include <fcntl.h>		 // open
# include <stdio.h>      // printf
# include <sys/time.h>   // gettimeofday
# include <unistd.h>     // read, write

# define __USE_MISC      // Math constants (M_PI)
# include <math.h>       // (almost all of them)

# include "mlx.h"        // mlx_*
# include "ft_alloc.h"   // ft_calloc, ft_free, ft_clean // good stuff

# define RIGHT_ARROW 65363
# define LEFT_ARROW 65361

# define SPEED 3
# define ROT_SPEED 3

# define WIN_WIDTH 800
# define WIN_HEIGHT 600
//define WIN_WIDTH 200
//define WIN_HEIGHT 200

// Parsing defines
# define MAP_SIZE_MAX 1048576
# define ALLOWED_CHARS "01NESW "
# define MOVE_AREA "0"
# define USER_POS "NESW"
# define WALL "1"
# define EMPTY_SPACE " "
# define PLY_CONST 0.66

/******************************************************************************/

enum e_events
{
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_DESTROY = 17,
};

typedef struct s_img	t_img;
struct s_img
{
	void	*ptr;
	int		width;
	int		height;
	char	*data;
	int		bpp;
	int		sl;
	int		endian;
};

typedef struct s_mlx	t_mlx;
struct s_mlx
{
	void	*ptr;
	void	*win;
	int		width;
	int		height;
	char	*title;
};

typedef struct s_usr	t_usr;
struct s_usr
{
	float	posx;
	float	posy;
	float	dirx;
	float	diry;
	float	plx;
	float	ply;
};

typedef struct s_map	t_map;
struct s_map
{
	int		*data;
	int		width;
	int		height;
	int		ceil_color;
	int		floor_color;
	t_img	north;
	t_img	south;
	t_img	east;
	t_img	west;
};

typedef struct s_time	t_ime;
struct s_time
{
	float	fps;
	float	dt;
};

typedef struct s_g		t_g;
struct s_g
{
	t_mlx	mlx;
	t_img	frame;
	t_map	map;
	t_usr	usr;
	t_ime	time;
	int		argc;
	char	**argv;
	int		key_pressed[177];
	int		exit_status;
	int		debug;
};

typedef struct s_dda	t_dda;
struct s_dda
{
	float	rdx;
	float	rdy;
	float	ddx;
	float	ddy;
	float	sdx;
	float	sdy;
	float	wdist;
	int		x;
	int		mx;
	int		my;
	int		sx;
	int		sy;
	int		hit;
	int		side;
	int		dstart;
	int		dend;
	int		offset;
};

typedef struct s_parsing
{
	t_map	*map;
	t_usr	*usr;
	t_mlx	*mlx;
	int		fd;
	char	buffer[MAP_SIZE_MAX];
	char	**buffer_split;
	int		ceiling_color;
	int		floor_color;
	char	**map_temp;
	int		map_start;
	int		map_end;
}	t_parsing;

/******************************************************************************/

// main.c
int		get_map_value(int x, int y);

// time.c
size_t	time_now(void);
void	time_update(void);

// global.c
t_g		*g(void);

// init.c
void	init_frame(void);
void	init_window(void);
int		init_map_tmp(void);

// event.c
int		event_quit(void);
int		event_keydown(int keycode);
int		event_keyup(int keycode);

// raycast.c
t_dda	raycast_start(int x);
void	raycast_hit(t_dda *dda);
void	raycast_draw(t_dda *dda, t_img *img);
void	raycast(void);

// action.c
void	rotate(float speed);
void	move(float speed);

// draw.c
void	put_pixel(int x, int y, int color);
void	draw_vertical_line(int x, int ystart, int yend, int color);
int		darken_color(int color, float factor);

// ft_memmove.c
void	*ft_memmove(void *dest, const void *src, size_t n);
char	*ft_strstr(const char *s1, const char *s2);

// ft_calloc.c
void	*ft_calloc(size_t nmemb, size_t size);

// parsing.c
int		parsing(void);
int		get_data(t_parsing *parsing_data);
int		write_error(int type, t_parsing *parsing_data);
void	parsing_free(t_parsing *parsing_data, int images);
int		convert_map(t_parsing *p_data);

// parsing_ceiling_floor_color.c
int		get_ceiling_floor_color(t_parsing *parsing_data);
int		parse_int(char *line);
int		parse_color(char *line);

// parsing_texture.c
int		get_textures(t_parsing *parsing_data);
int		load_texture(char *file, t_parsing *p_data, char *type);
int		check_line(char *buffer_split, t_parsing *p_data);

// parsing_map.c
int		get_map(t_parsing *p_data);
void	get_map_dimensions(t_parsing *p_data);
int		set_map_temp(t_parsing *p_data);
int		check_map(t_parsing *p_data);
int		check_path(t_parsing *p_data, int i, int j);

// parsing_usr.c
int		get_usr(t_parsing *p_data);
void	get_user_info(t_parsing *p_data, int y, int x);

// parsing_utils.c
void	*ft_calloc2(size_t nmemb, size_t size);
int		ft_strlen(char *s);
char	*ft_strchr(char *s, int c);
void	*ft_memset(void *str, int c, size_t n);

// parsing_utils2.c
void	remove_first_line(char *buffer);
char	**ft_split(char *str, char c);
void	ft_array_free(char **array);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif // CUB3D_H

MLX		= minilibx-linux
SRCS	= $(wildcard *.c)
ODIR	= objs
OBJS	= $(SRCS:%.c=$(ODIR)/%.o)
CFLAGS	= -O3 -Wall -Wextra -Werror -pedantic -I. -I$(MLX)
LDFLAGS	= -L$(MLX)
LDLIBS	= -lmlx -lX11 -lXext -lm
NAME	= cub3d

all: $(NAME) test

test: $(NAME)
	./$(NAME) test_maps/t2.fdf

$(MLX):
	@make -sC $@

$(NAME): $(OBJS) | $(LIB) $(MLX)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM)r $(ODIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) checker

re: fclean all

$(ODIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all bonus clean fclean re $(LIB) $(MLX)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0 -Wno-unused-variable -Wno-unused-parameter
NAME = MiniRT

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=objs/%.o)
HEADER = $(wildcard includes/*.h)

LIBS = -lm -L./mlx -lmlx -lX11 -lXext -L./Libft -lft

all: $(NAME)

$(NAME): $(OBJS)
	make -C Libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

objs/%.o: %.c $(HEADERS)
	@mkdir -p objs
	$(CC) $(CFLAGS) -c $< -o $@ -DLINE_EQ

clean:
	make clean -C Libft
	rm -rf objs

fclean: clean
	make fclean -C Libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
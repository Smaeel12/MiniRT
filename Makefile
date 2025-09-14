CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O3
NAME = MiniRT

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=objs/%.o)
HEADER = $(wildcard includes/*.h)

LIBS = -lm -L. -lmlx -lX11 -lXext

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

objs/%.o: %.c $(HEADERS)
	@mkdir -p objs
	$(CC) $(CFLAGS) -c $< -o $@ -DLINE_EQ

clean:
	rm -rf objs

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
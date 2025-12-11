CC := cc
NAME := MiniRT
LIBS := -lm -LLibft -lft -lmlx -lX11 -lXext
CFLAGS := -W -Werror -Wextra

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=objs/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS += -g3
# CFLAGS += -fsanitize=address
CFLAGS += -Wno-unused-parameter
CFLAGS += -MMD -MP

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C Libft
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

objs/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C Libft
	rm -rf objs/

fclean: clean
	make fclean -C Libft
	rm -rf $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all fclean clean re

CC := cc
NAME := MiniRT
LIBS := -lm -LLibft -lft
CFLAGS := -W -Werror -Wextra

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=objs/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS += -g3 -fsanitize=address -Wno-unused-parameter
CFLAGS += -MMD -MP

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C Libft
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

objs/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf objs/

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all fclean clean re

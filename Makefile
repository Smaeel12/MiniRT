
NAME    := miniRT
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -Iincludes -Imlx -Ilibft
LDFLAGS := -Lmlx -lmlx -L/usr/lib -lXext -lX11 -lm -Llibft -lft

SRC_DIR := srcs
OBJ_DIR := objs

SRC := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/window.c \
	$(SRC_DIR)/hooks.c

OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	./$(NAME)

.PHONY: all clean fclean re run

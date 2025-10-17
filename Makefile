NAME    := miniRT
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -Iincludes -Imlx -Ilibft
LDFLAGS := -Lmlx -lmlx -L/usr/lib -lXext -lX11 -lm -Llibft -lft

SRC_DIR := srcs
OBJ_DIR := objs

LIBFT_DIR := libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

SRC := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/window.c \
	$(SRC_DIR)/hooks.c \
	$(SRC_DIR)/render.c \
	$(SRC_DIR)/parse_utils.c

OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)


$(NAME): $(OBJ) $(LIBFT_A)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $(OBJ_DIR)/$*.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean || true

re: fclean all

run: all
	./$(NAME)

.PHONY: all clean fclean re run

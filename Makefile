NAME = minishell

LSRC = ./src

INCLUDE = .

SRCS = $(LSRC)/parser/env.c $(LSRC)/parser/expansion.c $(LSRC)/execution/main.c $(LSRC)/syntax_manager/syntax.c $(LSRC)/parser/tokenize.c $(LSRC)/execution/utils.c $(LSRC)/parser/command.c $(LSRC)/execution/executor.c $(LSRC)/execution/pipes.c $(LSRC)/built-ins/builtin_parser.c $(LSRC)/built-ins/export.c $(LSRC)/built-ins/unset.c $(LSRC)/built-ins/ch_dir.c $(LSRC)/built-ins/pwd.c $(LSRC)/built-ins/echo.c $(LSRC)/execution/heredoc.c $(LSRC)/test/print.c $(LSRC)/signals/signals.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -I$(LSRC) -I$(INCLUDE) -I/usr/include
LDFLAGS = -L/usr/lib
LDLIBS = -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

DEBUG = -g #-fsanitize=address
END = \033[m
YELLOW = \033[33m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "\n$(YELLOW)Minishell in the oven...$(END)"
	$(CC)  $(DEBUG) $(LDFLAGS) $(OBJS) $(LIBFT) $(LDLIBS) -o $(NAME)

%.o: %.c minishell.h libft/libft.h Makefile
	@$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "\n$(YELLOW)Construyendo libft...$(END)"
	@make -C $(LIBFT_DIR)

clean:
	@echo "\n$(YELLOW)Limpiando archivos...$(END)"
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "\n$(YELLOW)Limpiando todo...$(END)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re submodule

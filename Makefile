NAME = minishell

LSRC = ./src

INCLUDE = .

SRCS = $(LSRC)/parser/env.c $(LSRC)/parser/expansion.c $(LSRC)/execution/main.c $(LSRC)/syntax_manager/syntax.c $(LSRC)/parser/tokenize.c $(LSRC)/execution/utils.c $(LSRC)/parser/command.c $(LSRC)/execution/executor.c $(LSRC)/built-ins/builtin_parser.c $(LSRC)/built-ins/export.c $(LSRC)/built-ins/unset.c $(LSRC)/execution/heredoc.c $(LSRC)/test/print.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS =  -I$(LSRC) -I$(INCLUDE) -I/usr/include
LDFLAGS = -L/usr/lib
LDLIBS = -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

END = \033[m
YELLOW = \033[33m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "\n$(YELLOW)Minishell in the oven...$(END)"
	$(CC) $(LDFLAGS) $(OBJS) $(LIBFT) $(LDLIBS) -o $(NAME)

%.o: %.c minishell.h libft/libft.h Makefile
	@$(CC) $(CFLAGS) -c $< -o $@

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

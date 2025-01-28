# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eriviere <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/23 17:55:26 by eriviere          #+#    #+#              #
#    Updated: 2025/01/23 17:55:30 by eriviere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Directorios
LSRC = ./src
INCLUDE = .
OBJDIR = ./obj
LIBFT_DIR = libft

END = \033[0m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RED = \033[31m

SRCS = \
    $(LSRC)/parser/env.c \
    $(LSRC)/parser/expansion.c \
    $(LSRC)/parser/expansion_utils.c \
    $(LSRC)/execution/main.c \
    $(LSRC)/syntax_manager/syntax.c \
    $(LSRC)/parser/tokenize.c \
    $(LSRC)/parser/tokenize_utils_1.c \
    $(LSRC)/parser/tokenize_utils_2.c \
    $(LSRC)/execution/utils.c \
    $(LSRC)/parser/command_tokens.c \
    $(LSRC)/parser/command_init.c \
    $(LSRC)/parser/command_exec.c \
    $(LSRC)/parser/command_free.c \
    $(LSRC)/execution/exec_arr.c \
    $(LSRC)/execution/exec_cmd.c \
    $(LSRC)/execution/exec_path.c \
    $(LSRC)/execution/pipes.c \
    $(LSRC)/built-ins/builtin_parser.c \
    $(LSRC)/built-ins/export.c \
    $(LSRC)/built-ins/export_utils.c \
    $(LSRC)/built-ins/unset.c \
    $(LSRC)/built-ins/ch_dir.c \
    $(LSRC)/built-ins/pwd.c \
    $(LSRC)/built-ins/echo.c \
    $(LSRC)/built-ins/exit.c \
    $(LSRC)/execution/heredoc.c \
    $(LSRC)/test/print_commands.c \
    $(LSRC)/test/print_tokens.c \
    $(LSRC)/signals/signals.c \
    $(LSRC)/signals/signals_utils.c

OBJS = $(SRCS:$(LSRC)/%.c=$(OBJDIR)/%.o)

CFLAGS = -Wall -Wextra -Werror -I$(LSRC) -I$(INCLUDE) -I/usr/include
LDFLAGS = -L/usr/lib
LDLIBS = -lreadline
DEBUG = -g -fsanitize=address
HEADERS = $(INCLUDE)/minishell.h

LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_SRCS = $(wildcard $(LIBFT_DIR)/*.c)
LIBFT_HEADER = $(LIBFT_DIR)/libft.h

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) Makefile
	@echo "\n$(BLUE)Linking $(NAME)...$(END)"
	@$(CC) $(DEBUG) $(LDFLAGS) $(OBJS) $(LIBFT) $(LDLIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) built successfully!$(END)"

# Crear objetos
$(OBJDIR)/%.o: $(LSRC)/%.c $(HEADERS) | $(OBJDIR)
	@$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@
	@printf "$(YELLOW)Compiling $<... \r$(END)"

# Crear directorio de objetos
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(dir $(OBJS))

$(LIBFT): $(LIBFT_SRCS) $(LIBFT_HEADER)
	@echo "$(YELLOW)Compiling $<...\r$(END)"
	@make -C $(LIBFT_DIR) --no-print-directory

clean:
	@echo "$(RED)Cleaning minishell object files...$(END)"
	@rm -rf $(OBJDIR)
	@make -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	@echo "$(RED)Cleaning minishell...$(END)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re

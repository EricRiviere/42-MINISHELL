# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gualvare <gualvare@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 14:05:44 by gualvare          #+#    #+#              #
#    Updated: 2024/07/06 12:15:31 by gualvare         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
       ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
       ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
       ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c \
       ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
       ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

AR = ar
ARFLAGS = rcs

# Colores
END = \033[0m
YELLOW = \033[33m
GREEN = \033[32m
RED = \033[31m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Creating $(NAME)...$(END)"
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS)

%.o: %.c libft.h Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(YELLOW)Compiling $<...\r$(END)"


clean:
	@echo "$(RED)Cleaning libft object files...$(END)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(END)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

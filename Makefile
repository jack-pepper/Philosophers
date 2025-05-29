# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/14 09:35:38 by mmalie            #+#    #+#              #
#    Updated: 2025/05/29 22:04:31 by mmalie           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###################################################
### Name of the file obtained after compilation ###
###################################################

NAME = philo

############################
### Compilation settings ###
############################

CC=gcc
CFLAGS=-lpthread -g -Wall -Wextra -Werror

####################
### Dependencies ###
####################

DEPS = philo.h

####################
### Source files ###
####################

SRC = philo.c \
	args_parser.c \
	initer.c \
	mtx_handler.c \
	sim_launcher.c \
	sim_state.c \
	gandalf_barrier.c \
	routine_clock.c \
	routine_philo.c \
	forks_utils.c \
	libft_utils.c \
        philo_utils.c \
	time_utils.c \
	memory.c \

### Object files
OBJ = $(SRC:.c=.o)

##############################################
### Default rules (compile the executable) ###
##############################################

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

######################################
### Compile .c files into .o files ###
######################################

$(OBJ): %.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

#########################
### Cleaning-up rules ###
#########################

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

#########################
### .PHONY Exceptions ###
#########################

.PHONY: all clean fclean re bonus


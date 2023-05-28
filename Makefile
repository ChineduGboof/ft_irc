NAME	=	ircserv
COMPILER=	c++
FLAGS	=	-std=c++98 -Wall -Wextra -Werror
SRC		=	main.cpp
OBJ		=	$(SRC:.cpp=.o)
INC		=	


GREEN ='\033[1;32m'
ORANGE ='\033[1;33m'
NC='\033[0m'

.PHONY: all clean fclean re

all: assembling $(NAME) assembled

%.o: %.cpp $(INC)
	$(COMPILER) $(FLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	@$(COMPILER) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)
	@rm -f */*.out
	@rm -f *.out
	@rm -f */*.gch
	@rm -f *.gch
	@rm -rf *.dSYM
	@rm -rf */*.dSYM
	@echo ${ORANGE}"Clearing"$(NC)

fclean: clean
	@rm -f $(NAME)
	@echo ${GREEN}"Cleared"$(NC)

re: fclean all

run:
	@echo $(GREEN)"Start!"$(NC)
	@./$(NAME) 127.0.0.1:6667:23

nc:
	@echo $(GREEN)"Enter to server new user"$(NC)
	@nc 127.0.0.1 6667

assembling:
	@echo $(ORANGE)"Assembling"$(NC)

assembled:
	@echo $(GREEN)"Assembled"$(NC)

irssi:
	irssi -c 127.0.0.1

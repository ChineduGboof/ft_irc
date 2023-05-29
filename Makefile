NAME	=	ircserv
COMPILER=	c++
FLAGS	=	-std=c++98 -Wall -Wextra #-Werror
SRC		=	main.cpp Server/Utils.cpp Server/Server.cpp
OBJ		=	$(SRC:.cpp=.o)
INC		=	


DEFAULT	=	'\033[0m'
GREEN	=	'\033[1;32m'
ORANGE 	=	'\033[1;33m'

.PHONY: all clean fclean re

all: compiling $(NAME) compiled

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
	@rm -rf */*.DStore
	@echo ${ORANGE}"Cleaning up refuse"$(DEFAULT)

fclean: clean
	@rm -f $(NAME)
	@echo ${GREEN}"All Clean"$(DEFAULT)

re: fclean all


compiling:
	@echo $(ORANGE)"Ft_IRC is being compiled"$(DEFAULT)

compiled:
	@echo $(GREEN)"Ft_IRC is ready"$(DEFAULT)


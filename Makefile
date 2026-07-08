NAME = ircserv
SOURCE = $(shell find ./src -iname "*.cpp")

OBJS = $(SOURCE:.cpp=.o)
CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address
INCLUDE = -I ./include

all:$(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(FLAGS) $(OBJS) $(INCLUDE) -o $(NAME) 

%.o: %.cpp
	@$(CXX) $(FLAGS) $(INCLUDE) -c $< -o $@ && printf "Compiling: $(notdir $<)\n"

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
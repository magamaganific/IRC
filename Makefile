NAME = ircserv
SRCS = main.cpp irc.cpp
OBJS = $(SRCS:.cpp=.o)
CXX = c++ -Wall -Wextra -Werror -std=c++98 -g3

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
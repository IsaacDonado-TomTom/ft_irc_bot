BONUSBOT = bonusbot

SRCS = bonusbot.cpp ArgParser.cpp AutoReply.cpp Connect.cpp

OBJS = $(SRCS:.cpp=.o)

DEBUG_FLAGS = -g3 -fsanitize=address

all: $(BONUSBOT)

$(BONUSBOT): $(OBJS)
	clang++ -o $@ $^  -Wall  -Wextra -std=c++98 -pedantic

%.o: %.cpp
	clang++ -o $@ -c $^ -Wall  -Wextra -std=c++98 -pedantic -fPIC -g

clean:
	-rm -f $(OBJS)

fclean: clean
	-rm -f $(BONUSBOT)

re: fclean all


.PHONY: bonusbot clean fclean re

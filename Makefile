NAME		= ircserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
CPPFLAGS	= -Iinclude -MMD -MP

SRCS_NETWORK	=

SRCS_IRC		=

SRCS		= src/main.cpp $(SRCS_NETWORK) $(SRCS_IRC)
OBJS		= $(SRCS:src/%.cpp=obj/%.o)
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re

-include $(DEPS)

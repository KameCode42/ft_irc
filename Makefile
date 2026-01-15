NAME = ircserv

SRCS =	src/main.cpp \
		src/client/Client.cpp \
		src/channel/Channel.cpp \
		src/commands/Cmd_Cap.cpp \
		src/commands/Cmd_Pass.cpp \
		src/commands/Cmd_Nick.cpp \
		src/commands/Cmd_User.cpp \
		src/commands/Cmd_Ping.cpp \
		src/commands/Cmd_Prvmsg.cpp \
		src/commands/Cmd_Quit.cpp \
		src/commands/Cmd_Join.cpp \
		src/commands/Cmd_Mode.cpp \
		src/commands/Cmd_Topic.cpp \
		src/commands/Cmd_Kick.cpp \
		src/commands/Cmd_Invite.cpp \
		src/commands/Cmd_Part.cpp \
		src/commands/ParseCommands.cpp \
		src/server/Server.cpp 



HEADERS =	inc/Client.hpp \
			inc/Messages.hpp \
			inc/Commands.hpp \
			inc/Channel.hpp \
			inc/Server.hpp

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
OBJDIR = obj

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I./inc

RM = rm -rf

GREEN = \033[0;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "\n$(GREEN)[$(NAME)] Compiled successfully!$(RESET)\n"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJDIR)
	@echo "\n$(GREEN)[$(NAME)] Object files removed!$(RESET)\n"

fclean: clean
	$(RM) $(NAME)
	@echo "\n$(GREEN)[$(NAME)] Executable removed!$(RESET)\n"

re: fclean all

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME) 4242 alefur

term:
	./run_term.sh

tmux:
	./run_tmux.sh


.PHONY: all clean fclean re val

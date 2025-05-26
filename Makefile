RED					= \033[31m
GREEN				= \033[32m
YELLOW				= \033[33m
BLUE				= \033[34m
MAGENTA				= \033[35m
RESET				= \033[0m

NAME				= webserv

CONFIG_FILE			= configurations/new.conf

FILES				= $(shell find ./src -type f -name '*.cpp')

OBJS				= $(FILES:%.cpp=%.o)

HEADERS				= $(addprefix -I, $(shell find . -type d))

COMPILER			= c++

CFLAGS				= -Wall -Wextra -Werror -std=c++98 -g3

all: 				$(NAME)

$(NAME):			$(OBJS)
					@$(COMPILER) $(CFLAGS) $(OBJS) -o $(NAME)
					@echo "$(BLUE)Compiled $(NAME) successfully$(RESET)"

%.o: 				%.cpp
					@$(COMPILER) $(CFLAGS) -c $< -o $@ $(HEADERS)
					@echo "$(GREEN)Compiled$(RESET) $(notdir $<)"

clean:
					@echo "$(RED)Removing$(RESET) $(NAME) objects"
					@rm -rf $(OBJS)

fclean: 			clean
					@echo "$(RED)Removing$(RESET) $(NAME)"
					@rm -rf $(NAME)

re:					fclean all

build:
	@docker build -t webserv .

up:
	docker compose -f docker-compose.yml up --build

rm:
	@docker rmi webserv

logs:
	@docker logs -f $(shell docker ps -q)

sub:
	@bash scripts/submodules.sh

test:
	@bash scripts/build_and_run_tests.sh
	$(call run_colorized_tests)

test_clean:
	@rm -rf build
	$(call print_color, $(RED), "Removing test library")

compile_flags:
	@find ./src -type d | sed 's|^|-I|' > compile_flags.txt

# -----------------------------------------------------------------------------
# Documentation:
#
# To override the default configuration file, you can specify a different file
# by setting the CONFIG_FILE variable when invoking make. For example:
#
#     make CONFIG_FILE=configurations/default.conf
#
# This command tells the build process to use the configuration file located at
# "default.conf" instead of the default one (new.conf). Ensure that the file at 
# "newpath" exists and is correctly formatted for your application.
# -----------------------------------------------------------------------------
run:
	@./webserv $(CONFIG_FILE)

define run_colorized_tests
	@cd build && \
	GTEST_COLOR=1 ctest --output-on-failure -j12
endef

export TERM=xterm-256color

define print_color
	@echo "$(1)$(2)$(RESET)"
endef

.PHONY: test_clean test sub all clean fclean re run build up logs

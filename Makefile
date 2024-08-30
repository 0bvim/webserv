CXX := c++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Iinclude -g3
CXXFLAGS_TEST := -Wall -Wextra -Werror -Iinclude -isystem $(GTEST_DIR)/include

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS += $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
OBJS := $(patsubst $(SRC_DIR)/%%/%.cpp, $(BUILD_DIR)/%%/%.o, $(SRCS))
NAME := $(BIN_DIR)/webserv

.PHONY: all clean fclean re run run2 tests

all: $(NAME)
	@echo "Build complete"

$(NAME): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean all
	@echo "Rebuilding..."

run: all
	@./$(NAME) server.conf

run2: all
	@./$(NAME) arquivo.conf

tests:
	@mkdir -p build && cmake .. && make

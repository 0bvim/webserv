CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -Iinclude -g3
CXXFLAGS_TEST := -Wall -Wextra -Werror -Iinclude -isystem $(GTEST_DIR)/include

SRC_DIR := src
BUILD_DIR := build
TESTS_DIR := tests

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
NAME := webserv

TEST_SRCS := $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TESTS_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_SRCS))
TEST_TARGET := $(TEST_BIN_DIR)/tests

.PHONY: all clean fclean re run tests run_tests

all: $(NAME)
	@echo "Build complete"

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
	@echo "Rebuilding..."

run: all
	@./$(NAME) config/default.conf
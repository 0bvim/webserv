CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -Iinclude -g3
CXXFLAGS_TEST := -Wall -Wextra -Werror -Iinclude -isystem $(GTEST_DIR)/include

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TESTS_DIR := tests
TEST_BUILD_DIR := $(TESTS_DIR)/build
TEST_BIN_DIR := $(TESTS_DIR)/bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
NAME := $(BIN_DIR)/webserv

TEST_SRCS := $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TESTS_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_SRCS))
TEST_TARGET := $(TEST_BIN_DIR)/tests

GTEST_DIR := /usr/src/gtest
GTEST_LIB := /usr/lib/libgtest.a /usr/lib/libgtest_main.a

.PHONY: all clean fclean re run tests run_tests

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
	@./$(NAME) default.conf

run2: all
	@./$(NAME) arquivo.conf

tests: $(TEST_TARGET)

$(TEST_TARGET): $(filter-out $(BUILD_DIR)/main.o,$(OBJS)) $(TEST_OBJS) | $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS_TEST) -pthread -o $@ $^ $(GTEST_LIB)

$(TEST_BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(CXX) $(CXXFLAGS_TEST) -c -o $@ $<

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TEST_BIN_DIR):
	mkdir -p $(TEST_BIN_DIR)

run_tests: tests
	@./$(TEST_TARGET)
	@rm -rf $(TEST_BIN_DIR) $(TEST_BUILD_DIR)

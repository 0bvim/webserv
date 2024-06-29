CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -Iinclude

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TESTS_DIR := tests

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
NAME := $(BIN_DIR)/webserv

TEST_SRCS := $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TESTS_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))
TEST_TARGET := $(BUILD_DIR)/test_bin

GTEST_DIR := /usr/src/gtest
GTEST_LIB := /usr/lib/libgtest.a /usr/lib/libgtest_main.a

.PHONY: all clean tests run_tests

all: $(NAME)
	@echo ""

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
	@echo "Rebuilding...\n"

run: all
	@./$(NAME) $*

tests: $(TEST_TARGET)

$(TEST_TARGET): $(OBJS) $(TEST_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -pthread -o $@ $^ $(GTEST_LIB)

$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR) -c -o $@ $<

run_tests: tests
	./$(TEST_TARGET)
	rm -f $(TEST_TARGET)

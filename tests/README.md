# Tests for WebReq

This directory contains tests for the WebReq project. The tests are organized into directories by categories.

## Directory Structure

- **tests/** - Root directory for all tests
  - **mocks/** - Common mock objects used across tests
  - **parser/** - Tests for the parser functionality
  - **http/** - Tests for HTTP related functionality
  - **network/** - Tests for network related functionality
  - **mime/** - Tests for MIME related functionality
  - **response/** - Tests for response building functionality
  - **utils/** - Tests for utility functions

## Running Tests

To run the tests, follow these steps:

1. Build the project with tests enabled:
   ```bash
   cmake -DBUILD_TESTING=ON ..
   make
   ```

2. Run the tests:
   ```bash
   ctest
   ```

   Or run the test executable directly:
   ```bash
   ./tests/webreq
   ```

## Adding Tests

To add new tests:

1. Create a new test file in the appropriate category directory
2. Add the test file to `tests/CMakeLists.txt`
3. Build and run the tests to make sure they work

## Mocks

Common mock objects are stored in the `mocks/` directory. These include:

- `MockParser.hpp` - Mock for parser functionality
- `mock_connection.hpp` - Mock for connection functionality

Use these mocks when needed to isolate the code being tested. 
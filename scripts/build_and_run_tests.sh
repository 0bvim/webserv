#!/usr/bin/sh

# consider use `make test` command to run this is script.
# makefile consider root path. If you want run this script in terminal
# change dir path to `../build`

DIR=build
BUILD='cmake --build build -j 12'
BUILD_SOURCE="cmake -S . -B build"

execute_command() {
  for cmd in "$@"; do
    echo "Executing: $cmd"
    eval "($cmd)"
  done
}

check_if_test_lib_exist() {
  if [ ! -d $DIR ]; then
    execute_command "mkdir -p $DIR" "$BUILD_SOURCE"
  elif [ -z "$(ls -A "$DIR")" ]; then
      execute_command "$BUILD_SOURCE" "echo @@@@@"
  fi
  echo "google test lib ok..."
}

build_test_lib() {
  execute_command "$BUILD"
}

check_if_test_lib_exist
build_test_lib
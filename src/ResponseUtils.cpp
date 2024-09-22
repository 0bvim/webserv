#include "Response.hpp"

bool  endsWith(const std::string& str, const std::string & suffix) {
  if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool  isInterpreterInstalled(const char *command) {
  // User system() to check if the interpreter is installed
  std::string cmd = command;

  cmd = std::string("which ") + command + " > /dev/null 2>&1";
  int success = system(cmd.c_str());
  return (success == 0);
}

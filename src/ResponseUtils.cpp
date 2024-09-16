#include "Response.hpp"

bool  endsWith(const std::string& str, const std::string & suffix) {
  if (suffix.size() > str.size()) return false;
  return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool  isInterpreterInstalled(const char *command) {
  (void)command;
  return false;
}

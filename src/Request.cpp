/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:29:18 by bmoretti          #+#    #+#             */
/*   Updated: 2024/09/15 15:27:06 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

Request::Request(std::string &str) : _str(str) { this->_parseHTTPRequest(); }

Request::~Request() {}

t_request Request::getRequest() const { return this->_request; }

std::string Request::_trim(const std::string &str) {
  std::string::size_type first = str.find_first_not_of(' ');
  std::string::size_type last = str.find_last_not_of(' ');

  if (first == std::string::npos || last == std::string::npos) {
    return "";
  }
  return str.substr(first, last - first + 1);
}

void Request::_getMethod(const std::string &str) {
  if (!str.compare("GET")) {
    this->_request.method = GET;
  } else if (!str.compare("POST")) {
    this->_request.method = POST;
  } else if (!str.compare("DELETE")) {
    this->_request.method = DELETE;
  } else {
    this->_request.method = OTHER;
  }
}

void Request::_getHTTPVersion(const std::string &str) {
  std::string::size_type pos = str.find("HTTP/");
  if (pos != std::string::npos)
    this->_request.HTTPVersion = str.substr(pos + 5);
  else
    this->_request.HTTPVersion = "";
  OUTNL(this->_request.HTTPVersion + "aaa");
}

void Request::_parseHTTPRequest() {
  std::istringstream requestStream(this->_str);
  std::string line;
  std::string requestMethod;

  std::getline(requestStream, line);
  std::istringstream requestLineStream(line);
  requestLineStream >> requestMethod;
  this->_getMethod(requestMethod);
  requestLineStream >> this->_request.uri;

  while (std::getline(requestStream, line) && line != "\r") {
    std::string::size_type pos = line.find(':');
    if (pos != std::string::npos) {
      std::string key = this->_trim(line.substr(0, pos));
      std::string value = this->_trim(line.substr(pos + 1));
      this->_request.headers[key] = value;
    }
  }
}

void Request::printRequest() const {
  std::cout << "Method: " << this->_request.method << std::endl;
  std::cout << "URI: " << this->_request.uri << std::endl;
  std::cout << "Headers:" << std::endl;
  for (std::map<std::string, std::string>::const_iterator it =
           this->_request.headers.begin();
       it != this->_request.headers.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
}

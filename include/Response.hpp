/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:06:55 by bmoretti          #+#    #+#             */
/*   Updated: 2024/09/14 16:53:33 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "common.hpp"
#include "Request.hpp"
#include "Config.hpp"

namespace HttpStatus
{
	enum Code
	{
		ZERO,
		CONTINUE = 100,
		OK = 200,
		CREATED = 201,
		NO_CONTENT = 204,
		MOVED_PERMANENTLY = 301,
		BAD_REQUEST = 400,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		NOT_ALLOWED = 405,
		TIMEOUT = 408,
		CONFLICT = 409,
		PAYLOAD_TOO_LARGE = 413,
		SERVER_ERR = 500,
		NOT_IMPLEMENTED = 501,
		SERVICE_UNAVAILABLE = 503,
	};
};

typedef struct s_response
{
	std::string statusLine;
	bool		isCGI;
	std::map<std::string, std::string> headers;
	std::string body;
} t_response;

class Response
{
public:
	Response(Request &request, const ServerConfig &config);
	~Response();

	std::string getResponse() const;

private:
	Request &_request;
	const ServerConfig &_config;
	t_response _response;

	void	_generateFullURI();
	void _generateStatusLine();
	void _generateHeaders();
	void _generateBody(std::string &path);
	std::string _generateResponse() const;
	void _identifyCGI();
	void _executeCgi();
	std::string _location;

	bool _checkErrors();
	void _error405();
};

// utils
bool  endsWith(const std::string& str, const std::string & suffix);

#endif

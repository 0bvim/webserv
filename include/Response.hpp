/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-frei <vde-frei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:06:55 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/28 22:33:21 by vde-frei         ###   ########.fr       */
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
	std::map<std::string, std::string> headers;
	std::string body;
} t_response;

class Response
{
public:
	Response(Request &request, Config &config);
	~Response();

	std::string getResponse() const;

private:
	Request &_request;
	Config &_config;
	t_response _response;

	void _generateStatusLine();
	void _generateHeaders();
	void _generateBody();
	std::string _generateResponse() const;
	bool _identifyCGI();
};

#endif

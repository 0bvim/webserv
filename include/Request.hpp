/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:24:18 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/11 18:58:26 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "common.hpp"

enum RequestMethod {
	GET,
	POST,
	DELETE,
	OTHER
};

typedef struct s_request
{
	RequestMethod						method;
	std::string							uri;
	std::map<std::string, std::string>	headers;
} t_request;

class Request
{
public:
	Request(const char * str);
	~Request();

	t_request	getRequest() const;
	void printRequest() const;

private:
	const char *	_str;
	t_request		_request;

	std::string	_trim(const std::string & str);
	void		_getMethod(const std::string & str);
	void		_parseHTTPRequest();

};

#endif

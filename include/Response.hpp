/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:06:55 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/13 14:02:44 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "common.hpp"
# include "Request.hpp"

typedef struct s_response
{
	std::string							statusLine;
	std::map<std::string, std::string>	headers;
	std::string							body;
} t_response;

class Response
{
public:
	Response(Request & request);
	~Response();

	std::string	getResponse() const;

private:
	Request &	_request;
	t_response	_response;

	void		_generateStatusLine();
	void		_generateHeaders();
	void		_generateBody();
	std::string	_generateResponse() const;

};

#endif

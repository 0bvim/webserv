/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:24:18 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/11 18:58:26 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "common.hpp"

typedef struct s_request
{
	std::string							method;
	std::string							uri;
	std::map<std::string, std::string>	headers;
} t_request;

class Parser
{
public:
	Parser(const char * str);
	~Parser();

	t_request	getRequest() const;

private:
	const char *	_str;
	t_request		_request;

	std::string	_trim(const std::string & str);
	void		_parseHTTPRequest();

};

#endif

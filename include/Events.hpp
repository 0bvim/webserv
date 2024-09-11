/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nivicius <nivicius@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:05:36 by bmoretti          #+#    #+#             */
/*   Updated: 2024/09/10 21:28:09 by nivicius         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "common.hpp"
#include "Config.hpp"
#include "Server.hpp"

class Events
{
public:
	Events(const std::vector<ServerConfig> &servers);
	~Events();
	void run();

private:
	std::vector<Server *> _servers;
	int _epoll_fd;
	epoll_event *_event;
};

#endif

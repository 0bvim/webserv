/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:05:36 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/18 17:29:30 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_HPP
# define EVENTS_HPP

# include "common.hpp"
# include "Config.hpp"
# include "Server.hpp"

class Events
{
public:
	Events(const std::vector<ServerConfig> & servers);
    ~Events();
    void    run();

private:
	std::vector<Server *> _servers;
	int	_epoll_fd;
    epoll_event *_event;

	void	handleEvents();
};

#endif

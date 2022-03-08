/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabenjam <jabenjam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 12:34:54 by jabenjam          #+#    #+#             */
/*   Updated: 2022/03/08 16:43:12 by jabenjam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_irc.hpp"

/**
 * @brief polls all fds to check if one had any expected event
 * and manages new connections, i/o operations
 * 
 * @param data 
 * @return 1 if poll() malfunctionned or if poll() timed out
 */
int poll_setup(t_data &data)
{
	int poll_r = poll(reinterpret_cast<struct pollfd *>(&data.poll_fds[0]), data.poll_fds.size(), data.timeout);
	v_pollfds::iterator it = data.poll_fds.begin();
	v_pollfds::iterator end = data.poll_fds.end();

	if (poll_r < 0)
	{
		CERR(YELLOW, "poll()");
		return (1);
	}
	else if (poll_r == 0)
	{
		CERR(YELLOW, "poll() timeout.");
		return (1);
	}
	for (it = data.poll_fds.begin(); it != end; it++)
	{
		COUT(WHITE, "(" << &it << ")it->fd: " << it->fd << " | data.sock_fd: " << data.sock_fd << " | end(" << &end << ")");
		if (it->revents == 0)
			continue;
		else
		{
			if (it->fd == data.sock_fd)
			{
				COUT(WHITE, "listening socket is readable");
				while (1)
				{
					if (new_connection(data) == -1)
						break;
				}
				end = data.poll_fds.end();
			}
			else
			{
				COUT(WHITE, it->fd << " is readable");
				io_loop(data, it);
			}
		}
	}
	return (0);
}

void	handle_signals(int signal)
{
	if (signal == SIGQUIT)
	{
		COUT(WHITE, "\b\bprogram terminated by ctrl-\\");
		exit(1);
	}
	else if (signal == SIGINT)
	{
		COUT(WHITE, "\b\bprogram interrupted by ctrl-c");
		exit(1);
	}
}

void	signal_manager()
{
	signal(SIGQUIT, &handle_signals);
	signal(SIGINT, &handle_signals);
}

/**
 * @brief main server loop
 * 
 * @param data 
 * @return 1 if /quit command is received or poll() return is < 1
 */
int server_loop(t_data &data)
{
	while (1)
	{
		signal_manager();
		// if (command_loop(data) == 1)
		// 	return (0);
		if (poll_setup(data) == 1)
			return (1);
	}
	return (0);
}

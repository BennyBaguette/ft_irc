/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thoberth <thoberth@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 11:27:36 by thoberth          #+#    #+#             */
/*   Updated: 2022/05/21 11:36:40 by thoberth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/ft_irc.hpp"

void command_quit(t_data &data, Message &cmd)
{
	Users *sender = cmd.getSender();
	for (v_Chan::iterator it = data.chans.begin(), ite = data.chans.end(); it != ite; it++)
	{
		if (it->is_in_channel(sender))
		{
			it->rmusers(sender);
			COUT(YELLOW, sender->getNick_name() << " removed from " << it->getName());
		}
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktiong <ktiong@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 15:22:37 by ktiong            #+#    #+#             */
/*   Updated: 2021/06/06 18:12:50 by ktiong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h" 

char	ft_con_c(char c, int c1, int c2)
{
	if (c)
		return (c1);
	else
		return (c2);
}

static pid_t	mt_num(char *str)
{
	pid_t		i;

	i = 0;
	while (*str)
	{
		if (*str < '0' || '9' < *str)
			break ;
		i = i * 10 + (*str - '0');
		str++;
	}
	return (i);
}

/* uint8_t is a 8bits integer = 1 byte
** left bitshifting through 128 decimal, representing all
** unicode chars 
**
** right bitshifting back, basically count/2^n where n
** takes 1, 2, 3 and so on...
** 1 char is 1 byte
*/

static int	send_signal(pid_t pid, char sig)
{
	int	bit;
	int	j;
	int	i;

	bit = 0;
	i = 0;
	while (i++ < 3)
	{
		j = 0;
		g_nbr = 0;
		kill(pid, ft_con_c((sig & (1 << bit)), SIGUSR1, SIGUSR2));
		while (++j < 100)
		{
			if (g_nbr)
			{
				++bit;
				i = 0;
				break ;
			}
			usleep(1); //delay after each signal so that the server can correctly process the signals
		}
		if (bit == 8)
			return (1);
	}
	return (0);
}

/* handler takes the pid and the message from send_signal, and sends characters to that pid 
** signal confirmation received by the client */

static void	handler(int seg)
{
	(void)seg;
	g_nbr = 1;
}

int	main(int ac, char *av[])
{
	pid_t		client_pid;

	if (ac != 3)
	{
		write(1, "\033[0;33mInput: ./client [pid_id] [message]\n", 50);
		return (1);
	}
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler); // signal 2 should come
	client_pid = mt_num(av[1]);
	while (1)
	{
		if (!send_signal(client_pid, *av[2]))  // transform the string and send to the server
		{
			send_signal(client_pid, '\0');
			write(1, "\033[0;31mYour message has failed to deliver\n", 46);
			return (1);
		}
		if (!*(av[2]++))
			break ;
	}
	write(1, "\033[0;32mMessage has been successfully sent\n", 46);
	return (0);
}
 

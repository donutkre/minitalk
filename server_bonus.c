/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktiong <ktiong@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 12:21:44 by ktiong            #+#    #+#             */
/*   Updated: 2021/06/06 18:12:46 by ktiong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	mt_putstr(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(1, str, len);
}

static void	mt_putnbr_fd(int n)
{
	char	a;

	a = n % 10 + '0';
	if (n >= 10)
		mt_putnbr_fd(n / 10);
	write(1, &a, 1);
}

void	recept(int signum, siginfo_t *info, void *ptr)
{
	static int	ascii;
	static int	power;

	(void)ptr;
	if (kill(info->si_pid, signum) != 0)
		return ;
	if (!info->si_pid)
		return ;
	if (signum == SIGUSR1)
		ascii |= (1 << power);
	if (++power == 8)
	{
		power = 0;
		if (ascii)
			write(1, &ascii, 1);
		else
			mt_putstr("\n");
		ascii = 0;
	}
}

int	main(void)
{
	pid_t				serv_pid;
	struct sigaction	sa;
	sigset_t			hold;

	serv_pid = getpid();
	mt_putstr("\033[1;35mWelcome to Minitalk\nserver pid: ");
	mt_putnbr_fd((int)serv_pid);
	mt_putstr("\n");
	sa.sa_sigaction = &recept;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_mask = hold;
	sigaddset(&hold, SIGUSR1);
	sigaddset(&hold, SIGUSR2);
	if (sigaction(SIGUSR1, &sa, NULL) || sigaction(SIGUSR2, &sa, NULL))
	{
		mt_putstr("\033[0;31msigaction failed, please retry\n");
	}
	while (42)
		pause();
	return (0);
}
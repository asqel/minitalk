/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:20:02 by axlleres          #+#    #+#             */
/*   Updated: 2025/02/18 20:27:44 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int	g_msg_info[3] = {0, 0, 0};

void	sig_handler(int signo, siginfo_t *info, void *context)
{
	(void)context;
	if (signo == SIGUSR1)
	{
		g_msg_info[0] <<= 1;
		g_msg_info[0] |= 1;
	}
	else if (signo == SIGUSR2)
	{
		g_msg_info[0] <<= 1;
	}
	g_msg_info[1]++;
	g_msg_info[2] = info->si_pid;
}

void	ft_putnbr(int nb)
{
	if (nb == -2147483648)
		write(1, "-2147483648", 11);
	else if (nb < 0)
		ft_putnbr(-nb);
	else
	{
		if (nb > 9)
			ft_putnbr(nb / 10);
		write(1, &"0123456789"[nb % 10], 1);
	}
}

void	send_acknowledgement(void)
{
	if (g_msg_info[2] != 0)
		kill(g_msg_info[2], SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;
	int					pid;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (write(1, "Error\n", 6), 1);
	pid = getpid();
	write(1, "Server PID: ", 12);
	ft_putnbr(pid);
	write(1, "\n", 1);
	while (1)
	{
		if (g_msg_info[1] == 8)
		{
			if (g_msg_info[0] == 0)
				send_acknowledgement();
			write(1, &g_msg_info[0], 1);
			g_msg_info[0] = 0;
			g_msg_info[1] = 0;
		}
		pause();
	}
}

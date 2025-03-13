/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:58:14 by axlleres          #+#    #+#             */
/*   Updated: 2025/03/13 16:04:06 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minitalk.h"

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

void	ft_memcpy(char *dst, char *src, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

extern void	sig_handler(int signo, siginfo_t *info, void *context);

void	init(void)
{
	struct sigaction	sa;
	int					pid;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(1, "Error\n", 6);
		exit(1);
	}
	pid = getpid();
	write(1, "Server PID: ", 12);
	ft_putnbr(pid);
	write(1, "\n", 1);
}

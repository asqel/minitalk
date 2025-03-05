/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:20:02 by axlleres          #+#    #+#             */
/*   Updated: 2025/03/05 18:19:57 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minitalk.h"

t_msg_info	g_msg_info = {0};

void	reset_info(void)
{
	if (g_msg_info.msg)
		free(g_msg_info.msg);
	g_msg_info.pid = 0;
	g_msg_info.msg = NULL;
	g_msg_info.msg_len = 0;
	g_msg_info.alloc_len = 0;
	g_msg_info.current_char = 0;
	g_msg_info.current_bit = 0;
}

void	sig_handler(int signo, siginfo_t *info, void *context)
{
	(void)context;
	if (g_msg_info.pid != info->si_pid)
	{
		reset_info();
		g_msg_info.pid = info->si_pid;
		kill(g_msg_info.pid, SIGUSR2);
		return ;
	}
	if (g_msg_info.current_bit == 8)
		return ;
	if (signo == SIGUSR1)
		g_msg_info.current_char = (g_msg_info.current_char << 1) | 1;
	else if (signo == SIGUSR2)
		g_msg_info.current_char = g_msg_info.current_char << 1;
	g_msg_info.current_bit++;
	if (g_msg_info.current_bit < 8)
		kill(g_msg_info.pid, SIGUSR2);
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
	if (g_msg_info.pid != 0)
		kill(g_msg_info.pid, SIGUSR1);
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

void	append_char(void)
{
	char	*tmp;

	if (g_msg_info.msg_len == g_msg_info.alloc_len)
	{
		g_msg_info.alloc_len += 10;
		tmp = malloc(g_msg_info.alloc_len);
		if (!tmp)
		{
			reset_info();
			write(1, "Malloc Error\n", 6);
			exit(1);
		}
		ft_memcpy(tmp, g_msg_info.msg, g_msg_info.msg_len);
		free(g_msg_info.msg);
		g_msg_info.msg = tmp;
	}
	g_msg_info.msg[g_msg_info.msg_len] = g_msg_info.current_char;
	g_msg_info.msg_len++;
}

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

int	main(void)
{
	init();
	while (1)
	{
		if (g_msg_info.current_bit == 8)
		{
			append_char();
			if (g_msg_info.current_char == 0)
			{
				write(1, g_msg_info.msg, g_msg_info.msg_len);
				send_acknowledgement();
				reset_info();
			}
			g_msg_info.current_char = 0;
			g_msg_info.current_bit = 0;
			kill(g_msg_info.pid, SIGUSR2);
		}
		pause();
	}
}



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:20:02 by axlleres          #+#    #+#             */
/*   Updated: 2025/03/13 16:02:25 by axlleres         ###   ########.fr       */
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

extern void	ft_putnbr(int nb);
extern void	ft_memcpy(char *dst, char *src, int len);

void	send_acknowledgement(void)
{
	if (g_msg_info.pid != 0)
		kill(g_msg_info.pid, SIGUSR1);
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

extern void	init(void);

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:16:08 by axlleres          #+#    #+#             */
/*   Updated: 2025/03/13 16:23:19 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int	g_has_received = 0;

extern int	write_error(char *s);

int	ft_atoi(char *str)
{
	int	sign;
	int	res;

	sign = 1;
	res = 0;
	if (*str == '-')
		return (-1);
	while (*str >= '0' && *str <= '9' && *str != '\0')
	{
		res *= 10;
		res += *str - '0';
		str++;
	}
	return (res * sign);
}

void	send_char(char c, int pid)
{
	int	i;
	int	err;
	int	signal;
	int	try;

	i = 7;
	while (i >= 0)
	{
		if ((c >> i) & 1)
			signal = SIGUSR1;
		else
			signal = SIGUSR2;
		err = kill(pid, signal);
		if (err == -1)
		{
			write_error("Error sending signal to server (check the pid)\n");
			exit(1);
		}
		try = 0;
		while (g_has_received == 0 && try++ < 1000000)
			usleep(1);
		g_has_received = 0;
		i--;
	}
}

void	sig_handler(int signo)
{
	if (signo == SIGUSR1)
	{
		write(1, "Server received our message\n", 28);
		exit(0);
	}
	else if (g_has_received == 0)
		g_has_received = 1;
}

void	init(int pid)
{
	int	try;

	if (kill(pid, SIGUSR1) == -1)
	{
		write_error("Error sending signal to server (check the pid)\n");
		exit(1);
	}
	try = 0;
	while (g_has_received == 0 && try++ < 1000000)
		usleep(1);
	g_has_received = 0;
}

int	main(int argc, char **argv)
{
	int		pid;
	int		i;
	char	*message;

	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);
	if (argc != 3)
		return (write_error("Wrong arguments\nUsage: <PID> <message>\n"));
	pid = ft_atoi(argv[1]);
	init(pid);
	if (pid == -1)
		return (write_error("Wrong PID\n"));
	message = argv[2];
	i = 0;
	while (message[i])
	{
		send_char(message[i], pid);
		i++;
	}
	if (i <= 0 || message[i - 1] != '\n')
		send_char('\n', pid);
	send_char('\0', pid);
	pause();
	return (0);
}

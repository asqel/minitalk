/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:16:08 by axlleres          #+#    #+#             */
/*   Updated: 2025/02/18 20:28:35 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int	write_error(char *error)
{
	int	len;

	len = 0;
	while (error[len])
		len++;
	write(2, error, len);
	return (1);
}

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

	i = 7;
	while (i >= 0)
	{
		if ((c >> i) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		i--;
		usleep(500);
	}
}

void	sig_handler(int signo)
{
	(void)signo;
	write(1, "Server received our message\n", 28);
	exit(0);
}

int	main(int argc, char **argv)
{
	int		pid;
	int		i;
	char	*message;

	signal(SIGUSR1, sig_handler);
	if (argc != 3)
		return (write_error("Wrong arguments\nUsage: <PID> <message>\n"));
	pid = ft_atoi(argv[1]);
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

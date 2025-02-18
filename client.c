#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

void write_error(char *error) {
	int len;

	len = 0;
	while (error[len])
		len++;
	write(2, error, len);
}

int ft_atoi(char *str) {
	int sign;
	int res;

	sign = 1;
	res = 0;
	if (*str == '-')
		return -1;
	while (*str >= '0' && *str <= '9' && *str != '\0') {
		res *= 10;
		res += *str - '0';
		str++;
	}
	return (res * sign);
}

void send_char(char c, int pid) {
	int i;

	i = 7;
	while (i >= 0) {
		if ((c >> i) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		i--;
		usleep(10000);
	}
}

int main(int argc, char **argv) {
	if (argc != 3) {
		write_error("Wrong number of arguments\nUsage: <PID> <message>\n");
		return (1);
	}
	int pid = ft_atoi(argv[1]);
	char *message = argv[2];

	int i = 0;
	while (message[i]) {
		send_char(message[i], pid);
		i++;
	}
	return (0);

}

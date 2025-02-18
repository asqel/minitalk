#include <signal.h>
#include <unistd.h>
#include <stdio.h>

char msg_info[2] = {0, 0};

void sig_handler(int signo) {
	if (signo == SIGUSR1)
	{
		msg_info[0] <<= 1;
		msg_info[0] |= 1;
	}
	else if (signo == SIGUSR2)
	{
		msg_info[0] <<= 1;
	}
	msg_info[1]++;
}

void ft_putnbr(int nb) {
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

int main() {
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);

	int pid = getpid();
	write(1, "Server PID: ", 12);
	ft_putnbr(pid);
	write(1, "\n", 1);
	while (1)
	{
		if (msg_info[1] == 8)
		{
			write(1, &msg_info[0], 2);
			msg_info[0] = 0;
			msg_info[1] = 0;
		}
	}
}
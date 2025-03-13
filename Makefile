NAME = minitalk

SRCS_SRV = server.c server2.c
SRCS_CLT = client.c client2.c

OBJS_SRV = $(SRCS_SRV:.c=.o)
OBJS_CLT = $(SRCS_CLT:.c=.o)

SRV = server
CLT = client

CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
CC = clang

all: $(NAME)

$(NAME) : $(SRV) $(CLT)

$(SRV) : $(OBJS_SRV)
	$(CC) $(LDFLAGS) -o $(SRV) $(OBJS_SRV)

$(CLT) : $(OBJS_CLT)
	$(CC) $(LDFLAGS) -o $(CLT) $(OBJS_CLT)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_SRV) $(OBJS_CLT)

fclean: clean
	rm -f $(SRV) $(CLT)

re: fclean all

.PHONY: clean fclean re all


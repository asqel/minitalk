/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:57:40 by axlleres          #+#    #+#             */
/*   Updated: 2025/03/13 16:04:23 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	write_error(char *error)
{
	int	len;

	len = 0;
	while (error[len])
		len++;
	write(2, error, len);
	return (1);
}

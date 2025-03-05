/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlleres <axlleres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:49:37 by axlleres          #+#    #+#             */
/*   Updated: 2025/03/05 17:50:51 by axlleres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

typedef struct s_msg_info
{
	int		pid;
	char	*msg;
	int		msg_len;
	int		alloc_len;
	char	current_char;
	int		current_bit;
}	t_msg_info;

#endif
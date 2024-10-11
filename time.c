/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 23:57:09 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/11 01:48:55 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	time_update(void);
int		event_quit(void);

void	time_update(void)
{
	static struct timeval	now = {0};
	static struct timeval	before = {0};
	t_ime	*time;

	time = &g()->time;
	if (gettimeofday(&now, NULL) < 0)
	{
		perror("time_now - gettimeofday");
		event_quit();
	}
	time->dt = (now.tv_sec - before.tv_sec) + (now.tv_usec - before.tv_usec)
		/ 1000000.0;
	if (time->dt > 0)
		time->fps = 1.0 / time->dt;
	else
		time->fps = 0;
	before = now;
}

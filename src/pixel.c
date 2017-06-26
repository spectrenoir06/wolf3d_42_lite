/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spectre <spectre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/15 21:45:47 by adoussau          #+#    #+#             */
/*   Updated: 2016/03/13 20:37:16 by spectre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

inline void	game_draw_pixel(t_game *game, int x, int y, uint32_t *c)
{
	game->sdl.text_buf[x + (y * WIN_LX)] = *c;
}

inline void	game_draw_pixel_black(t_game *game, int x, int y, uint32_t *c)
{
	int color = *c;

	unsigned char *tmp = (unsigned char*)&color;

	tmp[0] = tmp[0] >> 1;
	tmp[1] = tmp[1] >> 1;
	tmp[2] = tmp[2] >> 1;

	game->sdl.text_buf[x + (y * WIN_LX)] = color;
}

void		bmp_draw(t_game *game, SDL_Surface *img, int startx, int starty)
{
	int			x;
	int			y;
	uint32_t	*color;

	x = 0;
	while (x < img->w && x + startx < WIN_LX)
	{
		y = 0;
		while (y < img->h && y + starty < WIN_LY)
		{
			t_color pixel = ((t_color*)img->pixels)[x + (y * img->w)];
			if (pixel.r != 255 || pixel.g != 0 || pixel.b != 255)
				game_draw_pixel(game, x + startx, y + starty, &pixel);
			y++;
		}
		x++;
	}
}

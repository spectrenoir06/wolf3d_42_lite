/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spectre <spectre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/15 21:45:47 by adoussau          #+#    #+#             */
/*   Updated: 2016/03/15 20:02:30 by spectre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include <math.h>

void		hud_background(t_game *game, int mode)
{
	SDL_Surface		*hud[4];
	int				i;
	char			*strmode;
	char			path[256];
	SDL_Surface *tmp;

	strmode = ft_itoa(mode);
	ft_kebab(path, "modes/", strmode, "/hud/top.png", NULL);
	tmp = IMG_Load(path);
	hud[0] = SDL_ConvertSurface(tmp, game->map.bmpFormat->format, 0);
	SDL_FreeSurface(tmp);


	ft_kebab(path, "modes/", strmode, "/hud/left.png", NULL);
	tmp = IMG_Load(path);
	hud[1] = SDL_ConvertSurface(tmp, game->map.bmpFormat->format, 0);
	SDL_FreeSurface(tmp);


	ft_kebab(path, "modes/", strmode, "/hud/right.png", NULL);
	tmp = IMG_Load(path);
	hud[2] = SDL_ConvertSurface(tmp, game->map.bmpFormat->format, 0);
	SDL_FreeSurface(tmp);


	ft_kebab(path, "modes/", strmode, "/hud/bottom.png", NULL);
	tmp = IMG_Load(path);
	hud[3] = SDL_ConvertSurface(tmp, game->map.bmpFormat->format, 0);
	SDL_FreeSurface(tmp);

	free(strmode);
	bmp_draw(game, hud[0], 0, 0);
	bmp_draw(game, hud[1], 0, GAME_Y1);
	bmp_draw(game, hud[2], GAME_X2, GAME_Y1);
	bmp_draw(game, hud[3], 0, GAME_Y2);
	i = 0;
	while (i < 4)
		SDL_FreeSurface(hud[i++]);
}

void		hud_render(t_game *game)
{
	hud_map(game);
	weapon_animate(game, &game->player);
	weapon_draw(game);
}

int			bgr_average(int c1, int c2)
{
	int		r;
	int		g;
	int		b;

	r = ((c1 >> 16) & 0xFF) + ((c2 >> 16) & 0xFF);
	r = r >> 1;
	g = ((c1 >> 8) & 0xFF) + ((c2 >> 8) & 0xFF);
	g = g >> 1;
	b = (c1 & 0xFF) + (c2 & 0xFF);
	b = b >> 1;
	return ((b << 16) + (g << 8) + r);
}

void		hud_map(t_game *game)
{
	int		x;
	int		y;
	Uint32	color;
	Uint32	bcolor;

	x = 0;
	while (x < game->map.lx)
	{
		y = 0;
		while (y < game->map.ly)
		{
			color = (game->map.wall[x + (y * game->map.lx)] ? WALL : FLOOR);
			bcolor = game->sdl.text_buf[(GAME_X1 + (x * 4)) + ((GAME_Y1 + (y * 4)) * WIN_LX)];
			color = bgr_average(color, bcolor);
			game_draw_rect(game, GAME_X1 + x * 4, GAME_Y1 + y * 4, color);
			y++;
		}
		x++;
	}
	game_draw_rect(game, GAME_X1 + game->player.pos.x * 4, GAME_Y1 + game->player.pos.y * 4, PLAYER);
	game_draw_rect(game, GAME_X1 + game->player.pos.x * 4 + (game->player.dir.x * 4), GAME_Y1 + game->player.pos.y * 4 + (game->player.dir.y * 4), FACE);
}

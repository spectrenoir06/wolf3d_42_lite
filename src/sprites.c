/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spectre <spectre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/04 17:34:27 by adoussau          #+#    #+#             */
/*   Updated: 2016/03/13 20:41:05 by spectre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			sprite_angle(t_drsprite *sprt, t_game *game)
{
	sprt->stripe = sprt->drawstartx - 1;
	sprt->pos.x = game->player.pos.x - game->map.entity_ptr[sprt->x]->pos.x;
	sprt->pos.y = game->player.pos.y - game->map.entity_ptr[sprt->x]->pos.y;
	if (game->map.sprite[game->map.entity_ptr[sprt->x]->texture].frames == 8)
	{
		sprt->angle = atan2(sprt->pos.y, sprt->pos.x) -
		atan2(game->map.entity_ptr[sprt->x]->dir.y,
		game->map.entity_ptr[sprt->x]->dir.x) + M_PI_4 / 2;
		if (sprt->angle < 0)
			sprt->angle += 2 * M_PI;
		sprt->i = ((sprt->angle) / (M_PI * 2.0) * 8.0);
		sprt->i = sprt->i > 7 ? 7 : sprt->i;
		sprt->i = sprt->i < 0 ? 0 : sprt->i;
	}
	else
		sprt->i = 0;
}

void			game_draw_sprites_init(t_drsprite *sprt, t_game *game)
{
	sprt->spritex = game->map.entity_ptr[sprt->x]->pos.x - game->player.pos.x;
	sprt->spritey = game->map.entity_ptr[sprt->x]->pos.y - game->player.pos.y;
	sprt->invdet = 1.0 / (game->player.plane.x * game->player.dir.y -
	game->player.dir.x * game->player.plane.y);
	sprt->transformx = sprt->invdet * (game->player.dir.y * sprt->spritex -
	game->player.dir.x * sprt->spritey);
	sprt->transformy = sprt->invdet * (-game->player.plane.y * sprt->spritex +
	game->player.plane.x * sprt->spritey);
	sprt->spritescreenx = (int)(((GAME_LX) / 2.0) *
	(1 + sprt->transformx / sprt->transformy));
	sprt->spriteheight = abs((int)((GAME_LY) / sprt->transformy));
	sprt->drawstarty = -sprt->spriteheight / 2.0 + (GAME_LY) / 2.0;
	if (sprt->drawstarty < 0)
		sprt->drawstarty = 0;
	sprt->drawendy = sprt->spriteheight / 2.0 + (GAME_LY) / 2.0;
	if (sprt->drawendy >= (GAME_LY))
		sprt->drawendy = (GAME_LY) - 1;
	sprt->spritewidth = abs((int)((GAME_LY) / sprt->transformy));
	sprt->drawstartx = -sprt->spritewidth / 2.0 + sprt->spritescreenx;
	if (sprt->drawstartx < 0)
		sprt->drawstartx = 0;
	sprt->drawendx = sprt->spritewidth / 2.0 + sprt->spritescreenx;
	if (sprt->drawendx >= (GAME_LX))
		sprt->drawendx = (GAME_LX) - 1;
	sprite_angle(sprt, game);
}

void			game_draw_sprites_draw(t_drsprite *sprt, t_game *game)
{
	t_color color;

	sprt->d = sprt->y - (GAME_LY) / 2.0 + sprt->spriteheight / 2.0;
	sprt->texy = ((sprt->d * 512) / sprt->spriteheight);

	color = ((t_color*)(game->map.sprite[game->map.entity_ptr[sprt->x]->texture].tex->pixels))[(int)sprt->texx + (sprt->texy * game->map.sprite[game->map.entity_ptr[sprt->x]->texture].tex->w) + (sprt->i * TEX_SIZE)];


	if (color.r != 255 || color	.g != 0 || color.b != 255)
		game_draw_pixel(game, GAME_X1 + GAME_LX - sprt->stripe, GAME_Y1 + sprt->y, &color);
}

void			game_sort_sprites(t_drsprite *sprt, t_game *game)
{
	sprt->x = -1;
	while (++sprt->x < game->map.nb_entity)
		game->map.entity[sprt->x].dist = ((game->player.pos.x - game->map.entity
		[sprt->x].pos.x) * (game->player.pos.x - game->map.entity[sprt->x].
		pos.x) + (game->player.pos.y - game->map.entity[sprt->x].pos.y) *
		(game->player.pos.y - game->map.entity[sprt->x].pos.y));
	ft_sort_qck((void **)game->map.entity_ptr,
	game->map.nb_entity, sprite_compare);
}

void			game_draw_sprites(t_game *game)
{
	t_drsprite	sprt;

	game_sort_sprites(&sprt, game);
	sprt.x = -1;
	while (++sprt.x < game->map.nb_entity)
	{
		game_draw_sprites_init(&sprt, game);
		while (++sprt.stripe <= sprt.drawendx)
		{
			sprt.texx = (int)(256 * (sprt.stripe - (-sprt.spritewidth / 2 +
			sprt.spritescreenx)) * 512 / sprt.spritewidth / 256);
			if (sprt.transformy > 0 && sprt.stripe > 0 && sprt.stripe <
				(GAME_LX) && sprt.transformy < game->zbuffer[sprt.stripe])
			{
				sprt.y = sprt.drawstarty - 1;
				while (++sprt.y < sprt.drawendy)
					game_draw_sprites_draw(&sprt, game);
			}
		}
	}
}

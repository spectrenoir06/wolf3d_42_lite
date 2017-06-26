/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caster.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/05 17:53:48 by adoussau          #+#    #+#             */
/*   Updated: 2015/02/05 17:53:56 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	choose_step(t_ray *ray, t_wall *wall)
{
	wall->map.x = (int)ray->pos.x;
	wall->map.y = (int)ray->pos.y;
	if (ray->dir.x < 0)
	{
		wall->step.x = -1;
		ray->side.x = (ray->pos.x - wall->map.x) * ray->delta.x;
	}
	else
	{
		wall->step.x = 1;
		ray->side.x = (wall->map.x + 1.0 - ray->pos.x) * ray->delta.x;
	}
	if (ray->dir.y < 0)
	{
		wall->step.y = -1;
		ray->side.y = (ray->pos.y - wall->map.y) * ray->delta.y;
	}
	else
	{
		wall->step.y = 1;
		ray->side.y = (wall->map.y + 1.0 - ray->pos.y) * ray->delta.y;
	}
}

void		ray_caster(t_game *game, t_ray *ray, t_wall *wall)
{
	choose_step(ray, wall);
	while (42)
	{
		if (ray->side.x < ray->side.y)
			ray->side.x += ray->delta.x,
			wall->map.x += wall->step.x,
			wall->side = 0;
		else
			ray->side.y += ray->delta.y,
			wall->map.y += wall->step.y,
			wall->side = 1;
		if (game->map.wall[wall->map.x + (wall->map.y * game->map.lx)] > 0)
			break ;
	}
	wall->dist = (wall->side ? fabs((wall->map.y - ray->pos.y + (1 -
			wall->step.y) / 2) / ray->dir.y) : fabs((wall->map.x - ray->pos.x +
			(1 - wall->step.x) / 2) / ray->dir.x));
	wall->id = game->map.wall[wall->map.x + (wall->map.y * game->map.lx)];
	if (game->map.has_notext)
	{
		if (wall->side == 0)
			wall->side = (wall->step.x > 0 ? 0 : 2);
		else
			wall->side = (wall->step.y > 0 ? 1 : 3);
	}
}

void		draw_fc_pixel(t_game *game, t_fc *cf, t_rend *rend)
{
	cf->w = (game->calcule[cf->y - (GAME_LY / 2)]) / (rend->wall.dist);

	cf->c_fl.x = cf->w * cf->fl.x + (1.0 - cf->w) * game->player.pos.x;
	cf->c_fl.y = cf->w * cf->fl.y + (1.0 - cf->w) * game->player.pos.y;

	cf->fl_t.x = (uint32_t)(cf->c_fl.x * TEX_SIZE) % TEX_SIZE;
	cf->fl_t.y = (uint32_t)(cf->c_fl.y * TEX_SIZE) % TEX_SIZE;

	cf->colf = (uint32_t*)&((t_color*)(game->map.textures[(uint32_t)game->map.ceil[((uint32_t)
		cf->c_fl.x) + ((uint32_t)(cf->c_fl.y) * game->map.lx)]
		]->pixels))[(uint32_t)cf->fl_t.x + ((uint32_t)cf->fl_t.y *
		TEX_SIZE)];
	cf->colc = (uint32_t*)&((t_color*)(game->map.textures[(uint32_t)game->map.floor[
		((uint32_t)cf->c_fl.x) + ((uint32_t)(cf->c_fl.y) * game->map.lx)
		]]->pixels))[(uint32_t)cf->fl_t.x + ((uint32_t)cf->fl_t.y *
		TEX_SIZE)];
	if ((*(cf->colc) & 0x00FFFFFF) == 0xFF00FF)
	{
	//printf("%x %f\n",*(cf->colc), cf->agl);
		game_draw_pixel(game,
						cf->x + GAME_X1,
						GAME_LY + GAME_Y1 - cf->y,
						(uint32_t*)&((t_color*)(game->map.sky->pixels))[(cf->sky) + (((GAME_LY) - cf->y) * (game->map.sky->w))]
						);
	}
	else
	{
		game_draw_pixel(game,
						cf->x + GAME_X1,
						GAME_LY + GAME_Y1 - cf->y,
						cf->colc
						);
	}
		game_draw_pixel(game,
						cf->x + GAME_X1,
						cf->y + GAME_Y1,
						cf->colf
						);
}

void		fc_choose_step(t_rend *rend, t_fc *cf)
{
	if (rend->wall.side == 0 && rend->ray.dir.x > 0)
	{
		cf->fl.x = rend->wall.map.x;
		cf->fl.y = rend->wall.map.y + rend->wallx;
	}
	else if (rend->wall.side == 0 && rend->ray.dir.x < 0)
	{
		cf->fl.x = rend->wall.map.x + 1.0;
		cf->fl.y = rend->wall.map.y + rend->wallx;
	}
	else if (rend->wall.side == 1 && rend->ray.dir.y > 0)
	{
		cf->fl.x = rend->wall.map.x + rend->wallx;
		cf->fl.y = rend->wall.map.y;
	}
	else if (rend->wall.side == 1 && rend->ray.dir.y < 0)
	{
		cf->fl.x = rend->wall.map.x + rend->wallx;
		cf->fl.y = rend->wall.map.y + 1.0;
	}
}

void		draw_floor_and_ceil(t_game *game, t_rend *rend)
{
	t_fc		cf;
	uint32_t	color1 = 0x646464;
	uint32_t	color2 = 0x323232;

	cf.x = GAME_LX - rend->x;
	cf.y = rend->y;
	fc_choose_step(rend, &cf);

	cf.sky = cf.x + (game->player.agl) / (M_PI) * (float)(game->map.sky->w);
	cf.sky %= game->map.sky->w;

	if (game->map.has_fc)
	{
		while (cf.y <= GAME_LY)
		{
			draw_fc_pixel(game, &cf, rend);
			cf.y++;
		}
	}
	else
	{
		while (cf.y <= GAME_LY)
		{	game_draw_pixel(game, cf.x + GAME_X1, cf.y + GAME_Y1, &color1);
			game_draw_pixel(game, cf.x + GAME_X1, GAME_LY + GAME_Y1 - cf.y, &color2);
			cf.y++;
		}
	}
}

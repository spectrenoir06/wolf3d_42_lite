/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/04 17:31:50 by adoussau          #+#    #+#             */
/*   Updated: 2017/06/26 03:05:46 by spectre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	sdl_exit(t_game *game)
{
	weapon_unload(&game->map);
	map_unload(&game->map);
	free(game->sdl.text_buf);
	if (SDL_NumJoysticks() == 1)
	{
		SDL_HapticClose(game->haptic);
		SDL_JoystickClose(game->joystick);
	}
	SDL_DestroyTexture(game->sdl.tex);
	SDL_DestroyRenderer(game->sdl.rd);
	SDL_DestroyWindow(game->sdl.win);
	#ifndef __EMSCRIPTEN__
		sdl_mixer_quit(&game->sounds);
	#endif
	SDL_Quit();
	exit(0);
}

void	kb_key_down(SDL_Event ev, t_game *game)
{
	if (ev.key.keysym.sym == SDLK_LSHIFT || ev.key.keysym.sym == SDLK_RSHIFT)
		game->player.speed += 2;
	else if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
		game->input[MOV_Y] = SINT16_MAX;
	else if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
		game->input[MOV_Y] = SINT16_MIN;
	else if (ev.key.keysym.sym == SDLK_a)
		game->input[MOV_X] = SINT16_MAX;
	else if (ev.key.keysym.sym == SDLK_d)
		game->input[MOV_X] = SINT16_MIN;
	else if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_q)
		game->input[ROT_Z] = SINT16_MIN;
	else if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_e)
		game->input[ROT_Z] = SINT16_MAX;
	else if (ev.key.keysym.sym == SDLK_SPACE && game->player.w_anim == 0)
	{
		weapon_start_anim(game, &game->player);
		#ifndef __EMSCRIPTEN__
			Mix_PlayChannel(-1, game->sounds.son2, 0);
		#endif
	}
	else if (ev.key.keysym.sym == SDLK_ESCAPE)
		sdl_exit(game);
}

void	kb_key_up(SDL_Event ev, t_game *game)
{
	if (ev.key.keysym.sym == SDLK_LSHIFT || ev.key.keysym.sym == SDLK_RSHIFT)
		game->player.speed -= 2;
	else if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
		game->input[MOV_Y] = 0;
	else if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
		game->input[MOV_Y] = 0;
	else if (ev.key.keysym.sym == SDLK_a)
		game->input[MOV_X] = 0;
	else if (ev.key.keysym.sym == SDLK_d)
		game->input[MOV_X] = 0;
	else if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_q)
		game->input[ROT_Z] = 0;
	else if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_e)
		game->input[ROT_Z] = 0;
	#ifndef __EMSCRIPTEN__
		Mix_FadeOutChannel(1, 200);
	#endif
}

int		game_event_handler(t_game *game)
{
	if (!SDL_PollEvent(&game->ev))
		return (0);
	if (game->ev.type == SDL_MOUSEMOTION && (game->ev.motion.xrel
		> SINT16_MIN && game->ev.motion.xrel < SINT16_MAX))
		game->input[ROT_Z_M] = game->ev.motion.xrel;
	else if (game->ev.type == SDL_MOUSEBUTTONDOWN
	&& (game->ev.button.button == SDL_BUTTON_LEFT && game->player.w_anim == 0))
	{
		weapon_start_anim(game, &game->player);
		#ifndef __EMSCRIPTEN__
			Mix_PlayChannel(-1, game->sounds.son2, 0);
		#endif
	}
	else if (game->ev.type == SDL_KEYDOWN)
		kb_key_down(game->ev, game);
	else if (game->ev.type == SDL_KEYUP)
		kb_key_up(game->ev, game);
	else if (game->ev.type == SDL_JOYBUTTONDOWN)
		joy_but_down(game->ev, game);
	else if (game->ev.type == SDL_JOYBUTTONUP)
		joy_but_up(game->ev, game);
	else if (game->ev.type == SDL_JOYAXISMOTION)
		joy_axis(game->ev, game);
	else if (game->ev.type == SDL_QUIT)
		sdl_exit(game);
	return (1);
}

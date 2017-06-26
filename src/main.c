/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spectre <spectre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/13 16:55:44 by adoussau          #+#    #+#             */
/*   Updated: 2016/03/15 19:54:32 by spectre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#ifdef EMSCRIPTEN
	#include <emscripten.h>
#endif


struct timeval tv1;
struct timeval tv2;

void	setdt(t_game *game)
{
	tv2 = tv1;
	gettimeofday(&tv1, NULL);
	game->dt = ((tv1.tv_sec - tv2.tv_sec) +
	((tv1.tv_usec - tv2.tv_usec) / 1000000.0));
}

void	check_args(t_game *game, int ac, char *av[])
{
	if (ac == 2)
	{
		if (!ft_strcmp(av[1], "1"))
			map_init(game, 1, 1);
		else if (!ft_strcmp(av[1], "2"))
			map_init(game, 1, 2);
		else if (!ft_strcmp(av[1], "3"))
			map_init(game, 1, 3);
		else if (!ft_strcmp(av[1], "4"))
			map_init(game, 1, 4), game->map.has_notext = 1;
		else
			map_init(game, 1, 2);
	}
	else
		map_init(game, 1, 2);
}

t_game	game;

SDL_Surface *image1;
SDL_Surface *image2;
SDL_Surface *image3;

void update(void);

int		main(int ac, char **av)
{
	// char	fps[30] = "Wolf3d : FPS = ";
	int		i;

	// float	test = 0;
	// int		test2 = 0;
	game.dt = 0;
	gettimeofday(&tv1, NULL);
	printf("hello 2\n");
	game_init_sdl(&game);
	printf("hello 3\n");
	check_args(&game, ac, av);
	printf("hello 4\n");
	i = ((GAME_LY) / 2) - 1;
	while (i++ < (GAME_LY) - 1)
		game.calcule[(i) - (GAME_LY / 2)] = (GAME_LY) / (2.0 * (i) - (GAME_LY));

	//multi_init(&game);


	i = 0;

	#ifdef __EMSCRIPTEN__
		// void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
		emscripten_set_main_loop(update, 60, 1);
	#else
		while (42)
			update();
	#endif
	return (0);
}


void update(void)
{
	setdt(&game);
	while (game_event_handler(&game))
	;
	player_update(&(game.player), &game);
	game_render(&game);
	hud_render(&game);

	game_draw_all(&game);

	/*
	UDPpacket packet;
	int numrecv;

	numrecv = SDLNet_UDP_Recv(game.multi.udp_socket, &packet);
	if(numrecv) {
	printf("receive udp\n");
}

//int result = SDLNet_TCP_Recv(game.multi.socket, game.multi.buffer, 17);
//	if(result > 0)
//	{
//		printf("x -> %f, y -> %f\n",*((double*)game.multi.buffer),*((double*)(game.multi.buffer + 8)));
//	}
//	else
//		printf("error\n");
//
//	multi_send_pos(&game);
test = 0;
}*/
}

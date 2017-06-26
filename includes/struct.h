/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spectre <spectre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/15 21:45:47 by adoussau          #+#    #+#             */
/*   Updated: 2016/03/15 19:35:42 by spectre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# define SDL_R SDL_Renderer
# define SDL_J SDL_Joystick
# define NB_SPRITE_TEX 174
# define NB_WEAPON_TEX 1
# define NB_WEAPON_TEX_FRAME 5
# define NB_ENTITY_MAX 1000

typedef enum	e_key
{
	MOV_X = 0,
	MOV_Y = 1,
	MOV_Z = 2,
	ROT_X = 3,
	ROT_Y = 4,
	ROT_Z = 5,
	ROT_Z_M = 6,
	USE = 7
}				t_key;

typedef struct	s_vect2di
{
	int			x;
	int			y;
}				t_vect2di;

typedef struct	s_vect2dd
{
	float		x;
	float		y;
}				t_vect2dd;

typedef struct	s_vect3di
{
	int			x;
	int			y;
	int			z;
}				t_vect3di;

typedef struct	s_vect3dd
{
	float		x;
	float		y;
	float		z;
}				t_vect3dd;

typedef struct	s_rectdd
{
	float		x;
	float		y;
	float		w;
	float		h;
}				t_rectdd;

typedef struct	s_color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}				t_color;

typedef struct	s_entity
{
	t_vect2dd	pos;
	t_vect2dd	dir;
	int			texture;
	int			tex_anim;
	int			tex_focus;
	float		dist;
	int			type;
}				t_entity;

typedef struct	s_drsprite
{
	int			y;
	int			x;
	float		spritex;
	float		spritey;
	float		invdet;
	float		transformx;
	float		transformy;
	int			spritescreenx;
	int			spriteheight;
	int			drawstarty;
	int			drawendy;
	int			spritewidth;
	int			drawstartx;
	int			drawendx;
	int			stripe;
	int			i;
	int			texx;
	int			d;
	int			texy;
	float		angle;
	int			*color;
	t_vect2dd	pos;
}				t_drsprite;

typedef struct	s_sprite
{
	SDL_Surface	*tex;
	int			frames;
}				t_sprite;

typedef struct	s_player
{
	t_vect2dd	pos;
	t_vect2dd	dir;
	t_vect2dd	plane;
	t_vect2dd	box;
	int			speed;
	char		weapon;
	float		w_anim;
	float		agl;
}				t_player;

typedef struct	s_sounds
{
	Mix_Chunk	*pas;
	Mix_Chunk	*son1;
	Mix_Chunk	*son2;
	Mix_Music	*music;
}				t_sounds;

typedef struct	s_sdl
{
	SDL_Window	*win;
	SDL_Texture	*tex;
	SDL_R		*rd;
	Uint32		*text_buf;
	Uint32		*hud_buf;

}				t_sdl;

typedef struct	s_map
{
	int			lx;
	int			ly;
	int			nb_entity;
	int			nb_sprite;
	int			nb_texture;
	int			has_fc;
	int			colors[4];
	int			has_notext;
	int			start_x;
	int			start_y;
	Uint8		*floor;
	Uint8		*wall;
	Uint8		*ceil;
	SDL_Surface	**textures;
	SDL_Surface	*sky;
	t_entity	entity[NB_ENTITY_MAX];
	t_entity	*entity_ptr[NB_ENTITY_MAX];
	t_sprite	sprite[NB_SPRITE_TEX];
	SDL_Surface	*weapon_tex[NB_WEAPON_TEX + 1][NB_WEAPON_TEX_FRAME + 1];
	SDL_Surface	*bmpFormat;
}				t_map;


// typedef struct 	s_multi
// {
// 	int			connect;
// 	TCPsocket	tcp_socket;
// 	IPaddress	ipTcp;
// 	IPaddress	ipUdp;
// 	UDPsocket	udp_socket;
// 	UDPpacket	*packet;
// 	Uint8		buffer[512];
// }				t_multi;

typedef struct	s_game
{
	t_sdl		sdl;
	t_player	player;
	t_map		map;
	t_sounds	sounds;
//	t_multi		multi;
	t_color		texture[TEX_SIZE][TEX_SIZE];
	float		dt;
	float		zbuffer[GAME_LX];
	float		calcule[GAME_LY / 2];
	SDL_J		*joystick;
	SDL_Haptic	*haptic;
	SDL_Event	ev;

	Sint16		input[12];
}				t_game;

typedef struct	s_ray
{
	t_vect2dd	pos;
	t_vect2dd	dir;
	t_vect2dd	delta;
	t_vect2dd	side;
}				t_ray;

typedef struct	s_wall
{
	t_vect2di	map;
	float		dist;
	int			side;
	int			id;
	t_vect2di	step;
}				t_wall;

typedef struct	s_rend
{
	int			x;
	int			y;
	float		camera_x;
	int			linehgt;
	int			drawstart;
	int			drawend;
	float		wallx;
	int			tex_x;
	int			tex_y;
	t_color		*color;
	t_ray		ray;
	t_wall		wall;
}				t_rend;

typedef struct	s_fc
{
	t_vect2dd	fl;
	float		w;
	t_vect2dd	c_fl;
	t_vect2di	fl_t;
	uint32_t	*colc;
	uint32_t	*colf;
	int			x;
	int			y;
	int			sky;
}				t_fc;

typedef struct	s_mapload
{
	int			i;
	int			fd;
	char		buff[256];
	char		*nb;
	float		tmp_x;
	float		tmp_y;
}				t_mapload;
;

#endif

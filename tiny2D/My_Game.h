#pragma once

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{
	int m(int x, int y, int ncols)
	{
		return y * ncols + x;
	}

	void random_Walk_Step(Grid::Point *p, Grid::Grid *g)
	{
		switch (rand() % 4)
		{
		case 0:
			if(p->x < g->n_cols) p->x++;
			break;
		case 1:
			if(p->x > 0) p->x--;
			break;
		case 2:
			if(p->y < g->n_rows) p->y++;
			break;
		case 3:
			if(p->y > 0) p->y--;
			break;
		}


		g->data[m(p->x, p->y, g->n_cols)]++;

	}

	namespace World
	{
		//manages a tileset image
		Tileset::Tileset tileset;

		//manages font
		Font::Font text;
		
		Grid::Point current_position;
		Grid::Grid map;
		Grid::Grid display_map;

		Grid_Camera::Grid_Camera camera;

		Grid::Point wallpaper;

	}

	//do allocations and system initialization here
	void init(int screen_w, int screen_h)
	{
		srand(time(0));

		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::init(&World::map, 48, 64);
		Grid::init(&World::display_map, 48, 64);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);

		
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::map.n_cols;
		World::camera.world_coord.h = World::map.n_rows;
		
		memset(World::map.data, 0, sizeof(int)*World::map.n_cols*World::map.n_rows);
		memset(World::display_map.data, 0, sizeof(int)*World::map.n_cols*World::map.n_rows);
		

		World::current_position.x = World::map.n_cols / 2;
		World::current_position.y = World::map.n_rows / 2;

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
		Grid_Camera::calibrate(&World::camera);

		random_Walk_Step(&World::current_position, &World::map);

		for (int i = 0; i < World::display_map.n_cols * World::display_map.n_rows; i++)
		{
			if (World::map.data[i] == 0)
			{
				World::display_map.data[i] = 0;
			}
			else if (World::map.data[i] == 1)
			{
				World::display_map.data[i] = 3;
			}
			else if (World::map.data[i] <= 2)
			{
				World::display_map.data[i] = 1;
			}
			else if (World::map.data[i] == 3)
			{
				World::display_map.data[i] = 4;
			}

		}

	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		//Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);
		Tileset::draw_Grid(&World::tileset, &World::camera, &World::display_map, &RGBA::default, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}
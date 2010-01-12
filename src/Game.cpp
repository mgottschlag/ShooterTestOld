/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "Game.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "player/PlayerFactory.hpp"
#include "container/ContainerFactory.hpp"
#include "star/StarFactory.hpp"
#include "MainMenu.hpp"

Game::Game() : peak::Game()
{
	static PlayerFactory playerfactory;
	addEntityFactory(&playerfactory, "player");
	static ContainerFactory containerfactory;
	addEntityFactory(&containerfactory, "container");
	static StarFactory starfactory;
	addEntityFactory(&starfactory, "star");
}
Game::~Game()
{
}

bool Game::init()
{
	if (!graphics.init(1024, 768, false))
		return false;
	graphics.loadFile("media/drone.lfm");
	graphics.loadFile("media/container.lfm");
	graphics.loadFile("media/pointer.lfm");
	graphics.loadFile("media/soldier.lfm");
	graphics.loadFile("media/gun.lfm");
	graphics.loadFile("media/star.lfm");
	graphics.loadFile("media/bullettrail.lfm");
	graphics.loadFile("media/clearlooks.lfgt");
	/*peak::ModelSceneNode *model = new peak::ModelSceneNode("container",
		&graphics);
	model->setParent(graphics.getRootSceneNode());
	model->setTransformation(peak::Vector3F(0, 0, 10), peak::Vector3F(0, 0, 0),
		peak::OS::get().getTime());*/
	peak::TerrainSceneNode *terrain = new peak::TerrainSceneNode("terrain",
		"media/heightmap.png", "media/terrain.jpg", "media/detailmap.png",
		peak::Vector3F(1, 1.0f/16.0f, 1), &graphics);
	terrain->setTransformation(peak::Vector3F(-128, 0, -128), peak::Vector3F(0, 0, 0),
		peak::OS::get().getTime());
	terrain->setParent(graphics.getRootSceneNode());

	MainMenu *menu = new MainMenu(&graphics, this);
	peak::Menu::registerMenu(menu, "MainMenu");
	menu->setActive(true);
	return true;
}
bool Game::shutdown()
{
	graphics.shutdown();
	return true;
}

peak::Server *Game::createServer(peak::BufferPointer serverdata)
{
	peak::Server *server = new Server(getEngine());
	if (!server->init(serverdata, 27272))
	{
		delete server;
		return 0;
	}
	return server;
}
peak::Client *Game::createClient(std::string address, unsigned int port, unsigned int ms)
{
	peak::Client *client = new Client(getEngine());
	if (!client->init(address, port, ms))
	{
		delete client;
		return 0;
	}
	return client;
}
peak::Client *Game::createClient(peak::Server *server)
{
	peak::Client *client = new Client(getEngine());
	if (!client->initLocally(server))
	{
		delete client;
		return 0;
	}
	return client;
}

peak::Graphics *Game::getGraphics()
{
	return &graphics;
}

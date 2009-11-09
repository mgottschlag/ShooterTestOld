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

#include "Server.hpp"
#include "ServerContainer.hpp"

Server::Server(peak::Engine *engine) : peak::Server(engine)
{
}
Server::~Server()
{
}

bool Server::shutdown()
{
	return peak::Server::shutdown();
}
bool Server::load(peak::BufferPointer serverdata)
{
	physics.init();
	plane.init();
	planebody.init(&physics, &plane);
	terrain.init("media/heightmap.png", 0.0f, peak::Vector3F(1, 1.0f/16.0f, 1));
	terrainbody.init(&physics, &terrain);
	terrainbody.setPosition(peak::Vector3F(0, 8, 0));
	// Create some entities
	peak::Game *game = getEngine()->getGame();
	peak::EntityFactory *factory = game->getEntityFactory("container");
	return true;
}

peak::BufferPointer Server::onNewConnection(peak::Connection *connection)
{
	peak::BufferPointer serverdata = new peak::Buffer();
	serverdata->writeString("testmap");
	return serverdata;
}
void Server::onConnectionAccepted(peak::Connection *connection)
{
	// Create player
	peak::Game *game = getEngine()->getGame();
	peak::EntityFactory *factory = game->getEntityFactory("player");
	peak::ServerEntity *player = factory->createServerEntity(this);
	addEntity(player);

	peak::EntityFactory *containerfactory = game->getEntityFactory("container");
	for (unsigned int i = 0; i < 10; i++)
	{
		peak::ServerEntity *container = containerfactory->createServerEntity(this);
		((ServerContainer*)container)->setPosition(peak::Vector3F(0, 10 + i * 4, 0));
		addEntity(container);
	}
}

void Server::update()
{
	physics.update();
	peak::Server::update();
}

peak::Physics &Server::getPhysics()
{
	return physics;
}

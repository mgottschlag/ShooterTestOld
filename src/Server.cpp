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
}

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

#include "Client.hpp"

Client::Client(peak::Engine *engine) : peak::Client(engine)
{
}
Client::~Client()
{
}

bool Client::shutdown()
{
	return peak::Client::shutdown();
}
bool Client::load(peak::BufferPointer serverdata)
{
	// Create physics simulation (solely for collision detection)
	physics.init();
	plane.init();
	planebody.init(&physics, &plane);
	terrain.init("media/heightmap.png", 0.0f, peak::Vector3F(1, 1.0f/16.0f, 1));
	terrainbody.init(&physics, &terrain);
	terrainbody.setPosition(peak::Vector3F(0, 8, 0));
	return true;
}

void Client::update()
{
	physics.update();
	peak::Client::update();
}

peak::Physics &Client::getPhysics()
{
	return physics;
}

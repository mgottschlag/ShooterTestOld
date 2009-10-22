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

#include "ClientPlayer.hpp"
#include "Game.hpp"

#include <cstdio>

ClientPlayer::ClientPlayer(peak::Client *client) : ClientEntity(client),
	health(this), position(this), rotation(this), keys(this), gotinput(false)
{
	health.init(100, 8);
	addProperty(&health);
	static peak::Vector3F startpos;
	position.init(startpos);
	addProperty(&position);
	static peak::Vector2F startrot;
	rotation.init(startrot);
	addProperty(&rotation);
	keys.init(0, 8);
	addProperty(&keys);

	Game *game = (Game*)getManager()->getEngine()->getGame();
	peak::Graphics *graphics = game->getGraphics();
	camera = new peak::CameraSceneNode(graphics);
	camera->setParent(graphics->getRootSceneNode());

	graphics->addInputReceiver(this);
}
ClientPlayer::~ClientPlayer()
{
}

std::string ClientPlayer::getType()
{
	return "player";
}

void ClientPlayer::update()
{
	peak::ScopedLock lock(mutex);
	if (gotinput)
	{
		peak::Buffer *msg = new peak::Buffer();
		msg->writeUnsignedInt(currentkeys, 8);
		msg->writeInt(mousemovement.x, 16);
		msg->writeInt(mousemovement.y, 16);
		sendMessage(msg, true);
		mousemovement = peak::Vector2I(0, 0);
		gotinput = false;
	}
	camera->setTransformation(position.get(), camerarotation,
		peak::OS::get().getTime() + 40000);
}

void ClientPlayer::onKeyDown(peak::KeyCode key)
{
	peak::ScopedLock lock(mutex);
	if (key == peak::KEY_KEY_W)
	{
		currentkeys |= 0x80;
	}
	else if (key == peak::KEY_KEY_S)
	{
		currentkeys |= 0x40;
	}
	else if (key == peak::KEY_KEY_A)
	{
		currentkeys |= 0x20;
	}
	else if (key == peak::KEY_KEY_D)
	{
		currentkeys |= 0x10;
	}
	gotinput = true;
}
void ClientPlayer::onKeyUp(peak::KeyCode key)
{
	peak::ScopedLock lock(mutex);
	if (key == peak::KEY_KEY_W)
	{
		currentkeys &= ~0x80;
	}
	else if (key == peak::KEY_KEY_S)
	{
		currentkeys &= ~0x40;
	}
	else if (key == peak::KEY_KEY_A)
	{
		currentkeys &= ~0x20;
	}
	else if (key == peak::KEY_KEY_D)
	{
		currentkeys &= ~0x10;
	}
	gotinput = true;
}
void ClientPlayer::onMouseMoved(int x, int y, int dx, int dy)
{
	peak::ScopedLock lock(mutex);
	mousemovement += peak::Vector2I(dx, dy);
	gotinput = true;
	camerarotation += peak::Vector3F(dy, dx, 0) * 0.1;
}

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

#include <iostream>

ClientPlayer::ClientPlayer(peak::Client *client) : ClientEntity(client),
	health(this), position(this), rotation(this), keys(this), currentkeys(0),
	gotinput(false)
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
	// Load player model
	model = new peak::ModelSceneNode("drone", graphics);
	model->setParent(graphics->getRootSceneNode());
	// Create camera
	cameramount = new peak::GroupSceneNode(graphics);
	cameramount->setParent(model);
	camera = new peak::CameraSceneNode(graphics);
	camera->setParent(cameramount);
	camera->setTransformation(peak::Vector3F(0, 2, -10), peak::Vector3F(0, 0, 0),
		0);

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
		msg->writeFloat(camerarotation.x);
		msg->writeFloat(camerarotation.y);
		sendMessage(msg, false);
		mousemovement = peak::Vector2I(0, 0);
		gotinput = false;
	}
	model->setTransformation(position.get(),
		peak::Vector3F(0, rotation.get().y, 0),
		peak::OS::get().getTime() + 40000);
	cameramount->setTransformation(peak::Vector3F(),
		peak::Vector3F(rotation.get().x, 0, 0),
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
	else if (key == peak::KEY_SPACE)
	{
		currentkeys |= 0x08;
	}
	else if (key == peak::KEY_LSHIFT)
	{
		currentkeys |= 0x04;
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
	else if (key == peak::KEY_SPACE)
	{
		currentkeys &= ~0x08;
	}
	else if (key == peak::KEY_LSHIFT)
	{
		currentkeys &= ~0x04;
	}
	gotinput = true;
}
void ClientPlayer::onMouseMoved(int x, int y, int dx, int dy)
{
	std::cout << "Mouse movement: " << dx << "/" << dy << std::endl;
	peak::ScopedLock lock(mutex);
	mousemovement += peak::Vector2I(dx, dy);
	gotinput = true;
	camerarotation += peak::Vector3F(dy, dx, 0) * 0.1;
}

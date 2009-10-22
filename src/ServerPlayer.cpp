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

#include "ServerPlayer.hpp"

#include <iostream>

ServerPlayer::ServerPlayer(peak::Server *server) : ServerEntity(server),
	health(this), position(this), rotation(this), keys(this)
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
}
ServerPlayer::~ServerPlayer()
{
}

std::string ServerPlayer::getType()
{
	return "player";
}

void ServerPlayer::receiveMessage(peak::Buffer *buffer)
{
	std::cout << "Received message." << std::endl;
	unsigned int currentkeys = buffer->readUnsignedInt(8);
	keys.set(currentkeys);
	peak::Vector2I mousemovement;
	mousemovement.x = buffer->readInt(16);
	mousemovement.y = buffer->readInt(16);
	rotation.set(rotation.get() + peak::Vector2F(mousemovement.x, mousemovement.y) * 0.1);
}

void ServerPlayer::update()
{
	std::cout << "Updating." << std::endl;
	unsigned int currentkeys = keys.get();
	if (currentkeys != 0)
	{
		peak::Vector3F pos = position.get();
		peak::Vector3F movement;
		if (currentkeys & 0x80)
		{
			movement += peak::Vector3F(0, 0, 0.1);
		}
		if (currentkeys & 0x40)
		{
			movement += peak::Vector3F(0, 0, -0.1);
		}
		if (currentkeys & 0x20)
		{
			movement += peak::Vector3F(0.1, 0, 0);
		}
		if (currentkeys & 0x10)
		{
			movement += peak::Vector3F(-0.1, 0, 0);
		}
		peak::Vector2F rot = rotation.get();
		movement.rotate(peak::Vector3F(0, rot.x, 0));
		if (currentkeys & 0x08)
		{
			movement += peak::Vector3F(0, 0.05, 0);
		}
		if (currentkeys & 0x04)
		{
			movement += peak::Vector3F(0, -0.05, 0);
		}
		pos += movement;
		position.set(pos);
	}
}

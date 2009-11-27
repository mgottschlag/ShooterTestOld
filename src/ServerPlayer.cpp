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
#include "Server.hpp"

#include <iostream>

ServerPlayer::ServerPlayer(peak::Server *server) : ServerEntity(server),
	health(this), position(this), rotation(this), keys(this), pointerpos(this),
	clientkeys(this), clientrotation(this), lastshot(0)
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
	pointerpos.init(startpos);
	addProperty(&pointerpos);

	clientkeys.init(0, 8);
	addClientProperty(&clientkeys);
	clientrotation.init(startrot);
	addClientProperty(&clientrotation);

	character.init(&((Server*)getManager())->getPhysics());
	character.getBody()->setUserData(this);
}
ServerPlayer::~ServerPlayer()
{
}

std::string ServerPlayer::getType()
{
	return "player";
}

void ServerPlayer::onUpdate()
{
	std::cout << "Keys: " << clientkeys.get() << std::endl;
	keys.set((unsigned char)clientkeys.get());
	std::cout << "Rotation: " << clientrotation.get().x << "/" << clientrotation.get().y << std::endl;
	rotation.set(clientrotation.get());
}

void ServerPlayer::update()
{
	unsigned int currentkeys = keys.get();
	// Set speed
	peak::Vector3F speed;
	if (currentkeys & 0x80)
	{
		speed += peak::Vector3F(0, 0, 3);
	}
	if (currentkeys & 0x40)
	{
		speed += peak::Vector3F(0, 0, -3);
	}
	if (currentkeys & 0x20)
	{
		speed += peak::Vector3F(3, 0, 0);
	}
	if (currentkeys & 0x10)
	{
		speed += peak::Vector3F(-3, 0, 0);
	}
	peak::Vector2F rot = rotation.get();
	speed.rotate(peak::Vector3F(0, rot.y, 0));
	// Update character controller
	character.setHorizontalSpeed(speed);
	character.update();
	// Get the position
	position.set(character.getBody()->getPosition());
	// Set the pointer
	peak::Vector3F raystart = position.get();
	peak::Vector3F raylength(0, 0, 100);
	raylength.rotate(peak::Vector3F(rot.x, rot.y, 0));
	peak::Vector3F rayend = raystart + raylength;
	peak::Physics &physics = ((Server*)getManager())->getPhysics();
	peak::CollisionInfo collinfo;
	bool havetarget = physics.castRay(raystart, rayend, &collinfo);
	if (havetarget)
	{
		pointerpos.set(collinfo.point);
	}
	// Shooting
	bool shooting;
	unsigned int time = getManager()->getTime();
	shooting = keys.get() & 0x2;
	if (shooting && (time - lastshot > 7))
	{
		lastshot = time;
		// Create shot
		bulletrotation = peak::Vector3F(rotation.get().x, rotation.get().y, 0);
		bulletposition = peak::Vector3F(-0.4, -0.3, 2);
		bulletposition.rotate(bulletrotation);
		bulletposition += position.get() + peak::Vector3F(0, 0.4, 0);
		// Damage other player
		if (havetarget && collinfo.body->getUserData())
		{
			ServerEntity *entity = (ServerEntity*)collinfo.body->getUserData();
			if (entity->getType() == "player")
			{
				ServerPlayer *player = (ServerPlayer*)entity;
				player->damage(30);
			}
		}
	}
}

void ServerPlayer::damage(unsigned int dmg)
{
	int currenthp = health.get();
	currenthp -= dmg;
	if (currenthp < 0)
		currenthp = 0;
	health.set(currenthp);
	std::cout << "Health: " << currenthp << std::endl;
}

void ServerPlayer::setPosition(peak::Vector3F position)
{
	character.getBody()->setPosition(position);
}

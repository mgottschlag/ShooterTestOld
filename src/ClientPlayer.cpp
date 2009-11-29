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
#include "Client.hpp"

#include <iostream>

ClientPlayer::ClientPlayer(peak::Client *client, bool local)
	: ClientEntity(client, local), health(this), position(this), rotation(this),
	keys(this), pointerpos(this), clientkeys(this), clientrotation(this),
	currentkeys(0), gotinput(false), lastshot(0), hud(0)
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

	if (isLocal())
	{
		clientkeys.init(0, 8);
		addClientProperty(&clientkeys);
		clientrotation.init(startrot);
		addClientProperty(&clientrotation);
	}

	character.init(&((Client*)getManager())->getPhysics());

	Game *game = (Game*)getManager()->getEngine()->getGame();
	peak::Graphics *graphics = game->getGraphics();
	// Load camera
	if (isLocal())
	{
		camera = new peak::CameraSceneNode(graphics);
		camera->setParent(graphics->getRootSceneNode());
		camera->setTransformation(peak::Vector3F(0, 2, -10), peak::Vector3F(0, 0, 0),
			0);

		hud = new HUD(graphics);
	}

	// Pointer (debugging)
	pointer = new peak::ModelSceneNode("pointer", graphics);
	pointer->setParent(graphics->getRootSceneNode());
	if (isLocal())
	{
		pointer2 = new peak::ModelSceneNode("pointer", graphics);
		pointer2->setParent(graphics->getRootSceneNode());
	}

	// Player model
	model = new peak::ModelSceneNode("soldier", graphics);
	model->setParent(graphics->getRootSceneNode());
	gun = new peak::ModelSceneNode("gun", graphics);
	if (isLocal())
		gun->setParent(camera);
	else
		gun->setParent(model);
	gun->setTransformation(peak::Vector3F(0.4, -0.3, 0.9), peak::Vector3F(0, -90, 0),
		0);
	muzzleflash = new peak::ModelSceneNode("muzzleflash", graphics);
	muzzleflash->setVisible(false);
	muzzleflash->setParent(gun);
	bullettrail = new peak::ModelSceneNode("bullettrail", graphics);
	bullettrail->setParent(graphics->getRootSceneNode());

	if (local)
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
	unsigned int time = getManager()->getTime();
	peak::ScopedLock lock(mutex);
	if (gotinput)
	{
		std::cout << "Setting keys to " << (int)currentkeys << std::endl;
		clientkeys.set(currentkeys);
		clientrotation.set(peak::Vector2F(camerarotation.x, camerarotation.y));
		mousemovement = peak::Vector2I(0, 0);
		gotinput = false;
		// Store input
		currentinput.keys = currentkeys;
		currentinput.rotation = camerarotation;
		peak::InputHistoryFrame<PlayerInput> *inputframe;
		inputframe = new peak::InputHistoryFrame<PlayerInput>;
		inputframe->time = time;
		inputframe->data = currentinput;
		inputhistory.insert(inputframe);
	}
	// Move according to the current state of input
	if (isLocal())
		move(currentinput);
	// Update model
	if (isLocal())
	{
		camera->setTransformation(position.get() + peak::Vector3F(0, 0.4, 0),
			peak::Vector3F(rotation.get().x, rotation.get().y, 0),
			peak::OS::get().getTime() + 40000);
	}
	else
	{
		model->setTransformation(position.get(),
			peak::Vector3F(0, rotation.get().y, 0),
			peak::OS::get().getTime() + 40000);
	}

	// Update pointer
	pointer->setTransformation(pointerpos.get(), peak::Quaternion(), peak::OS::get().getTime() + 40000);
	if (isLocal())
	{
		// Set the client pointer
		peak::Vector2F rot = rotation.get();
		peak::Vector3F raystart = position.get();
		peak::Vector3F raylength(0, 0, 100);
		raylength.rotate(peak::Vector3F(rot.x, rot.y, 0));
		peak::Vector3F rayend = raystart + raylength;
		peak::Physics &physics = ((Client*)getManager())->getPhysics();
		peak::CollisionInfo collinfo;
		if (physics.castRay(raystart, rayend, &collinfo))
		{
			pointer2->setTransformation(collinfo.point, peak::Quaternion(), peak::OS::get().getTime() + 40000);
		}
	}
	// Shooting
	bool shooting;
	if (isLocal())
		shooting = currentkeys & 0x2;
	else
		shooting = keys.get() & 0x2;
	if (shooting && (time - lastshot > 7))
	{
		muzzleflash->setVisible(true);
		lastshot = time;
		// Create shot
		bulletrotation = peak::Vector3F(rotation.get().x, rotation.get().y, 0);
		bulletposition = peak::Vector3F(-0.4, -0.3, 2);
		bulletposition.rotate(bulletrotation);
		bulletposition += position.get() + peak::Vector3F(0, 0.4, 0);
		bullettrail->setTransformation(bulletposition, bulletrotation,
			peak::OS::get().getTime() + 40000);
	}
	else if (muzzleflash->isVisible())
	{
		muzzleflash->setVisible(false);
	}
	// Move bullet trail
	peak::Vector3F relmovement(0, 0, 2);
	relmovement.rotate(bulletrotation);
	bulletposition += relmovement;
	bullettrail->setTransformation(bulletposition, bulletrotation,
		peak::OS::get().getTime() + 40000);
	
	
}

void ClientPlayer::onUpdate(unsigned int acktime)
{
	if (!isLocal())
		return;
	peak::ScopedLock lock(mutex);
	// Create backup
	createBackup();
	// Clean input history
	inputhistory.removeIrrelevant(acktime + 1);
	// Apply input history
	unsigned int currenttime = getManager()->getTime();
	/*std::cout << "Lag: approx. " << 20 * (currenttime - acktime) / 2 << " ms." << std::endl;
	std::cout << "Times: " << currenttime << ", " << acktime << std::endl;*/
	peak::InputHistoryFrame<PlayerInput> *input = inputhistory.get();
	unsigned int replayedtime = 0;
	while (input)
	{
		// Get time limits
		unsigned int starttime = input->time;
		unsigned int endtime = currenttime;
		if (input->next)
			endtime = input->next->time;
		// Apply input frame
		replayedtime += endtime - starttime;
		//std::cout << "Keys: " << (unsigned int)input->data.keys << std::endl;
		for (unsigned int i = starttime; i < endtime; i++)
		{
			move(input->data);
		}
		// Next frame
		input = input->next;
	}
	//std::cout << "Replayed " << replayedtime << " input frames." << std::endl;

	if (hud)
		hud->setHealth(health.get());
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
	else if (key == peak::KEY_LBUTTON)
	{
		currentkeys |= 0x02;
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
	else if (key == peak::KEY_LBUTTON)
	{
		currentkeys &= ~0x02;
	}
	gotinput = true;
}
void ClientPlayer::onMouseMoved(int x, int y, int dx, int dy)
{
	peak::ScopedLock lock(mutex);
	mousemovement += peak::Vector2I(dx, dy);
	gotinput = true;
	camerarotation += peak::Vector2F(dy, dx) * 0.1;
}
void ClientPlayer::move(const PlayerInput &input)
{
	// Move
	character.getBody()->setPosition(position.get());
	// Set speed
	peak::Vector3F speed;
	if (input.keys & 0x80)
	{
		speed += peak::Vector3F(0, 0, 3);
	}
	if (input.keys & 0x40)
	{
		speed += peak::Vector3F(0, 0, -3);
	}
	if (input.keys & 0x20)
	{
		speed += peak::Vector3F(3, 0, 0);
	}
	if (input.keys & 0x10)
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
	// Set rotation
	rotation.set(input.rotation);
}

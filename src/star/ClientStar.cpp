/*
Copyright (c) 2009-2010, Mathias Gottschlag, Christian Reiser

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

#include "star/ClientStar.hpp"
#include "Game.hpp"
#include "Client.hpp"

ClientStar::ClientStar(peak::Client *client, bool local)
	: ClientEntity(client, local), position(this), rotation(this)
{
	static peak::Vector3F startpos;
	position.init(startpos);
	addProperty(&position);
	static peak::Quaternion startrot;
	rotation.init(startrot);
	addProperty(&rotation);

	peak::TrimeshData data;
	data.init("media/star.obj");
	shape.init(data, 0.0f);
	body.init(&((Client*)getManager())->getPhysics(), &shape);
	data.destroy();

	Game *game = (Game*)getManager()->getEngine()->getGame();
	peak::Graphics *graphics = game->getGraphics();
	// Load container model
	translation = new peak::GroupSceneNode(graphics);
	translation->setParent(graphics->getRootSceneNode());
	model = new peak::ModelSceneNode("star", graphics);
	model->setParent(translation);
	model->setTransformation(peak::Vector3F(0, 0, 0), peak::Quaternion(),
		peak::OS::get().getTime());
}
ClientStar::~ClientStar()
{
}

std::string ClientStar::getType()
{
	return "star";
}

void ClientStar::update()
{
	// Update physics shape
	body.setPosition(position.get());
	body.setRotation(rotation.get());
	// Update model
	translation->setTransformation(position.get(), rotation.get(),
		peak::OS::get().getTime() + 40000);
}

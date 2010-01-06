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

#include "ServerContainer.hpp"
#include "Server.hpp"

ServerContainer::ServerContainer(peak::Server *server) : ServerEntity(server),
	position(this), rotation(this)
{
	static peak::Vector3F startpos;
	position.init(startpos);
	addProperty(&position);
	static peak::Quaternion startrot;
	rotation.init(startrot);
	addProperty(&rotation);

	shape.init(peak::Vector3F(2.4f, 2.5f, 6.0f), 100.0f);
	body.init(&((Server*)getManager())->getPhysics(), &shape);
}
ServerContainer::~ServerContainer()
{
}

std::string ServerContainer::getType()
{
	return "container";
}

void ServerContainer::update()
{
	position.set(body.getPosition());
	rotation.set(body.getRotation());
}

void ServerContainer::setPosition(peak::Vector3F position)
{
	body.setPosition(position);
}

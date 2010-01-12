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

#include "star/ServerStar.hpp"
#include "Server.hpp"

ServerStar::ServerStar(peak::Server *server) : ServerEntity(server),
	position(this), rotation(this)
{
	static peak::Vector3F startpos;
	position.init(startpos);
	addProperty(&position);
	static peak::Quaternion startrot;
	rotation.init(startrot);
	addProperty(&rotation);

	peak::TrimeshData data;
	data.init("media/star.obj");
	shape.init(data,  0.0f);
	body.init(&((Server*)getManager())->getPhysics(), &shape);
	data.destroy();
}
ServerStar::~ServerStar()
{
}

std::string ServerStar::getType()
{
	return "star";
}

void ServerStar::update()
{
	position.set(body.getPosition());
	rotation.set(body.getRotation());
}

void ServerStar::setPosition(peak::Vector3F position)
{
	body.setPosition(position);
}

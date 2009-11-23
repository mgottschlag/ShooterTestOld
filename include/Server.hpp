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

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <PeakEngine.hpp>
#include <PeakPhysics.hpp>

class Server : public peak::Server
{
	public:
		Server(peak::Engine *engine);
		virtual ~Server();

		virtual bool shutdown();
		virtual bool load(peak::BufferPointer serverdata);

		virtual peak::BufferPointer onNewConnection(peak::Connection *connection);
		virtual void onConnectionAccepted(peak::ClientInfo &client);

		virtual void update();

		peak::Physics &getPhysics();
	private:
		peak::Physics physics;
		peak::Plane plane;
		peak::Body planebody;
		peak::Heightfield terrain;
		peak::Body terrainbody;
};

#endif

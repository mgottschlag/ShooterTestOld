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

#ifndef _SERVERPLAYER_HPP_
#define _SERVERPLAYER_HPP_

#include <PeakEngine.hpp>
#include <PeakPhysics.hpp>

class ServerPlayer : public peak::ServerEntity
{
	public:
		ServerPlayer(peak::Server *server);
		virtual ~ServerPlayer();

		virtual std::string getType();

		virtual void onUpdate();

		virtual void update();

		void damage(unsigned int dmg);

		void setPosition(peak::Vector3F position);
	private:
		peak::IntProperty health;
		peak::Vector3FProperty position;
		peak::Vector2FProperty rotation;
		peak::IntProperty keys;
		peak::Vector3FProperty pointerpos;

		peak::IntProperty clientkeys;
		peak::Vector2FProperty clientrotation;

		peak::Vector3F bulletrotation;
		peak::Vector3F bulletposition;
		unsigned int lastshot;

		/*peak::Capsule shape;
		peak::Body body;*/
		peak::CharacterController character;
};

#endif

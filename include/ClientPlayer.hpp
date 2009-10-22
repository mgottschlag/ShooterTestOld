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

#ifndef _CLIENTPLAYER_HPP_
#define _CLIENTPLAYER_HPP_

#include <PeakEngine.hpp>
#include <PeakGraphics.hpp>

class ClientPlayer : public peak::ClientEntity, public peak::InputReceiver
{
	public:
		ClientPlayer(peak::Client *client);
		virtual ~ClientPlayer();

		virtual std::string getType();

		virtual void update();

		virtual void onKeyDown(peak::KeyCode key);
		virtual void onKeyUp(peak::KeyCode key);
		virtual void onMouseMoved(int x, int y, int dx, int dy);
	private:
		peak::IntProperty health;
		peak::Vector3FProperty position;
		peak::Vector2FProperty rotation;
		peak::IntProperty keys;

		unsigned char currentkeys;
		peak::Vector2I mousemovement;
		bool gotinput;

		peak::Vector3F camerarotation;
		peak::CameraSceneNode *camera;

		peak::Mutex mutex;
};

#endif
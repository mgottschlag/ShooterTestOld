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

#ifndef _MAINMENU_HPP_
#define _MAINMENU_HPP_

#include "PeakGraphics.hpp"

class Game;

class MainMenu : public peak::Menu
{
	public:
		MainMenu(peak::Graphics *graphics, Game *game);
		virtual ~MainMenu();

		virtual std::string getName();

		virtual void onAction(peak::MenuElement *element);
	private:
		Game *game;

		peak::MenuButton *singleplayer;
		peak::MenuButton *createserver;
		peak::MenuButton *joinserver;
		peak::MenuButton *settings;
		peak::MenuButton *exit;
};

#endif

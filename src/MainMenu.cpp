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

#include "MainMenu.hpp"
#include "Game.hpp"

#include <iostream>

MainMenu::MainMenu(peak::Graphics *graphics, Game *game)
	: peak::Menu(graphics, "clearlooks"), game(game)
{
	setClear(true);
	setClearColor(0xFF0081b7);
	// Create widgets
	singleplayer = new peak::MenuButton(graphics, "Singleplayer", this);
	singleplayer->queueForLoading();
	singleplayer->setPosition(peak::ScreenPosition(peak::Vector2F(1.0, 0.5),
		peak::Vector2I(-220, -100)));
	singleplayer->setSize(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(200, 30)));
	createserver = new peak::MenuButton(graphics, "Create server", this);
	createserver->queueForLoading();
	createserver->setPosition(peak::ScreenPosition(peak::Vector2F(1.0, 0.5),
		peak::Vector2I(-220, -60)));
	createserver->setSize(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(200, 30)));
	joinserver = new peak::MenuButton(graphics, "Join server", this);
	joinserver->queueForLoading();
	joinserver->setPosition(peak::ScreenPosition(peak::Vector2F(1.0, 0.5),
		peak::Vector2I(-220, -20)));
	joinserver->setSize(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(200, 30)));
	settings = new peak::MenuButton(graphics, "Settings", this);
	settings->queueForLoading();
	settings->setPosition(peak::ScreenPosition(peak::Vector2F(1.0, 0.5),
		peak::Vector2I(-220, 20)));
	settings->setSize(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(200, 30)));
	exit = new peak::MenuButton(graphics, "Exit game", this);
	exit->queueForLoading();
	exit->setPosition(peak::ScreenPosition(peak::Vector2F(1.0, 0.5),
		peak::Vector2I(-220, 60)));
	exit->setSize(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(200, 30)));
	logo = new peak::MenuImage(graphics, "media/logo.png", this);
	logo->queueForLoading();
	logo->setPosition(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(30, 30)));
	logo->setSize(peak::ScreenPosition(peak::Vector2F(0.0, 0.0),
		peak::Vector2I(512, 256)));
}
MainMenu::~MainMenu()
{
}

std::string MainMenu::getName()
{
	return "MainMenu";
}

void MainMenu::onAction(peak::MenuElement *element)
{
	if ((element == singleplayer) || (element == createserver))
	{
		// TODO: Create a local server for single player games
		// Create server/client
		peak::Engine *engine = game->getEngine();
		peak::BufferPointer serverdata = new peak::Buffer();
		serverdata->writeString("testmap");
		if (!engine->createServer(serverdata))
		{
			return;
		}
		engine->createLocalClient();
		// Hide menu
		setActive(false);
		getGraphics()->setMouseLocked(true);
	}
	if (element == joinserver)
	{
		// Create client
		peak::Engine *engine = game->getEngine();
		if (!engine->createClient("phoenix64.dyndns.org", 27272, 5000))
			return;
		// Hide menu
		setActive(false);
		getGraphics()->setMouseLocked(true);
	}
	if (element == exit)
	{
		// Stop engine
		game->getEngine()->stop();
	}
}

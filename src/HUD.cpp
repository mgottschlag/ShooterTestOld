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

#include "HUD.hpp"

HUD::HUD(peak::Graphics *graphics) : SpriteLayer(graphics)
{
	// Static sprites
	infobox = new peak::Sprite(this, "media/hud.png",
		peak::ScreenPosition(peak::Vector2F(0.0, 1.0), peak::Vector2I(0, -104)),
		peak::ScreenPosition(peak::Vector2F(), peak::Vector2I(169, 104)),
		peak::Vector2I(0, 152), peak::Vector2I(169, 104));
	crosshair = new peak::Sprite(this, "media/hud.png",
		peak::ScreenPosition(peak::Vector2F(0.5, 0.5), peak::Vector2I(-16, -16)),
		peak::ScreenPosition(peak::Vector2F(), peak::Vector2I(32, 32)),
		peak::Vector2I(0, 120), peak::Vector2I(32, 32));
	// Health bar
	healthbar = new peak::Sprite(this, "media/hud.png",
		peak::ScreenPosition(peak::Vector2F(0.0, 1.0), peak::Vector2I(47, -70)),
		peak::ScreenPosition(peak::Vector2F(), peak::Vector2I(87, 10)),
		peak::Vector2I(169, 246), peak::Vector2I(1, 10));
	healthbarbg = new peak::Sprite(this, "media/hud.png",
		peak::ScreenPosition(peak::Vector2F(0.0, 1.0), peak::Vector2I(47, -70)),
		peak::ScreenPosition(peak::Vector2F(), peak::Vector2I(0, 10)),
		peak::Vector2I(169, 226), peak::Vector2I(1, 10));
	// Ammunition bar
	ammobar = new peak::Sprite(this, "media/hud.png",
		peak::ScreenPosition(peak::Vector2F(0.0, 1.0), peak::Vector2I(47, -24)),
		peak::ScreenPosition(peak::Vector2F(), peak::Vector2I(87, 10)),
		peak::Vector2I(169, 236), peak::Vector2I(1, 10));
	ammobarbg = new peak::Sprite(this, "media/hud.png",
		peak::ScreenPosition(peak::Vector2F(0.0, 1.0), peak::Vector2I(47, -24)),
		peak::ScreenPosition(peak::Vector2F(), peak::Vector2I(0, 10)),
		peak::Vector2I(169, 226), peak::Vector2I(1, 10));
}
HUD::~HUD()
{
}

void HUD::setHealth(int health)
{
	healthbar->setSize(peak::ScreenPosition(peak::Vector2F(),
		peak::Vector2I((int)(87.0 * health / 100), 10)));
	healthbarbg->setPosition(peak::ScreenPosition(peak::Vector2F(0.0, 1.0),
		peak::Vector2I(47 + (int)(87.0 * health / 100), -70)));
	healthbarbg->setSize(peak::ScreenPosition(peak::Vector2F(),
		peak::Vector2I((int)(87.0 - 87.0 * health / 100), 10)));
}
void HUD::setAmmunition(int ammo)
{
	ammobar->setSize(peak::ScreenPosition(peak::Vector2F(),
		peak::Vector2I((int)(87.0 * ammo / 100), 10)));
	ammobarbg->setPosition(peak::ScreenPosition(peak::Vector2F(0.0, 1.0),
		peak::Vector2I(47 + (int)(87.0 * ammo / 100), -24)));
	ammobarbg->setSize(peak::ScreenPosition(peak::Vector2F(),
		peak::Vector2I((int)(87.0 - 87.0 * ammo / 100), 10)));
}

#include "PlayerHubControl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
using namespace std;
PlayerHubControl::~PlayerHubControl()
{
}

void PlayerHubControl::update()
{
	auto& ihdlr = ih();
	auto currentTime = sdlutils().currRealTime();
	auto& vel = tr_->getVel();


	//movimiento en 8 direcciones
	if (ihdlr.keyDownEvent()) {


		if (ihdlr.isKeyDown(SDLK_w)) {

			vel.set(Vector2D(vel.getX(), -speed_));
		}
		if (ihdlr.isKeyDown(SDLK_a)) {

			vel.set(Vector2D(-speed_, vel.getY()));
		}
		if (ihdlr.isKeyDown(SDLK_d)) {

			vel.set(Vector2D(speed_, vel.getY()));
		}
		if (ihdlr.isKeyDown(SDLK_s)) {

			vel.set(Vector2D(vel.getX(), speed_));
		}
	}
	else {

		vel.set(Vector2D(0, 0));
	}
	vel.normalize();
}

void PlayerHubControl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);

	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
	playerCol_ = ent_->getComponent<RectangleCollider>();
	assert(playerCol_ != nullptr);
}

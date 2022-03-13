#include "HammerArm.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Manager.h"

HammerArm::HammerArm() :tr_(nullptr), state_(REPOSO), initialPos()
{
}

HammerArm::~HammerArm()
{
}

void HammerArm::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr, collider_ != nullptr, playertr_ != nullptr);

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
}

void HammerArm::goDiagonal()
{
	collider_->setActive(false);

	if (abs(tr_->getPos().getX() - playerXPos) > 5 || abs(tr_->getPos().getY()) > 5) {
		Vector2D dir = Vector2D(playerXPos - tr_->getPos().getX(), - tr_->getPos().getY());/*initialPos - tr_->getPos();*/
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(Vector2D(playerXPos, sdlutils().height() - tr_->getHeight()));
		changeState(HIT);
	}
}


void HammerArm::attack()
{

	collider_->setActive(true);
	collider_->setIsTrigger(true);

	if (tr_->getPos().getY() < sdlutils().height() - tr_->getHeight()) {
		tr_->getVel().set(Vector2D(handSpeed * 2, 0));
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().setY(sdlutils().height() - tr_->getHeight());
		lastTimeFloor = sdlutils().currRealTime();
		changeState(REPOSOSUELO);
	}
}

void HammerArm::goBack()
{
	if (abs(tr_->getPos().getX() - initialPos.getX()) > 5 || abs(tr_->getPos().getY() - initialPos.getY()) > 5) {
		Vector2D dir = initialPos - tr_->getPos();
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(initialPos);
		changeState(FIN);
	}
}

void HammerArm::stayFloor() {
	collider_->setIsTrigger(false);
	if (sdlutils().currRealTime() > lastTimeFloor + cooldoownInFloor)
		changeState(BACK);
}

void HammerArm::getPlayerX()
{
	playerXPos = playertr_->getPos().getX();
}

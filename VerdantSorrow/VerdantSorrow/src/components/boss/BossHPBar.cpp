#include "BossHPBar.h"

#include <cassert>

#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include "../../game/SceneManager.h"

BossHPBar::BossHPBar() :
	attrib_(), maxBarLength(), pos(), accumulatedDamage(),
	lastHP(), accumulatedDmgDecrease(), accumulatedDmgFirstDecrease(),
	bossBarArrow_left(&sdlutils().images().at("bossBarArrow_Left")),
	bossBarArrow_right(&sdlutils().images().at("bossBarArrow_Right")),
	middleBar(&sdlutils().images().at("middleBar")), damageDecreaseTimer_()
{ }

BossHPBar::~BossHPBar() {
}

void BossHPBar::initComponent() {
	if (SceneManager::scenes::Frog_ == sC().getScene()) attrib_ = mngr_->getHandler(ecs::_FROGBOSS)->getComponent<BossAtributos>();
	else if (SceneManager::scenes::Tree_ == sC().getScene()) attrib_ = mngr_->getHandler(ecs::_LANTERN)->getComponent<BossAtributos>();
	else if (SceneManager::scenes::Eye_ == sC().getScene()) attrib_ = mngr_->getHandler(ecs::_EYE)->getComponent<BossAtributos>();
	assert(attrib_ != nullptr);

	lastHP = attrib_->getLife();
	maxBarLength = sdlutils().width() * 0.5f;
	pos = Vector2D((sdlutils().width() - maxBarLength) / 2, sdlutils().height() - 80);
	damageDecreaseTimer_ = mngr_->addTimer();
}

void BossHPBar::render() {

	//suma de da�o acumulado
	if (accumulatedDamage == 0)
		accumulatedDmgFirstDecrease = damageDecreaseTimer_->currTime() + 1000;

	accumulatedDamage += (lastHP - attrib_->getLife());

	// Fondo de las barras
	int size = 52;
	SDL_Rect left = build_sdlrect(pos.getX() - size, pos.getY() - size/4, size, size);
	SDL_Rect right = build_sdlrect(pos.getX() + maxBarLength, pos.getY() - size/4, size, size);
	SDL_Rect middle = build_sdlrect(pos.getX(), pos.getY(), maxBarLength, size/2);


	size = 15;
	//barra roja
	SDL_Rect rect = build_sdlrect(pos.getX(), pos.getY() + (middle.h - size)/2, maxBarLength * (attrib_->getLife() / attrib_->getMaxHp()), size);
	//barra de da�o acumulado
	SDL_Rect rect2 = build_sdlrect(pos.getX() + rect.w, pos.getY() + (middle.h - size) / 2, maxBarLength * (accumulatedDamage / attrib_->getMaxHp()), rect.h);

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	scaleRect(left, sW, sH);
	scaleRect(right, sW, sH);
	scaleRect(middle, sW, sH);
	scaleRect(rect, sW, sH);
	scaleRect(rect2, sW, sH);

	//dibujar interfaz barra
	bossBarArrow_left->render(left); // 50 grosor
	bossBarArrow_right->render(right); // 50 grosor
	middleBar->render(middle); // 25 grosor

	//dibujar barras
	SDL_SetRenderDrawColor(sdlutils().renderer(), 58, 2, 0, 255);
	SDL_RenderFillRect(sdlutils().renderer(), &rect);
	SDL_SetRenderDrawColor(sdlutils().renderer(), 110, 89, 18, 255);
	SDL_RenderFillRect(sdlutils().renderer(), &rect2);

	//cada accumulatedDmgDecreaseCooldown reduce un poco la barra de da�o acumulado
	//si el da�o acumulado era 0, espera un poco antes de empezar a bajar
	if (damageDecreaseTimer_->currTime() >= accumulatedDmgFirstDecrease) {
		if (damageDecreaseTimer_->currTime() >= accumulatedDmgDecrease) {
			accumulatedDamage = (accumulatedDamage - 0.05 > 0 ? accumulatedDamage -= 0.05 : accumulatedDamage = 0);
			accumulatedDmgDecrease = damageDecreaseTimer_->currTime() + 15;
		}
	}

	lastHP = attrib_->getLife();
}

void BossHPBar::scaleRect(SDL_Rect& rect, float sW, float sH)
{
	rect.x *= sW;
	rect.w *= sW;
	rect.y *= sH;
	rect.h *= sH;
}

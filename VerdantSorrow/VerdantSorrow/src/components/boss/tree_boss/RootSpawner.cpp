#include "RootSpawner.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"
#include "../../Image.h"
#include "../../fondos/ParticleSystem.h"

RootSpawner::RootSpawner(CollisionManager* collManager) : collManager_(collManager), framedImg_(), rootWidth_(60)
{
}

RootSpawner::~RootSpawner()
{
}

void RootSpawner::initComponent()
{
}

void RootSpawner::createRoot(int x)
{
	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se a�aden los atributos del boss que est�n junto al transform
	auto RootAtribs = Root->addComponent<BossAtributos>(1);
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = sdlutils().height();
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la raiz
	RootTr->init(Vector2D(RootX, RootY), Vector2D(), rootWidth_, sdlutils().height() - 50, 0.0f);
	//Se le a�ade un color inicial a la raiz
	auto RootImg = Root->addComponent<Image>(&sdlutils().images().at("root"));

	//Se a�ade un collider a la raiz
	auto RootCollider = Root->addComponent<RectangleCollider>(RootTr->getWidth(), RootTr->getHeight());
	RootCollider->setIsTrigger(true);
	//Se a�ade el collider al colliderGameManager
	collManager_->addCollider(RootCollider);
	//Se a�ade el movimiento vertical
	Root->addComponent<RootMovement>();
	Root->addToGroup(ecs::_BOSS_GRP);

	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_tierra"), mngr_);
	particlesys->createParticlesRoots(10, RootX + (rootWidth_ / 2), sdlutils().height() - 50);
}

#include "FrogScene.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../components/RectangleRenderer.h"
#include "../components/RectangleCollider.h"
#include "../components/Transform.h"
#include "../components/FramedImage.h"
#include "../components/Image.h"
#include "../components/player/PlayerComponents.h"
#include "../components/boss/BossComponents.h"
#include "../components/boss/wave/WaveMovement.h"
#include "../components/boss/frog_boss/FrogAttackManager.h"
#include "../components/fondos/ShowAtOppositeSide.h"
#include "Fondos/Parallax.h"

#include "CollisionManager.h"
#include "../components/boss/wave/WaveSpawner.h"
#include "SceneManager.h"
#include "../game/CollisionChecker.h"
#include "../components/fondos/ParticleSystem.h"
#include "../components/fondos/Light.h"


void FrogScene::init()
{
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();

	background();

	waveSpawerGenerator(colManager);

	//Se crea el jugador
	player = mngr_->addEntity();
	playerGenerator(colManager, player);

	frogGenerator(colManager, player);

	colCheck_ = new CollisionChecker(colManager, mngr_);
	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particle"), mngr_);
	particlesys->createParticlesSnow(100);

	createLights();
}

void FrogScene::update()
{
	auto health = 0;
	auto bossHealth = 0;
	if (player != nullptr)
		health = player->getComponent<PlayerAttributes>()->getLives();
	if (Frog != nullptr)
		bossHealth = Frog->getComponent<BossAtributos>()->getLife();
	if (health > 0 && bossHealth > 0) {
		mngr_->update();
		colCheck_->checkCollisions();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
#ifdef _DEBUG
		mngr_->debug();
#endif		
		sdlutils().presentRenderer();
	}
	else {
		if (health <= 0) sC().changeFrogEssenceState(true);
		if (bossHealth <= 0)sC().changeStatePlayerInBoss(false);

		setAble(false);
		sC().decideScene();
	}
}

void FrogScene::waveSpawerGenerator(CollisionManager*& colManager)
{
	//se crea wave spwner
	auto waveSp = mngr_->addEntity();
	waveSp->addComponent<WaveSpawner>(colManager);
	mngr_->setHandler(ecs::_WAVE_GENERATOR, waveSp);
	waveSp->addToGroup(ecs::_BOSS_GRP);
}

void FrogScene::background()
{
	auto blCK = mngr_->addEntity();
	blCK->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	blCK->addComponent<Image>(&sdlutils().images().at("fondoNegro"));
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().images().at("fondo1"));

	auto parallax_ = new Parallax(mngr_);
	parallax_->AddLayer(&sdlutils().images().at("nubes"), 14, sdlutils().height(), sdlutils().width(), 0);
	/*auto nubes = mngr_->addEntity();
	auto nubestr = nubes->addComponent<Transform>(Vector2D(0, 0), Vector2D(-5,0), sdlutils().width(), sdlutils().height()/2, 0.0f);
	nubes->addComponent<Image>(&sdlutils().images().at("nubes"));
	nubes->addComponent<ShowAtOpposideSide>();*/
	auto fondodel = mngr_->addEntity();
	auto fondotr = fondodel->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	fondodel->addComponent<Image>(&sdlutils().images().at("fondodel"));

	auto suelo = mngr_->addEntity();
	auto suelo_Tr = suelo->addComponent<Transform>(Vector2D(0, sdlutils().height() - sdlutils().height()), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	suelo->addComponent<Image>(&sdlutils().images().at("Suelo"));
	blCK->addToGroup(ecs::_BACKGROUND_1_GRP);
	backgr_->addToGroup(ecs::_BACKGROUND_1_GRP);
	//nubes->addToGroup(ecs::_BACKGROUND_1_GRP);
	fondodel->addToGroup(ecs::_BACKGROUND_1_GRP);
	suelo->addToGroup(ecs::_BACKGROUND_1_GRP);
}

void FrogScene::frogGenerator(CollisionManager* colManager, Entity* player_) {

	Frog = mngr_->addEntity();
	Frog->addToGroup(ecs::_BOSS_GRP);
	mngr_->setHandler(ecs::_FROGBOSS, Frog);
	auto FrogAtribs = Frog->addComponent<BossAtributos>(40);

	auto frogH = 300;
	auto frogW = frogH * 1.11f;
	auto FrogX = sdlutils().width() / 2 - 25;
	auto FrogY = sdlutils().height() - frogH;
	//auto FrogTr = Frog->addComponent<Transform>(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f);
	auto FrogTr = Frog->addComponent<Transform>();

	FrogTr->init(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f, 1);

	Frog->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 5000, 32, "ranajump");

	//Se añade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth() - 150, FrogTr->getHeight() - 200, 0, 75);
	frogCollider->setIsTrigger(true);
	colManager->addCollider(frogCollider);

	Frog->addComponent<SimpleGravity>(.5);
	Frog->addComponent<CollideWithBordersBoss>();

	Frog->addComponent<FrogAttackManager>(colManager);

	Frog->addComponent<BossHPBar>();
	auto frogHPBar = mngr_->addEntity();
	frogHPBar->addComponent<BossHPBar>();
	frogHPBar->addToGroup(ecs::_UI_GRP);
}

bool FrogScene::getAble()
{
	return isAble;
}

void FrogScene::setAble(bool a)
{
	isAble = a;
}

void FrogScene::createLights() {
	new Light(&sdlutils().images().at("luz_cyan"), 100, sdlutils().height() - 200, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_cyan"), 300, sdlutils().height() - 200, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_cyan"), 500, sdlutils().height() - 200, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_cyan"), 700, sdlutils().height() - 200, 500, 100, mngr_);

	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 200, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 400, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 600, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 800, 400, 100, mngr_);

	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width() - 200, sdlutils().height() - 200, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width() - 200, sdlutils().height() - 400, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width() - 200, sdlutils().height() - 600, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width() - 200, sdlutils().height() - 800, 400, 100, mngr_);

	new Light(&sdlutils().images().at("luz_verde"), 1100, 300, 300, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 50, 200, 350, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 400, 100, 200, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 850, 50, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 600, 150, 350, 100, mngr_);
}
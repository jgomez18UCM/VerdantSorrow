#include "TreeScene.h"

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
#include "../components/boss/tree_boss/TreeAttackManager.h"
#include "../components/boss/tree_boss/RootWave.h"
#include "../components/boss/tree_boss/Root/RootMovement.h"
#include "../components/boss/tree_boss/RootSpawner.h"
#include "../components/boss/tree_boss/RootAutoAim.h"
#include "../components/boss/tree_boss/MeleeAttack.h"
#include "../components/boss/tree_boss/TreeMovement.h"


#include "CollisionManager.h"



void TreeScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	treeGenerator(colManager, player);
}


void TreeScene::treeGenerator(CollisionManager* colManager, Entity* player_) {

	auto Tree_ = mngr_->addEntity();
	auto TreeAtribs = Tree_->addComponent<BossAtributos>(10.0f);

	auto TreeTr = Tree_->addComponent<Transform>();
	auto TreeX = sdlutils().width() / 4 * 3 - 80;
	auto TreeY = sdlutils().height() - 320;
	TreeTr->init(Vector2D(TreeX, TreeY), Vector2D(), 160, 320, 0.0f);

	Tree_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Se añade un collider al arbol
	auto treeCollider = Tree_->addComponent<RectangleCollider>(TreeTr->getWidth(), TreeTr->getHeight());
	treeCollider->setIsTrigger(true);
	colManager->addCollider(treeCollider);

	Tree_->addComponent<BossHPBar>();

	//IMPORTANTE: movimiento y spawner antes de los ataques
	Tree_->addComponent<TreeMovement>(player_->getComponent<Transform>(), 2.0f);
	Tree_->addComponent<RootSpawner>(colManager);
	
	Tree_->addComponent<RootWave>();
	Tree_->addComponent<RootAutoAim>(player_);
	Tree_->addComponent<MeleeAttack>(50, player_->getComponent<Transform>()->getHeight(), colManager);
	
	//IMPORTANTE: attack manager al final
	Tree_->addComponent<TreeAttackManager>(colManager);
}

void TreeScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
	player_->addComponent<PlayerAttributes>();

	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 4 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 100, 200, 0.0f);


	player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 7, 5000, 30, "Chica_Idle");
	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre 
	player_->addComponent<SimpleGravity>(2.0);
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player_->addComponent<CollideWithBorders>();

	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth() - 16, playerTr->getHeight());
	colManager->addCollider(playerCollider);
	player_->addComponent<PlayerCtrl>(23, 8, 0.85, 12);

	//IMPORTANTE :No poner estas físicas detrás del playerctrl
	player_->addComponent<SimplePhysicsPlayer>(colManager);

	//Componente de ataque del jugador
	auto playerAttackCollider = player_->addComponent<Attack>(50,playerTr->getHeight(), colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player_);
}

void TreeScene::rootGenerator(CollisionManager* colManager, Entity* player_, float x) {

	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto RootAtribs = Root->addComponent<BossAtributos>();
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = sdlutils().height()+200;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la raiz
	RootTr->init(Vector2D(RootX, RootY), Vector2D(), 25, 1000, 0.0f);
	//Se le añade un color inicial a la raiz
	Root->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la onda
	auto RootCollider = Root->addComponent<RectangleCollider>(RootTr->getWidth(), RootTr->getHeight());
	RootCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	colManager->addCollider(RootCollider);
	//Se añade el movimiento horizontal
	Root->addComponent<RootMovement>();
}
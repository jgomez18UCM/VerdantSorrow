#pragma once
#include "../utils/Singleton.h"
#include "Scene.h"
#include "FrogScene.h"
#include "TreeScene.h"
#include "FinalBossScene.h"
#include "Hub.h"
#include "TutorialScene.h"
#include "MenuScene.h"
#include "ControlsScene.h"
#include "PauseMenu.h"
#include <vector>
class SceneManager :public Singleton<SceneManager> {

	friend Singleton<SceneManager>;
public:
	enum scenes { Hub_, Frog_, Tree_, Eye_, Tutorial_, Menu_, Controls_, PauseMenu_ };
	~SceneManager();
	void update();
	//inicializa la escena en la que est�
	void init();
	void render();
	scenes getScene() { return actScene; }
	void changeScene(scenes s_=Hub_);
	void changeFrogEssenceState(bool active);
	void changeTreeEssenceState(bool active);
	void changeEyeEssenceState(bool active);
	void FrogSceneState(bool active);
	void TreeSceneState(bool active);
	void EyeSceneState(bool active);
	void decideScene();
	//activa la primera escena a la que se puede acceder desde el hub
	void setFirstSceneAble();
private:
	SceneManager();
	//decide a que escena hay que cambiar
	scenes actScene;
	std::vector <Scene*> sceneList;
	Hub* h_;
	FrogScene* f_;
	TreeScene* t_;
	FinalBossScene* fin_;
	TutorialScene* tut_;
	MenuScene* menu_;
	ControlsScene* controls_;
	PauseMenu* pauseMenu_;
	bool frogEssenceObtained_, treeEssenceObtained_, eyeEssenceObtained_;
};
	// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
	inline SceneManager& sC() {
		return *SceneManager::instance();
	}


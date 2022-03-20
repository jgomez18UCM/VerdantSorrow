#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../utils/Vector2D.h"

class Transform;

class WaveMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_WAVEMOVEMENT)
	WaveMovement(Vector2D dir, float speed);
	~WaveMovement();

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	Vector2D dir_;
	float speed_;
	float lastTime_;
};
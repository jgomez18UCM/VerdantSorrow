#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

class Transform;

class HammerArm : public ecs::Component
{
public:

    enum State {
        DIAGONAL = 0,
        HIT,
        BACK,
        REPOSO,
        REPOSOSUELO,
        FIN
    };

    __CMPID_DECL__(ecs::_HAMMER_ARM)

        HammerArm(bool leftHand);
        ~HammerArm();

    void initComponent() override;

    void attack();
    void goBack();
    void goDiagonal();
    void stayFloor();
    void getPlayerX() { playerXPos = playertr_->getPos().getX(); }

    State getstate() { return state_; };
    void changeState(State s) { state_ = s; };

protected:
    Transform* tr_;
    RectangleCollider* collider_;
    bool leftHand_;
    int handSpeed = 6, cooldoownInFloor = 1500, lastTimeFloor;
    Transform* playertr_;
    int playerXPos;

    Vector2D initialPos;
    State state_;
};


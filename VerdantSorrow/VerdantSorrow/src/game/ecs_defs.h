// This file is part of the course TPV2@UCM - Samir Genaim

// DO NOT USE/MODIFY THIS FILE, IT IS JUST AN EXAMPLE OF HOW YOUR
// game/ecs_defs.h SHOULD LOOK LIKE
//
// Note that the backslash \ after each line below is very important
// when using #define, if you remove it them put all at the same
// line of #define
//

#pragma once

// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	_TRANSFORM, \
	_RENDERER, \
	_ONBORDERS, \
	_PLAYERCTRL, \
	_COLLIDER, \
	_SIMPLEMOVE, \
	_PHYSICS, \
	_ATTRIBUTES, \
	_FROGJUMP, \
	_FROGBIGJUMP, \
	_FROGTONGUE, \
	_FLY_HP, \
	_FLY_MOVEMENT, \
	_GRAVITY, \
	_ATTACK,\
	_FRAMEDIMAGE,\
	_IMAGE,\
	_WAVEMOVEMENT, \
	_PLAYERUI, \
	_BOSS_ATTRIBUTES, \
	_BOSS_HP_BAR, \
	_BOSS_ATTACK_MANAGER, \
	_FOLLOWPLAYER, \
	_ROOTMOVEMENT, \
	_ROOTWAVE, \
	_ROOTSPAWNER, \
	_ROOTAUTOAIM, \
	_TREEMOVEMENT, \
	_MELEEATTACK, \
	_NPC_CTRL, \
	_DIALOG_BOX_MNGR, \
	_HANDS_MANAGER, \
	_LANTERNSPAWNER, \
	_LANTERNMOVEMENT, \
	_LANTERN_COLLISIONS, \
	_PUNIETAZO, \
	_CLAP_ATTACK, \
	_HAMMER_ARM, \
	_VFX, \
	_WAVE_SPAWNER, \
	_TUTORIAL_FLY, \
	_TUTORIAL_ROOTSPAWNER, \
	_BUTTON_ATTRIBUTES, \
	_SHOWATOPPOSIDESIDE, \
	_CAMERA,\
	_PARALLAX



// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	_BACKGROUND_GRP, \
	_HUB_DECORATION_GRP, \
	_BOSS_GRP,\
	_BOSSELEMENTS_GRP

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    _FROGBOSS, \
	_PLAYER, \
	_FLY, \
	_LANTERN, \
	_TREEBOSS, \
	_WAVE_GENERATOR, \
	_hdlr_CAMERA


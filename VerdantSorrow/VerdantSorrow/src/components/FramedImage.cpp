#include "FramedImage.h"
#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"


#include "Transform.h"


FramedImage::FramedImage(Texture* tex, int row, int column,float time, int numframes_=0, std::string anim = 0) : frametime(time), tr_(), tex_(tex), row_(row), column_(column),flipX_(false),numframes(numframes_), currentAnim(anim),repeat_(false), repeating_(false)
{
	m_clip.w = tex_->width() / column;
	m_clip.h = tex_->height() / row;
	initime = sdlutils().currRealTime();
}

FramedImage::~FramedImage()
{
}

void FramedImage::select_sprite(int x, int y)
{
	m_clip.x = x * m_clip.w;
	m_clip.y = y * m_clip.h;
}

void FramedImage::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void FramedImage::render()
{
	if (!repeating_)
		select_sprite(i, j);

	if (!repeating_ && sdlutils().currRealTime() - initime >= frametime/numframes) {

		select_sprite(i, j);

		currentnumframes++;
		if (i < column_ - 1) { 
			i++; 	
		}
		else {
			i = 0;
			j++;
		}
		if ((currentnumframes >= numframes - 1)) { 
			j = 0; i = 0; currentnumframes = 0;
		    if (repeat_) {
			  repeating_ = true;
			}

			//repeating_ = false;
		}

		initime = sdlutils().currRealTime();
	}
	SDL_RendererFlip flip= SDL_FLIP_NONE;
	//flip the sprite
	if (flipX_)
		flip = SDL_FLIP_HORIZONTAL;


	//SDL_Rect dest;
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getWidth(),
		tr_->getHeight());

	if (currentAnim == "Chica_Idle" || currentAnim == "Chica_Jump" || currentAnim == "Chica_Run") {
		float multiplier = 1.3f;
		//float offset
		float xOffset = -10;
		float yOffset = -20;
		dest = build_sdlrect(tr_->getPos(), tr_->getWidth() * multiplier,
			tr_->getHeight() * multiplier);
		dest.x += xOffset;
		dest.y += yOffset;
	}

	assert(tex_ != nullptr);
	tex_->render(m_clip, dest, tr_->getRot(),nullptr,flip);
}

void FramedImage::flipX(bool s)
{
	flipX_ = s;
}

void FramedImage::repeat(bool h)
{
	repeat_ = h;
	repeating_ = false;
}

void FramedImage::changeanim(Texture* tex, int row, int column, float time, int numframes_, std::string newAnim)
{
	frametime = time;
	tex_ = tex;
	row_ = row;
	column_ = column;
	currentnumframes = 0;
	numframes = numframes_;
	i = j = 0;
	initime = sdlutils().currRealTime();
	m_clip.w = tex_->width() / column;
	m_clip.h = tex_->height() / row;

	currentAnim = newAnim;
}

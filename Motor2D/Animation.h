#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"

#define MAX_FRAMES 25

struct AnimationFrame {
	SDL_Rect rect;
	iPoint pivot;
};

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	AnimationFrame frames[MAX_FRAMES];
	float current_frame = 0.0f;

private:
	int last_frame = 0;
	int loops = 0;

	pugi::xml_document	sprites_file;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect, const iPoint& pivot = { 0, 0 })
	{
		frames[last_frame++] = { rect, pivot };
	}

	AnimationFrame& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
		if (speed == 0.0f)
			speed = 0.15f;
	}

	int getFrameIndex() const
	{
		return (int)current_frame;
	}

public:

	void Animation::LoadSprites(p2SString type) {


		pugi::xml_parse_result result = sprites_file.load_file("annedroid_sprites.xml");
		if (result != NULL)
		{
			pugi::xml_node player = sprites_file.child("sprites").child("player");
			speed = player.attribute("anim_speed").as_float();

			pugi::xml_node type_ = player.child(type.GetString());
			loop = type_.attribute("loop").as_bool();
			
			for (pugi::xml_node sprite = type_.child("sprite"); sprite; sprite = sprite.next_sibling("sprite"))
			{
				PushBack({sprite.attribute("x").as_int(), sprite.attribute("y").as_int(), sprite.attribute("w").as_int(), sprite.attribute("h").as_int()});
			}
		}
	}

};

#endif
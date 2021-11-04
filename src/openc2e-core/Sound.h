#pragma once

#include "openc2e-audiobackend/AudioState.h"

class Sound {
  public:
	Sound();
	operator bool();
	void fadeOut();
	void stop();
	void setPosition(float x, float y, float width, float height);
	AudioState getState();

  private:
	friend class SoundManager;
	int index = -1;
	int generation = -1;
};
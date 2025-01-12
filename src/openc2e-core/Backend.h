/*
 *  Backend.h
 *  openc2e
 *
 *  Created by Alyssa Milburn on Sat Dec 2 2006.
 *  Copyright (c) 2006 Alyssa Milburn. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#ifndef _BACKEND_H
#define _BACKEND_H

#include "BackendEvent.h"
#include "Texture.h"
#include "common/Image.h"
#include "common/span.h"

#include <memory>
#include <string>

using std::shared_ptr;

class creaturesImage;

struct RenderOptions {
	uint8_t alpha = 255;
	bool mirror = false;
	float scale = 1.0;
	bool override_drawsize = false;
	int overridden_drawwidth;
	int overridden_drawheight;
};

class RenderTarget {
  public:
	virtual void renderCreaturesImage(creaturesImage& tex, unsigned int frame, int x, int y, RenderOptions options = {}) = 0;
	void renderCreaturesImage(const std::shared_ptr<creaturesImage>& tex, unsigned int frame, int x, int y, RenderOptions options = {}) {
		assert(tex.get() != nullptr);
		renderCreaturesImage(*tex.get(), frame, x, y, options);
	}
	virtual void renderLine(int x1, int y1, int x2, int y2, unsigned int colour) = 0;
	virtual void blitRenderTarget(RenderTarget* src, int x, int y, int w, int h) = 0;
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual void renderClear() = 0;
	virtual void renderDone() = 0;
	virtual ~RenderTarget() {}
};

class Backend {
  public:
	virtual void init() = 0;
	virtual void shutdown() = 0;

	virtual unsigned int ticks() = 0;
	virtual bool pollEvent(BackendEvent& e) = 0;
	virtual bool keyDown(int key) = 0;

	virtual void resize(unsigned int width, unsigned int height) = 0;

	virtual RenderTarget* getMainRenderTarget() = 0;
	virtual RenderTarget* newRenderTarget(unsigned int width, unsigned int height) = 0;
	virtual void freeRenderTarget(RenderTarget* surf) = 0;

	virtual void setDefaultPalette(span<Color> palette) = 0;
	virtual Texture createTexture(const Image& image) = 0;
	virtual Texture createTextureWithTransparentColor(const Image& image, Color transparent) = 0;

	virtual int run() = 0;
	virtual void delay(int msec) = 0;
	virtual ~Backend() {}
};

#endif

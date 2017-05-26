
#pragma warning (disable : 4996)
#pragma once
#include <stdio.h>
#include <assert.h>
#include "Colors.h"
#include "Graphics.h"
#include <d3d9.h>

	static struct BitmapFileHeader {
		unsigned int fileSize;
		unsigned short reserve1, reserve2;
		unsigned int offset;
	};

static	struct BitmapInfoHeader {
		unsigned int headerSize;
		unsigned int width;
		unsigned int height;
		unsigned short planes;
		unsigned short bits;
		unsigned int compression;
		unsigned int imageSize;
		int xResolution, yResolution;
		unsigned int nColors;
		unsigned int importantcolors;
	};
static	struct Pixel24 {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	BitmapFileHeader file_header;
	BitmapInfoHeader info_header;
	Pixel24 pixel;

	void LoadBmp(const char * fileName, D3DCOLOR* surface)
	{
		FILE* bmpFile = fopen(fileName, "rb");
		char signature[2];
		fread(signature, sizeof(char), 2, bmpFile);
		assert(signature[0] == 'B' && signature[1] == 'M');
		fread(&file_header, sizeof(file_header), 1, bmpFile);
		fread(&info_header, sizeof(info_header), 1, bmpFile);
		fseek(bmpFile, file_header.offset, SEEK_SET);
		for (int y = (info_header.height - 1); y >= 0; y--)
		{
			for (int x = 0; x < info_header.width; x++)
			{
				fread(&pixel, sizeof(pixel), 1, bmpFile);
				surface[x + y*info_header.width] = D3DCOLOR_XRGB(pixel.b, pixel.g, pixel.r);
			};
		};

		fclose(bmpFile);

	};
	void Load(const char * fName, int width, int height, D3DCOLOR* surface, Graphics::Bmp* bmp, D3DCOLOR trans)
	{
		LoadBmp(fName, surface);
		bmp->width = width;
		bmp->height = height;
		bmp->surface = surface;
		bmp->trans = trans;
	};
	void LoadAnimation(Graphics::AnimationTemplate* temp, Graphics::Bmp* bmp, D3DCOLOR* surfaces,const char* basename, int width, int height, int nFrames, int frameDuration, D3DCOLOR trans) {
		for (int i = 0; i < nFrames; i++) {
			char base[64];
			sprintf(base, "%s%.2d.bmp", basename, i);
			Load(base, width, height, &surfaces[width * height * i], &bmp[i],trans);
		}
		temp->frames = bmp;
		temp->nFrames = nFrames;
		temp->frameDuration = frameDuration;
	};

	void Animating(Graphics::AnimationTemplate* temp, Graphics::Animation* instance, int x, int y){
		instance->temp = temp;
		instance->currentFrameExposure = 0;
		instance->currentFrame = 0;
		instance->xpos = x;
		instance->ypos = y;
	};
	void UpdateAnimation(Graphics::Animation* instance) {
		instance->currentFrameExposure++;
		if (instance->currentFrameExposure >= instance->temp->frameDuration) {
			instance->currentFrame++;
			instance->currentFrame %= instance->temp->nFrames;
			instance->currentFrameExposure = 0;
		}
	};

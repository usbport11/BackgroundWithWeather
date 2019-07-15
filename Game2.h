#ifndef game2H
#define game2H

#include "Weather.h"
#include "Background.h"

class MGame2
{
private:
	//pointers to width and height of main window
	int* pWindowWidth;
	int* pWindowHeight;
	
	//statuses
	bool Pause;
	bool Initialized;
	
	//textures
	unsigned int txWallCnt;
	stTexture* txWall;
	float TexStep[2];
	stPoint TexPos[2];
	
	stPoint Area; //x, y coords for glTranslate function
	stPoint Scale; //x, y coords for glScale function
	MTextureLoader* TextureLoader;
	MWeather* pWeather;
	MBackground* pBackground;

	void DrawArea();
	
public:
    bool* Key; //[256]

	MGame2();
	~MGame2();
	void Start();
	void Stop();
	void DrawFrame();
	void CheckButtons();
	bool Initialize(int* Width, int* Height);
	bool GetInitialized();
	bool Close();
};

#endif

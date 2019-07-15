#include "stdafx.h"
#include "Game2.h"

MGame2::MGame2()
{
    Initialized = false;
    
    Key = new bool [256];
    memset(Key, 0, 256);
    
	SetPoint(Area, -500, -500);
	SetPoint(Scale, 1, 1);
	pWindowWidth = NULL;
	pWindowHeight = NULL;
	TextureLoader = new MTextureLoader;
	
	TexStep[0] = 0.02;
	TexStep[1] = 0.05;
	SetPoint(TexPos[0], 0, 1);
	SetPoint(TexPos[1], 0, 1);
}

MGame2::~MGame2()
{
}

bool MGame2::Initialize(int* Width, int* Height)
{
    LogFile<<"Initialize Game2 data"<<endl; 
    
    //get width & height pointers
    pWindowWidth = Width;
	pWindowHeight = Height;
	
    //load all textures
    txWall = TextureLoader->LoadTexture("textures/background_64_2_1.tga", txWallCnt);
    if(!txWall) return false;
    
    LogFile<<"Randomize rand by time"<<endl; 
    srand(time(NULL));
    pWeather = new MWeather;
    if(!pWeather->CreateWeather(1))
    {
    	LogFile<<"Error while weather creating"<<endl;
    	return false;
	}
	pBackground = new MBackground [2];
	pBackground[0].SetTexId(txWall[0].Id);
	pBackground[0].CreateByPoints(0,0,800,600);
	pBackground[0].SetStep(0.02, 0);
	pBackground[1].SetTexId(txWall[1].Id);
	pBackground[1].CreateByPoints(0,0,800,600);
	pBackground[1].SetStep(0.05, 0);
    
    //VERY IMPORTANT! (Do not use game resources before textures loads)
    Initialized = true;
    //start capture buttons. scene always drawing
    Start();
    LogFile<<"Pause is "<<Pause<<endl;
    
    return true;
}

bool MGame2::GetInitialized()
{
    return Initialized;
}

bool MGame2::Close()
{
	//stop game cycle
	Stop();
	//free resources
	LogFile<<"Free game resources: Key, ProgressBar"<<endl;
	if(Key) delete [] Key;
    TextureLoader->DeleteTexture(txWall, txWallCnt);
    if(TextureLoader) delete TextureLoader;
    if(pWeather) delete pWeather;
    if(pBackground) delete [] pBackground;
    
	return true;
}

void MGame2::Start()
{
    Pause = false;
}

void MGame2::Stop()
{
    Pause = true;
}

void MGame2::DrawArea()
{	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glPushMatrix();
	//scale by axes
	glScalef(Scale.p[0], Scale.p[1], 1);
	//translate by axes
	glTranslatef(Area.p[0], Area.p[1], 0);
	glPopMatrix();
	
	pBackground[0].Draw();
	pBackground[1].Draw();
	pWeather->MoveParticles();
	pWeather->Draw();
}

void MGame2::DrawFrame()
{
	//CheckButtons();
	DrawArea();
}

void MGame2::CheckButtons()
{
	//start-stop game
	if(Key[VK_RETURN])
	{
		Pause = !Pause;
		LogFile<<"Pause = "<<Pause<<endl;
		Key[VK_RETURN] = 0;
	}
	if(Key[VK_LEFT])
	{
		pBackground[0].Move(true);
		pBackground[1].Move(true);
	}
	if(Key[VK_RIGHT])
	{
		pBackground[0].Move(false);
		pBackground[1].Move(false);
	}
}

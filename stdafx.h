#ifndef stdafxH
#define stdafxH

//==========================================headers
//windows
#include <windows.h>
#include <process.h>
//opengl
#include <gl/gl.h>
#include <gl/glu.h>
//dos
#include <time.h>
#include <stdio.h>
#include <math.h>
//file stats
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//stl
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
//point, plane, level, zone
#include "Plane.h"
//texture loader
#include "TextureLoader.h"
//==========================================namespace
//using namespace std;
//==========================================types

//global log file variable - inited in stdafx.cpp
extern ofstream LogFile;

struct stFrameControl
{
    //usual time
    DWORD FrameStart; //time of start draw one frame (ms)
    DWORD FrameEnd; //time of end draw one frame (ms)
    DWORD liStartTime; //time for start count frames (ms count)
	DWORD liEndTime; //time for end count frames (ms count)
	DWORD liDiffTime; //time of draw one frame (ms count)
	int Frames;
	float FPS;
	void Initialize()
	{
        Frames = 0;
        liStartTime = GetTickCount();
		liEndTime = liStartTime;
	}
	void Calculate()
	{
        Frames ++;
		liEndTime = GetTickCount();
		liDiffTime = liEndTime - liStartTime;
		if(liDiffTime >= 1000)
		{
            FPS = (float)Frames/liDiffTime*1000;
            liStartTime = liEndTime;
            Frames = 0;
        }
	}
};

void DrawTexturedBox(stPlane Box, unsigned int TexId, stPlane TexCoord);

#endif

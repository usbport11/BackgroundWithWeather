#include "stdafx.h"
#include "Config.h" 

MConfig::MConfig()
{
	//graphic
	cpgr_WindowWidth = 800;
	cpgr_WindowHeight = 600;
	cpgr_FullScreen = false;
	cpgr_MainTimer = 14;
	cpgr_FrameTimer = 240;
	//game
	cpgm_Font = NULL;
	cpgm_ShowFPS = true;
	cpgm_Pause = false;
	cpgm_ViewBox = true;
	cpgm_UseWeather = false;
	cpgm_UseBackground = false;
	//level
	cplv_UnitSizeX = 40;
	cplv_UnitSizeY = 40;
	cplv_ZoneUnitsX = 64;
	cplv_ZoneUnitsY = 48;
	//texture
	cptx_BackwardRead = true;
	cptx_Planes = NULL;
	cptx_Backgound = NULL;
	cptx_ObjectConfig = NULL;
	//log
	cplg_File = NULL;
	cplg_System = true;
	cplg_Level = false;
	cplg_Object = false;
	cplg_Mover = false;
}

MConfig::~MConfig()
{
	cpgr_WindowWidth = 0;
	cpgr_WindowHeight = 0;
	cpgr_FullScreen = false;
	cpgr_MainTimer = 0;
	cpgr_FrameTimer = 0;
	cpgm_ShowFPS = false;
	cpgm_Pause = false;
	cpgm_ViewBox = false;
	cpgm_UseWeather = false;
	cpgm_UseBackground = false;
	cplv_UnitSizeX = 0;
	cplv_UnitSizeY = 0;
	cplv_ZoneUnitsX = 0;
	cplv_ZoneUnitsY = 0;
	cpgm_Font = NULL;
	cptx_Planes = NULL;
	cptx_Backgound = NULL;
	cptx_ObjectConfig = NULL;
	cplg_File = NULL;
}

bool MConfig::LoadConfig(const char* FileName, bool ShortPath)
{
	ifstream infile;
	infile.open("skip");
	if(infile.good())
	{
		LogFile<<"Skip configuration file"<<endl;
		infile.close();
		return true;
	}
	
	bool Result = true;
	string IniFileName = "";
	if(!FileName)
	{
		LogFile<<"Emtpy config file name given"<<endl;
		return false;
	}
	
	//get full path to file
	if(ShortPath)
	{
		char* ExeFileName;
		ExeFileName = new char [1024];
		memset(ExeFileName, 0, 1024);
		GetModuleFileName(NULL, ExeFileName, 1023);
		IniFileName = ExeFileName;
		IniFileName = IniFileName.substr(0, IniFileName.find_last_of('\\') + 1);
		IniFileName += FileName;
		if(ExeFileName) delete [] ExeFileName;
	}
	else IniFileName = FileName;
	
	infile.open(IniFileName.c_str());
	if(!infile.good())
	{
		LogFile<<"Cannot open configuration file: "<<IniFileName<<endl;
		infile.close();
		return false;
	}
	infile.close();
	
	//prepare
	cpgm_Font = new char [255];
	memset(cpgm_Font, 0, 255);
	strcat(cpgm_Font, "trebuc.ttf");
	cptx_Planes = new char [255];
	memset(cptx_Planes, 0, 255);
	strcat(cptx_Planes, "textures/wall_16.tga");
	cptx_Backgound = new char [255];
	memset(cptx_Backgound, 0, 255);
	strcat(cptx_Backgound, "textures/background_64_2_1.tga");
	cptx_ObjectConfig = new char [255];
	memset(cptx_ObjectConfig, 0, 255);
	strcat(cptx_ObjectConfig, "config.txt");
	cplg_File = new char [255];
	memset(cplg_File, 0, 255);
	strcat(cplg_File, "log.txt");
	
	//read
	cpgr_WindowWidth = GetPrivateProfileInt("Graphic", "WindowWidth", cpgr_WindowWidth, IniFileName.c_str());
	cpgr_WindowHeight = GetPrivateProfileInt("Graphic", "WindowHeight", cpgr_WindowHeight, IniFileName.c_str());
	cpgr_FullScreen = (bool)GetPrivateProfileInt("Graphic", "FullScreen", cpgr_FullScreen, IniFileName.c_str());
	cpgr_MainTimer = GetPrivateProfileInt("Graphic", "MainTimer", cpgr_MainTimer, IniFileName.c_str());
	cpgr_FrameTimer = GetPrivateProfileInt("Graphic", "FrameTimer", cpgr_FrameTimer, IniFileName.c_str());
	GetPrivateProfileString("Game", "Font", cpgm_Font, cpgm_Font, 255, IniFileName.c_str()); 
	cpgm_ShowFPS = (bool)GetPrivateProfileInt("Game", "ShowFPS", cpgm_ShowFPS, IniFileName.c_str());
	cpgm_Pause = (bool)GetPrivateProfileInt("Game", "Pause", cpgm_Pause, IniFileName.c_str());
	cpgm_ViewBox = (bool)GetPrivateProfileInt("Game", "ViewBox", cpgm_ViewBox, IniFileName.c_str());
	cpgm_UseWeather = (bool)GetPrivateProfileInt("Game", "UseWeather", cpgm_UseWeather, IniFileName.c_str());
	cpgm_UseBackground = (bool)GetPrivateProfileInt("Game", "UseBackground", cpgm_UseBackground, IniFileName.c_str());
	cplv_UnitSizeX = GetPrivateProfileInt("Level", "UnitSizeX", cplv_UnitSizeX, IniFileName.c_str());
	cplv_UnitSizeY = GetPrivateProfileInt("Level", "UnitSizeY", cplv_UnitSizeY, IniFileName.c_str());
	cplv_ZoneUnitsX = GetPrivateProfileInt("Level", "ZoneUnitsX", cplv_ZoneUnitsX, IniFileName.c_str());
	cplv_ZoneUnitsY = GetPrivateProfileInt("Level", "ZoneUnitsY", cplv_ZoneUnitsY, IniFileName.c_str());
	cptx_BackwardRead = (bool)GetPrivateProfileInt("Textures", "BackwardRead", cptx_BackwardRead, IniFileName.c_str());
	GetPrivateProfileString("Textures", "Planes", cptx_Planes, cptx_Planes, 255, IniFileName.c_str());
	GetPrivateProfileString("Textures", "Backgound", cptx_Backgound, cptx_Backgound, 255, IniFileName.c_str());
	GetPrivateProfileString("Textures", "ObjectConfig", cptx_ObjectConfig, cptx_ObjectConfig, 255, IniFileName.c_str());
	GetPrivateProfileString("Log", "File", cplg_File, cplg_File, 255, IniFileName.c_str());
	cplg_System = (bool)GetPrivateProfileInt("Log", "System", cplg_System, IniFileName.c_str());
	cplg_Level = (bool)GetPrivateProfileInt("Log", "Level", cplg_Level, IniFileName.c_str());
	cplg_Object = (bool)GetPrivateProfileInt("Log", "Object", cplg_Object, IniFileName.c_str());
	cplg_Mover = (bool)GetPrivateProfileInt("Log", "Mover", cplg_Mover, IniFileName.c_str());
	
	//logical checks
	if((cpgr_WindowWidth!=640 || cpgr_WindowHeight!=480) && 
		(cpgr_WindowWidth!=800 || cpgr_WindowHeight!=600) && 
		(cpgr_WindowWidth!=1024 || cpgr_WindowHeight!=768))
	{
		LogFile<<"Wrong resolution values: "<<cpgr_WindowWidth<<"x"<<cpgr_WindowHeight<<endl;
		Result = false;
	}
	if(cpgr_MainTimer < 14 || cpgr_MainTimer > 33)
	{
		LogFile<<"Wrong main timer value"<<endl;
		Result = false;
	}
	if(cpgr_FrameTimer < cpgr_MainTimer || cpgr_FrameTimer > 250)
	{
		LogFile<<"Wrong frame timer value"<<endl;
		Result = false;
	}
	if(cplv_UnitSizeX < 20 || cplv_UnitSizeY < 20 || cplv_UnitSizeX > 50 || cplv_UnitSizeY > 50)
	{
		LogFile<<"Wrong unit size"<<endl;
		Result = false;
	}
	if(cplv_ZoneUnitsX < 16 || cplv_ZoneUnitsY < 12 || cplv_ZoneUnitsX > 70 || cplv_ZoneUnitsY > 50)
	{
		LogFile<<"Wrong zone units count"<<endl;
		Result = false;
	}
	if(!Result) CloseConfig();
	
	return Result;
}

bool MConfig::CloseConfig()
{
	if(cpgm_Font) delete [] cpgm_Font;
	if(cptx_Planes) delete [] cptx_Planes;
	if(cptx_Backgound) delete [] cptx_Backgound;
	if(cptx_ObjectConfig) delete [] cptx_ObjectConfig;
	if(cplg_File) delete [] cplg_File;
	return true;
}

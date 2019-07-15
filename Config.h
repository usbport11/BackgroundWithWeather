#ifndef configM
#define configM

class MConfig
{
private:
	int cpgr_WindowWidth;
	int cpgr_WindowHeight;
	bool cpgr_FullScreen;
	int cpgr_MainTimer;
	int cpgr_FrameTimer;
	bool cpgm_ShowFPS;
	char* cpgm_Font;
	bool cpgm_Pause;
	bool cpgm_ViewBox;
	bool cpgm_UseWeather;
	bool cpgm_UseBackground;
	int cplv_UnitSizeX;
	int cplv_UnitSizeY;
	int cplv_ZoneUnitsX;
	int cplv_ZoneUnitsY;
	bool cptx_BackwardRead;
	char* cptx_Planes;
	char* cptx_Backgound;
	char* cptx_ObjectConfig;
	char* cplg_File;
	bool cplg_System;
	bool cplg_Level ;
	bool cplg_Object;
	bool cplg_Mover;
public:
	MConfig();
	~MConfig();
	bool LoadConfig(const char* IniFileName, bool ShortPath);
	bool CloseConfig();
};

#endif

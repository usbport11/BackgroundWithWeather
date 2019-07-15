#ifndef graphbaseH
#define graphbaseH

#include "Game2.h"

class MGraphBase
{
private:
    //main window
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HDC m_hDC;
	HGLRC m_hGLRC;
	SYSTEM_INFO sysinfo;
    bool Exit;
    int WindowWidth;
    int WindowHeight;
    int SetUpWindow(HWND hWnd);
	int InitGL();
	
public:
    //opengl data
	string OpenGLData;
    //game
    MGame2 Game2;
       
	MGraphBase();
	~MGraphBase();
	HWND GetHWND();
    void DrawFrame();
	int CreateBaseWindow(HINSTANCE hInstance, WNDPROC WndProc, char* ClassName, char* Title, int Width, int Height);
	void ResizeScene(int width, int height);
	void Run();
	int CloseGL();
};

#endif

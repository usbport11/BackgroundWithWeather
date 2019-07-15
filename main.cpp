#include "stdafx.h"
#include "GraphBase.h"

#define MAIN_TIMER 1001
#define FRAME_TIMER 1002

char sWindowClass[] = "win32app";
char sTitle[] = "Win32 2D Engine";
MGraphBase GB;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
	if (!GB.CreateBaseWindow(hInstance, WndProc, sWindowClass, sTitle, 800, 600)) return 0;
	SetTimer(GB.GetHWND(), MAIN_TIMER, 14, NULL);
	GB.Run();
	LogFile<<"Closing application"<<endl;
	LogFile.close();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			if(GB.Game2.GetInitialized()) GB.Game2.Key[wParam] = 1;
            switch(wParam)
			{
				case VK_ESCAPE: 
					SendMessage(hWnd, WM_DESTROY, 0, 0);
					break;
				case VK_F1:
                    LogFile<<"F1 button pressed. Show OpenGL data"<<endl;
                    if(strlen(GB.OpenGLData.c_str()) > 0) MessageBox(hWnd, GB.OpenGLData.c_str(), "Info", 0);
                    break;
			}
			break;
		case WM_KEYUP:
            if(GB.Game2.GetInitialized()) GB.Game2.Key[wParam] = 0; 
			break;
		case WM_SIZE:
			GB.ResizeScene(LOWORD(lParam), HIWORD(lParam));
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case WM_PAINT:
            GB.DrawFrame();
            ValidateRect(hWnd, NULL);
            return 0;
		case WM_TIMER:
            switch(wParam)
            {
                case MAIN_TIMER:
                	GB.Game2.CheckButtons();
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
            }
            break;
		case WM_CLOSE:
		case WM_DESTROY:
			KillTimer(hWnd, MAIN_TIMER);
            GB.Game2.Close();
			GB.CloseGL();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}
	return 0;
}

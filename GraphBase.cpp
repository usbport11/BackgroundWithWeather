#include "stdafx.h"
#include "GraphBase.h"

MGraphBase::MGraphBase()
{   
    //set width and height
    WindowWidth = 0;
    WindowHeight = 0;
    Exit = false;
    GetSystemInfo( &sysinfo );
}

MGraphBase::~MGraphBase()
{
}

HWND MGraphBase::GetHWND()
{
    return m_hWnd;
}

int MGraphBase::CreateBaseWindow(HINSTANCE hInstance, WNDPROC WndProc, char* ClassName, char* Title, int Width, int Height)
{
    LogFile<<"Initialize Window"<<endl;
    
    WindowWidth = Width;
    WindowHeight = Height;
    LogFile<<"Window width: "<<WindowWidth<<" height: "<<Height<<endl;
    
	if(!ClassName) return 0;
	if(!Title) return 0;
	if(!WndProc) return 0;
	if(!hInstance) return 0;

	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;
	wc.style = CS_OWNDC;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClass(&wc))
    {
		MessageBox(NULL, "Can't reg window", "Error", 0);
		return 0;
	}

	m_hInstance = hInstance;

	m_hWnd = CreateWindow(ClassName, 
		Title, 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowWidth, WindowHeight,
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	if(!m_hWnd)
	{
		MessageBox(NULL, "Can't create window", "Error", 0);
		return 0;
	}

	if(!SetUpWindow(m_hWnd)) return 0;
	//init opengl
	if(!InitGL()) return 0;
	
	//get opengl data
	OpenGLData = "Vendor = ";
	OpenGLData += (char*)glGetString(GL_VENDOR);
	OpenGLData += "\nRenderer = ";
	OpenGLData += (char*)glGetString(GL_RENDERER);
	OpenGLData += "\nVersion = ";
	OpenGLData += (char*)glGetString(GL_VERSION);
	OpenGLData += "\n";

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return 1;
}

int MGraphBase::SetUpWindow(HWND hWnd)
{
	//prepare window
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16, 
		0,
		0,
		PFD_MAIN_PLANE,	
		0,
		0, 0, 0
	};

	m_hDC = GetDC(hWnd);
	if(!m_hDC)
	{
		MessageBox(NULL, "Can't get DC", "Error", 0);
		return 0;
	}
	unsigned short int PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if(!PixelFormat)
	{
		MessageBox(NULL, "Can't chose pixelformat", "Error", 0);
		return 0;
	}
	if(!SetPixelFormat(m_hDC, PixelFormat, &pfd))
	{
		MessageBox(NULL, "Can't set pixelformat", "Error", 0);
		return 0;
	}
	DescribePixelFormat(m_hDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd); //new
	m_hGLRC = wglCreateContext(m_hDC);
	if(!m_hGLRC)
	{
		MessageBox(NULL, "Can't create context", "Error", 0);
		return 0;
	}
	if(!wglMakeCurrent(m_hDC, m_hGLRC))
	{
		MessageBox(NULL, "Can't make context current", "Error", 0);
		return 0;
	}

	RECT Screen;
	GetClientRect(m_hWnd, &Screen);

	return 1;
}

int MGraphBase::InitGL()
{
    LogFile<<"Initialize OpenGl"<<endl;
	//prepare viport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//enable textures
	glEnable(GL_TEXTURE_2D);
	//set blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//enable blend
	glEnable(GL_BLEND);
	gluOrtho2D(0, WindowWidth, 0, WindowHeight);
	glMatrixMode(GL_MODELVIEW);
	
	//prepare game2
	if(!Game2.Initialize(&WindowWidth, &WindowHeight)) return 0;

	return 1;
}

int MGraphBase::CloseGL()
{
	Exit = true;
	if(m_hGLRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLRC);
	}
	return 1;
}

void MGraphBase::ResizeScene(int width, int height)
{
    if(width == 0) width = 0;
	if(height == 0) height = 1;
	WindowWidth = width;
	WindowHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MGraphBase::DrawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//draw
	Game2.DrawFrame();
	//swap
	SwapBuffers(m_hDC);
}

void MGraphBase::Run()
{
	MSG msg;
    while(GetMessage(&msg, m_hWnd, 0, 0) && !Exit)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

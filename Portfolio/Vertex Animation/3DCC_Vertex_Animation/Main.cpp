// Main.cpp

#define WIN32_LEAN_AND_MEAN

#include "OGL/GLee.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "App.h"
#include "AppConfig.h"

int		gWindowWidth	= 800;		// How wide do you want the window?
int		gWindowHeight	= 600;		// How tall do you want the window?
bool	bFullScreen		= false;	// Fullscreen?
bool	bAnimated		= true;	// Do you want the framework to auto-refresh

//////////////////////////////////////////////////////////////////////////////
void InitContextGL(void);						
void ShutdownContextGL(void);					
void ResizeContextGL(int nWidth, int nHeight);	
void RenderFrame(void);							

//////////////////////////////////////////////////////////////////////////////
// Globals
// These may be shared across modules
HINSTANCE	ghInstance = NULL;		// Application instance
HWND		ghMainWnd = NULL;		// Main Window (window manager)
DEVMODE		gDevMode;

#ifdef BUILD_EXAMPLE
#ifdef UNICODE
    const wchar_t szGameName[] = TEXT("Vertex Morphing - EXAMPLE");
#else // UNICODE
    const char szGameName[] = TEXT("Vertex Morphing - EXAMPLE");
#endif // UNICODE
#else // BUILD_EXAMPLE
#ifdef UNICODE
    const wchar_t szGameName[] = TEXT("Vertex Morphing");
#else // UNICODE
    const char szGameName[] = TEXT("Vertex Morphing");
#endif // UNICODE
#endif // BUILD_EXAMPLE

//////////////////////////////////////////////////////////////////////////////
// Forward  and external Declarations
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//////////////////////////////////////////////////////////////////////////////
// Program Entry Point
//////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
	{
	/////////////////////////////////////////////
	// Do any program wide Initialization here
	// Change display settings 
	if(bFullScreen == true)
		{
		gDevMode.dmPelsHeight = gWindowHeight;
		gDevMode.dmPelsWidth = gWindowWidth;
		gDevMode.dmSize = sizeof(DEVMODE);
		gDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		if(ChangeDisplaySettings(&gDevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
			MessageBox(NULL, TEXT("Cannot change to selected desktop resolution."),
							  NULL, MB_OK | MB_ICONSTOP);
			return -1;
			}
		}

	/////////////////////////////////////////////
	// Create Main Window. 
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL; //(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("OpenGLWin32Window"); // Should be unique
	wcex.hIconSm		= NULL;

	if(RegisterClassEx(&wcex) == 0)
		return -1;

	// Select window styles
	UINT uiStyle,uiStyleX;
	if(bFullScreen == true)
		{
		uiStyle = WS_POPUP;
		uiStyleX = WS_EX_TOPMOST;
		}
	else
		{
		uiStyle = WS_OVERLAPPEDWINDOW;
		uiStyleX = NULL;
		}

	// Create the main 3D window
	ghMainWnd = CreateWindowEx(uiStyleX, wcex.lpszClassName, szGameName, uiStyle,
      0, 0, gWindowWidth, gWindowHeight, NULL, NULL, hInstance, NULL);

	if (!ghMainWnd)
		return -1;

	// Make sure window manager stays hidden
	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);
	//SetFocus(ghMainWnd);

	/////////////////////////////////////////////
	// Message Pump - Use the form that goes idle and waits for 
	// messages, not continually running.
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) 
		{
		if(!TranslateMessage(&msg)) 
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}
		}


	/////////////////////////////////////////////
	// Do any program wide shutdown here
	// Restore Display Settings
	if(bFullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	// Return termination code
	return (int)msg.wParam;
	}


/////////////////////////////////////////////////////////////////////
// Main window message procedure. This is the window manager for
// the application
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, 
								WPARAM wParam, LPARAM lParam)
	{
	static HDC	hDC = NULL;		// GDI Device context
	static HGLRC hRC = NULL;	// OpenGL Rendering context

	switch (message) 
		{
		// Window is created, the application is starting
		case WM_CREATE:
			{
			hDC = GetDC(hWnd);

			static PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),		// Size of struct
				1,									// Version of struct
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // 
				PFD_TYPE_RGBA,	// Always put this
				32,				// Always put this, size of the color buffer
				0,0,0,0,0,0,0,0,0,0,0,0,0, // You have no control over
				16,					       // Depth of the Z buffer
				8,				// Stencil buffer
				0,0,0,0,0,0 };	// Either obsolete, or we don't care about

			int nPF = ChoosePixelFormat(hDC, &pfd);

			SetPixelFormat(hDC, nPF, &pfd);

			// Create rendering context
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			InitContextGL();
			}
			break;

		// Do not validate, this is sent repeatedly
		case WM_PAINT:
			RenderFrame();
			SwapBuffers(hDC);
			if(!bAnimated)
				ValidateRect(hWnd, NULL);
			break;

		case WM_SIZE:
			ResizeContextGL(LOWORD(lParam), HIWORD(lParam));
			break;

		// Window is destroyed, it's the end of the application
		case WM_DESTROY:
			// Cleanup
			ShutdownContextGL();
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);
			ReleaseDC(hWnd, hDC);
			PostQuitMessage(0);
			break;

		// Window is either full screen, or not visible
		case WM_ACTIVATE:
			{
			// Only handled when full screen mode
			if(bFullScreen == true)
				{
				WINDOWPLACEMENT wndPlacement;
				wndPlacement.length = sizeof(WINDOWPLACEMENT);
				wndPlacement.flags = WPF_RESTORETOMAXIMIZED;
				wndPlacement.ptMaxPosition.x = 0;
				wndPlacement.ptMaxPosition.y = 0;
				wndPlacement.ptMinPosition.x = 0;
				wndPlacement.ptMinPosition.y = 0;
				wndPlacement.rcNormalPosition.bottom = gDevMode.dmPelsHeight;
				wndPlacement.rcNormalPosition.left = 0;
				wndPlacement.rcNormalPosition.top = 0;
				wndPlacement.rcNormalPosition.right = gDevMode.dmPelsWidth;

				if(LOWORD(wParam) == WA_INACTIVE)
					{
					wndPlacement.showCmd = SW_SHOWMINNOACTIVE;
					SetWindowPlacement(hWnd, &wndPlacement);
					ShowCursor(TRUE);
					}
				else
					{
					wndPlacement.showCmd = SW_RESTORE;
					SetWindowPlacement(hWnd, &wndPlacement);
					ShowCursor(FALSE);
					}
				}
			}
			break;

	
		// Handle keyboard input
		case WM_CHAR:
			// Close program on ESC key press
			if(wParam == 27)
				DestroyWindow(hWnd);
			break;

		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	return 0;
	}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void InitContextGL(void)
	{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	App::GetInstance().Initialize();
	}


void ShutdownContextGL(void)
	{
		App::GetInstance().Shutdown();
	}	

void ResizeContextGL(int nWidth, int nHeight)
	{
		glViewport(0, 0, nWidth, nHeight);
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 45.0f, (float)nWidth/(float)nHeight, 0.1f, 300.0f );
		glMatrixMode( GL_MODELVIEW );
        App::GetInstance().GetCamera( ).SetProjectionPerspective( 45.0f, nWidth, nHeight, 0.1f, 300.0f );
	}


void RenderFrame(void)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	App::GetInstance().Update();
	}
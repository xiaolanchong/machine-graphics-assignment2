// Borland C++ - (C) Copyright 1991, 1992 by Borland International
//
//  Windows HELLO WORLD - C++ Version 

#define  STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _MSC_VER
#define _export 

void MoveTo(HDC hdc, int x, int y)
{
	MoveToEx( hdc, x, y, NULL);
}

void SetPointer(HWND hwnd, void* ptrToUserData)
{
	SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<ULONG_PTR>(ptrToUserData) );
}

void* GetPointer(HWND hwnd)
{
	return reinterpret_cast<void*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
}

#endif


LRESULT CALLBACK _export WndProc( HWND hWnd, UINT iMessage,
                                 WPARAM wParam, LPARAM lParam );
	 const int 	Xc = 320,
			Yc = 240,//koordinate centr

			a = 150,
			b = 100,//radiusi ellipsa x y

			n = 19,//amount koordinats
			nStartCorp 		= 0,
			nEndCorp 		= 6,
			nStartWindow 	= 7,
			nEndWindow		= 10,
			nStartLeftWing	= 11,
			nEndLeftWing	= 14,
			nStartRightWing	= 15,
            nEndRightWing	= 18,
			masch = 12;//maschtab

	const double pi = 3.14159;

	typedef double mas_kor[n];
	const mas_kor 	X = {-4,5,6,4,-2.5,-3,-4.5,	 -2,0,2,4, 	0,-0.5, 1,2.5, 	  0,-0.5,1,2.5},
					Y = {0, 0,1,2,   2, 4, 4,  	  1,1,1,1, 	0, -4,-4,  0,     2,   6,6,2};
class Main
{
public:
	static HINSTANCE hInstance;
	static HINSTANCE hPrevInstance;
	static int nCmdShow;
	static int MessageLoop( void );
};

class Window
{
protected:
    HWND hWnd;
public:
    // Provide (read) access to the window's handle in case it is needed
    // elsewhere.
    HWND GetHandle( void ) { return hWnd; }

    BOOL Show( int nCmdShow ) { return ShowWindow( hWnd, nCmdShow ); }
    void Update( void ) { UpdateWindow( hWnd ); }
    // Pure virtual function makes Window an abstract class.
    virtual long WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam ) = 0;
};

class MainWindow : public Window
{
private:
	
    static char szClassName[14];
    // Helper function used by Paint function; it is used as a
    // callback function by LineDDA.
    static void FAR PASCAL LineFunc( int X, int Y, LPSTR lpData );
public:
    // Register the class only AFTER WinMain assigns appropriate
    // values to static members of Main and only if no previous
    // instances of the program exist (a previous instance would
    // have already performed the registration).
	
    static void Register( void )
    {
        WNDCLASS wndclass;   // Structure used to register Windows class.

        wndclass.style         = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc   = ::WndProc;
        wndclass.cbClsExtra    = 0;
        // Reserve extra bytes for each instance of the window;
        // we will use these bytes to store a pointer to the C++
        // (MainWindow) object corresponding to the window.
        // the size of a 'this' pointer depends on the memory model.
        wndclass.cbWndExtra    = sizeof( MainWindow * );
        wndclass.hInstance     = Main::hInstance;
        wndclass.hIcon         = LoadIcon( Main::hInstance, "laba2" );
        wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
        wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
        wndclass.lpszMenuName  = NULL;
        wndclass.lpszClassName = szClassName;

        if ( ! RegisterClass( &wndclass ) )
            exit( FALSE );
    }

    // Do not create unless previously registered.
    MainWindow( void )
    {
        // Pass 'this' pointer in lpParam of CreateWindow().
        hWnd = CreateWindow( szClassName,
            szClassName,
            WS_OVERLAPPEDWINDOW,
            50,//CW_USEDEFAULT,
            50,
            640,
            480,
            NULL,
            NULL,
            Main::hInstance,
            (LPSTR) this );
        if ( ! hWnd )
            exit( FALSE );

        Show( Main::nCmdShow );
        Update();
    }
    long WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam );  

    // Print a message in the client rectangle.
	void Paint( void );
    void DRAW(HDC, int);
	 // Struct used by Paint to pass information to callback function
    // used by LineDDA
    struct LINEFUNCDATA
    {
        HDC hDC;
        char FAR *LineMessage;
        int MessageLength;
        LINEFUNCDATA( char *Message )
        {
            hDC = 0;
            MessageLength = strlen( Message );
            LineMessage = new far char [MessageLength+1];
            lstrcpy( LineMessage, Message );
        };
        ~LINEFUNCDATA( void ) { delete LineMessage; }
    };
};

HINSTANCE Main::hInstance = 0;
HINSTANCE Main::hPrevInstance = 0;
int Main::nCmdShow = 0;

int Main::MessageLoop( void )
{
    MSG msg;

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return msg.wParam;
}

char MainWindow::szClassName[] = "Laba2";

void MainWindow::DRAW(HDC hdc, int alpha)
{
	int i;
	int koef;
	mas_kor X1,Y1;
	int point0[2];
	double x = a*cos(alpha*pi/180);
	double y = -b*sin(alpha*pi/180);
	point0[0] = Xc + x;
	point0[1] = Yc + y;

	int ost = (alpha%360)*180;

	if(ost >= 180)
		koef = 1;
	else
		koef = -1;

	double k;
	if (ost == 0)
		k = pi/2;
	else
	{
		if (ost == 90)
			k = -3*pi/4;
		else
		{
			if(koef == 1)
				k = pi/2 - (pi/2 - atan(koef*b*x/(a*a*sqrt(1-x*x/a/a))));
			else
				k = -pi/2 - (pi/2 - atan(koef*b*x/(a*a*sqrt(1-x*x/a/a))));
		}
	}

	X1[0] = X[0]*masch*cos(k) + Y[0]*masch*sin(k) + point0[0];
	Y1[0] = -(Y[0]*masch*cos(k) - X[0]*masch*sin(k)) + point0[1];
	for(i = 0;i < n-1;i++)
	{
		X1[i+1] = X[i+1]*masch*cos(k) + Y[i+1]*masch*sin(k) + point0[0];
		Y1[i+1] = -(Y[i+1]*masch*cos(k) - X[i+1]*masch*sin(k)) + point0[1];
		if(i < nEndCorp)
        {
			MoveTo( hdc, X1[i],Y1[i]);
			LineTo( hdc, X1[i+1],Y1[i+1]);
        }
		if((i >= nStartWindow)&&(i <= nEndWindow))
        {
			Ellipse( hdc, X1[i]-0.5*masch, Y1[i]-0.5*masch,	X1[i]+0.5*masch, Y1[i]+0.5*masch);
        }
		if((i >= nStartLeftWing)&&(i < nEndLeftWing))
        {
			MoveTo( hdc, X1[i],Y1[i]);
			LineTo( hdc, X1[i+1],Y1[i+1]);
        }
		if((i >= nStartRightWing)&&(i < nEndRightWing))
        {
			MoveTo( hdc, X1[i],Y1[i]);
			LineTo( hdc, X1[i+1],Y1[i+1]);
        }
	}
	MoveTo( hdc, X1[6],Y1[6]);
	LineTo( hdc, X1[0],Y1[0]);

}
//extern void _far _pascal Sleep(DWORD);
void MainWindow::Paint( void )
{
    PAINTSTRUCT ps;
	HDC hdc;
    hdc = BeginPaint( hWnd, &ps );
    //GetClientRect( hWnd, (LPRECT) &rect );
	
	HBRUSH hbrush;
	HPEN hpen,hpen_2;

	hbrush = CreateSolidBrush(RGB(255,255, 255));
	SelectBrush(hdc, hbrush);
	       
	hpen = CreatePen( PS_SOLID, 1, RGB(255,0,0));
    COLORREF color = GetBkColor(hdc);
	hpen_2 = CreatePen( PS_SOLID, 1, color);

	int beta = 450;
	int count = GetTickCount();
  //  ValidateRect(GetHandle(), NULL);
	while (beta != 0)
	{
    	/*count = GetTickCount() - count;
		if (count > 200)
        {                */
			SelectPen(hdc, hpen);
			DRAW(hdc, beta);
		  //	Sleep(200);
            long int a = 0;
            while(a++ != 10000000/5);
	   	  	SelectPen(hdc, hpen_2);
		  	DRAW(hdc, beta);
			beta -= 2;
			count = GetTickCount();
       //}
	}            
					  	
	EndPaint( hWnd, &ps );
}

void FAR PASCAL _export MainWindow::LineFunc( int X, int Y, LPSTR lpData )
{
    LINEFUNCDATA FAR * lpLineFuncData = (LINEFUNCDATA FAR *) lpData;
    TextOut( lpLineFuncData->hDC, X, Y,
             lpLineFuncData->LineMessage, lpLineFuncData->MessageLength );
}

long MainWindow::WndProc( UINT iMessage, WPARAM wParam, LPARAM lParam )
{
    switch (iMessage)
    {
        case WM_CREATE:
	    break;
        case WM_PAINT:
            Paint();
            break;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc( hWnd, iMessage, wParam, lParam );
    }
    return 0;
}

// If data pointers are near pointers
#if defined(__SMALL__) || defined(__MEDIUM__)
inline Window *GetPointer( HWND hWnd )
{
    return (Window *) GetWindowWord( hWnd, 0 );
}
inline void SetPointer( HWND hWnd, Window *pWindow )
{
    SetWindowWord( hWnd, 0, (WORD) pWindow );
}

// else pointers are far
#elif defined(__LARGE__) || defined(__COMPACT__)
inline Window *GetPointer( HWND hWnd )
{
    return (Window *) GetWindowLong( hWnd, 0 );
}
inline void SetPointer( HWND hWnd, Window *pWindow )
{
    SetWindowLong( hWnd, 0, (LONG) pWindow );
}

#elif defined(_MSC_VER)
  // nothing, vc compiler
#else
    #error Choose another memory model!
#endif

LRESULT  CALLBACK _export WndProc( HWND hWnd, UINT iMessage, WPARAM wParam,
                                 LPARAM lParam )
{
    // Pointer to the (C++ object that is the) window.
    Window *pWindow = reinterpret_cast<Window*>(GetPointer( hWnd ));

    // The pointer pWindow will have an invalid value if the WM_CREATE
    // message has not yet been processed (we respond to the WM_CREATE
    // message by setting the extra bytes to be a pointer to the
    // (C++) object corresponding to the Window identified
    // by hWnd).  The messages that
    // precede WM_CREATE must be processed without using pWindow so we
    // pass them to DefWindowProc.
    // How do we know in general if the pointer pWindow is invalid?
    // Simple: Windows allocates the window extra bytes using LocalAlloc
    // which zero initializes memory; thus, pWindow will have a value of
    // zero before we set the window extra bytes to the 'this' pointer.
    // Caveat emptor: the fact that LocalAlloc will zero initialize the
    // window extra bytes is not documented; therefore, it could change
    // in the future.

    if ( pWindow == 0 )
    {
        if ( iMessage == WM_CREATE )
        {
            LPCREATESTRUCT lpcs;

            lpcs = (LPCREATESTRUCT) lParam;
            pWindow = (Window *) lpcs->lpCreateParams;

            // Store a pointer to this object in the window's extra bytes;
            // this will enable us to access this object (and its member
            // functions) in WndProc where we are
            // given only a handle to identify the window.
            SetPointer( hWnd, pWindow );
            // Now let the object perform whatever
            // initialization it needs for WM_CREATE in its own
            // WndProc.
	    return pWindow->WndProc( iMessage, wParam, lParam );
        }
        else
            return DefWindowProc( hWnd, iMessage, wParam, lParam );
    }
    else
        return pWindow->WndProc( iMessage, wParam, lParam );
}

// Turn off warning: Parameter 'lpszCmdLine' is never used in function WinMain(unsigned int,unsigned int,char far*,int)
#pragma argsused

// Turn off warning: 'MainWnd' is assigned a value that is never used in function WinMain(unsigned int,unsigned int,char far*,int)
#pragma option -w-aus

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine,
                    int nCmdShow )
{
    Main::hInstance = hInstance;
    Main::hPrevInstance = hPrevInstance;
    Main::nCmdShow = nCmdShow;

    // A Windows class should be registered with Windows before any windows
    // of that type are created.
    // Register here all Windows classes that will be used in the program.
    // Windows classes should not be registered if an instance of
    // the program is already running.
    if ( ! Main::hPrevInstance ) {
        MainWindow::Register();
    }

    MainWindow MainWnd;

    return Main::MessageLoop();
}

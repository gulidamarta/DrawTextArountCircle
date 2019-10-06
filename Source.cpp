#include <Windows.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

#define CIRCLE_RADIUS 100
#define ANGEL_STEP 10
#define FONT_SIZE 20
#define MAX_LETTERS_AMOUNT 40

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawImage(HDC, HWND);
void SaveKey(WPARAM);

INT windowHight = 0;
INT windowWidth = 0;

static int cch = 0;        // characters in buffer
static int amountLetters = 0;	// current number of the entered letters
static wchar_t *tempText = (wchar_t *)malloc(sizeof(wchar_t) * 2);
static float currentAngel = 0;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Initialize gdiplus functionality
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "Lab2";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);

	hWnd = CreateWindow("Lab2", "Lab2_OSiSP",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}

void DrawImage(HDC hdc, HWND hWnd) {
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Pen      pen(Gdiplus::Color(255, 0, 0, 255));

	Gdiplus::Font font(&Gdiplus::FontFamily(L"Consolas"), FONT_SIZE);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 255));

	RECT rect;
	if (GetClientRect(hWnd, &rect))
	{
		windowWidth = rect.right - rect.left;
		windowHight = rect.bottom - rect.top;
	}

	INT centerRectX = (windowWidth / 2) - CIRCLE_RADIUS;
	INT centerRectY = (windowHight / 2) - CIRCLE_RADIUS;

	graphics.DrawEllipse(&pen, centerRectX, centerRectY, CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2);

	if((cch != 0) && (amountLetters < MAX_LETTERS_AMOUNT)){
		graphics.TranslateTransform(centerRectX + CIRCLE_RADIUS, 
			centerRectY + CIRCLE_RADIUS);
		graphics.RotateTransform(currentAngel);
		graphics.TranslateTransform(-(centerRectX + CIRCLE_RADIUS),
			-(centerRectY + CIRCLE_RADIUS));
		graphics.DrawString(tempText, -1, &font, 
			Gdiplus::PointF(centerRectX + CIRCLE_RADIUS - 5, centerRectY - FONT_SIZE - 5), &brush);
		currentAngel += 9;
		cch--;
		amountLetters++;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CHAR:
		SaveKey(wParam);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawImage(hdc, hWnd);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO:
		MINMAXINFO FAR *lpMinMaxInfo;
		lpMinMaxInfo = (MINMAXINFO FAR *) lParam;
		lpMinMaxInfo->ptMinTrackSize.x = 250;
		lpMinMaxInfo->ptMinTrackSize.y = 340;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SaveKey(WPARAM wParam) {
    // current character
	wchar_t ch;
	ch = (wchar_t)wParam;
	cch++;
	tempText[0] = ch;
	tempText[1] = '\0';
}









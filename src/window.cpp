#include <window.hpp>
using namespace sglf;

#include "input.hpp"

bool Window::forceClose;
void (*Window::resizedCallback)(unsigned int width, unsigned int height);

WNDCLASSEXA Window::windowClass;
HWND Window::hwnd;
MSG Window::msg;
LONG_PTR Window::savedStyle;
RECT Window::savedRect;

unsigned int Window::width;
unsigned int Window::height;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_SIZE:
		{
			static RECT rc;

			GetClientRect(Window::hwnd, &rc);

			Window::width  = rc.right - rc.left;
			Window::height = rc.bottom - rc.top;

			if (Window::resizedCallback)
				Window::resizedCallback(Window::width, Window::height);

			return 0;
		}

		case WM_KEYDOWN:
		{
			Input::keyDown(wParam);
			return 0;
		}

		case WM_KEYUP:
		{
			Input::keyUp(wParam);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			Input::mousePos.x = LOWORD(lParam);
			Input::mousePos.y = HIWORD(lParam);
			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::initialize(int width, int height, const char *title)
{
	Window::forceClose = false;
	Window::resizedCallback = nullptr;
	Window::width = width;
	Window::height = height;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEXA));
	windowClass.cbSize        = sizeof(WNDCLASSEXA);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance     = nullptr;
	windowClass.lpfnWndProc   = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	windowClass.style         = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExA(&windowClass);
	RECT rect = { 0, 0, (LONG)Window::width, (LONG)Window::height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	// Calculate window position to center it on the screen
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (screenWidth - (rect.right - rect.left)) / 2;
	int posY = (screenHeight - (rect.bottom - rect.top)) / 2;

	Window::hwnd = CreateWindowExA(
		WS_EX_OVERLAPPEDWINDOW, windowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		posX, posY, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, nullptr, nullptr
	);
	
	ShowWindow(Window::hwnd, SW_NORMAL);
	SetForegroundWindow(Window::hwnd);
	SetActiveWindow(Window::hwnd);
	SetFocus(Window::hwnd);

	savedStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
	GetWindowRect(hwnd, &savedRect);

	ZeroMemory(&Window::msg, sizeof(MSG));
	Window::msg.message = WM_NULL;
}

void Window::finalize()
{
	UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
	DestroyWindow(Window::hwnd);
}

bool Window::shouldClose()
{
	if (Window::forceClose)
		return true;

	while (PeekMessage(&Window::msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (Window::msg.message == WM_QUIT)
			return true;

		TranslateMessage(&Window::msg);
		DispatchMessage(&Window::msg);
	}

	return false;
}

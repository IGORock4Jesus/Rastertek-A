#include <Windows.h>



LPCTSTR wndClassName = "firstwindow";
HWND hwnd;
const int Width = 800;
const int Height = 600;



bool InitializeWindow(HINSTANCE hinstance, int show, int width, int height, bool windowed);
int messageLoop();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int show)
{
	if (!InitializeWindow(hinstance, show, Width, Height, true))
	{
		MessageBox(0, "Window Initialization - failed", 0, 0);
		return 0;
	}

	messageLoop();

	return 0;
}

bool InitializeWindow(HINSTANCE hinstance, int show, int width, int height, bool windowed)
{
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hinstance;
	wc.hIcon = wc.hIconSm = LoadIcon(0, IDI_WINLOGO);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszClassName = wndClassName;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, "Error registering class", 0, 0);
		return false;
	}

	hwnd = CreateWindowEx(0, wndClassName, "Window Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, HWND_DESKTOP, 0, hinstance, 0);

	if (!hwnd)
	{
		MessageBox(0, "Error creating window", 0, 0);
		return false;
	}

	ShowWindow(hwnd, show);
	UpdateWindow(hwnd);

	return true;
}

int messageLoop()
{
	MSG msg{ 0 };
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) {
			if (MessageBox(0, "Are you sure you want to exit?", 0, MB_ICONQUESTION | MB_YESNO) == IDYES)
				DestroyWindow(hwnd);
		}
		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}
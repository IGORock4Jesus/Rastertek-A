#include <Windows.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <xnamath.h>


LPCTSTR wndClassName = "firstwindow";
HWND hwnd;
const int Width = 800;
const int Height = 600;
IDXGISwapChain* swapChain;
ID3D11Device* device;
ID3D11DeviceContext* context;
ID3D11RenderTargetView* renderTargetView;
float red, green, blue;
int colormodr = 1, colormodg = 1, colormodb = 1;


bool InitializeWindow(HINSTANCE hinstance, int show, int width, int height, bool windowed);
int messageLoop();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
bool InitializeDirect3D11App(HINSTANCE hinstance);
void ReleaseObjects();
bool InitScene();
void UpdateScene();
void DrawScene();




int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int show)
{
	if (!InitializeWindow(hinstance, show, Width, Height, true))
	{
		MessageBox(0, "Window Initialization - failed", 0, 0);
		return 0;
	}

	if (!InitializeDirect3D11App(hinstance)) {
		MessageBox(0, "Direct3D initialization - failed", 0, 0);
		return 0;
	}

	if (!InitScene()) {
		MessageBox(0, "Scene Initialization - failed", 0, 0);
		return 0;
	}

	messageLoop();

	ReleaseObjects();

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
			UpdateScene();
			DrawScene();
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


bool InitializeDirect3D11App(HINSTANCE hinstance) {
	DXGI_MODE_DESC mode{ 0 };
	mode.Width = Width;
	mode.Height = Height;
	mode.RefreshRate.Numerator = 60;
	mode.RefreshRate.Denominator = 1;
	mode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC scd{ 0 };
	scd.BufferCount = 1;
	scd.BufferDesc = mode;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = true;

	auto result = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, 0, &context);

	ID3D11Texture2D* backBuffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	result = device->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();

	context->OMSetRenderTargets(0, &renderTargetView, 0);

	return true;
}

void ReleaseObjects() {
	context->Release();
	device->Release();
	swapChain->Release();
}

bool InitScene() {
	return true;
}

void UpdateScene() {
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
}

void DrawScene() {
	D3DXCOLOR backColor{ red, green, blue, 1.0f };
	context->ClearRenderTargetView(renderTargetView, backColor);

	swapChain->Present(0, 0);
}


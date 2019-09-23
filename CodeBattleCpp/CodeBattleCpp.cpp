#include <iostream>
#include <random>
#include <io.h>
#include <fcntl.h> 

#include "client.hpp"
#include "graphics.hpp"


using namespace std::literals::chrono_literals;

LRESULT WINAPI __WndInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::shared_ptr<client> gcb;
std::unique_ptr<graphics> gfx;
/*
 After completing registration you will be redirected to gamefield
 Example url: http://localhost:8888/codenjoy-contest/board/player/cg601yim3186cotnftue?code=307643814057246189&gameName=loderunner
 @url: localhost:8888
 @user_id: cg601yim3186cotnftue
 @user_code: 8887669793631271133
*/
void run_client(std::string_view url, std::string_view user_id, std::string_view user_code) {
	gcb = std::make_shared<client>(url, user_id, user_code);
	gcb->run([&]() {
			auto gb = gcb->get_game_board();
			gcb->send(gb->current_act());
	});
}

void create_console() {
	AllocConsole();

	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	FILE* COutputHandle = _fdopen(SystemOutput, "w");

	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE* CErrorHandle = _fdopen(SystemError, "w");

	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE* CInputHandle = _fdopen(SystemInput, "r");

	std::ios::sync_with_stdio(true);

	freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
	freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);
}

HWND create_window(HINSTANCE hInstance){
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,__WndInit,0,0,
		hInstance,nullptr,nullptr,nullptr,nullptr,
		L"PomoikaGUI",nullptr };
	wc.hIconSm = (HICON)LoadImage(hInstance, NULL, IMAGE_ICON, 16, 16, 0);
	wc.hIcon = (HICON)LoadImage(hInstance, NULL, IMAGE_ICON, 32, 32, 0);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	RegisterClassEx(&wc);

	RECT wr;
	wr.left = 10;
	wr.right = 1000 + wr.left;
	wr.top = 10;
	wr.bottom = 1000 + wr.top;
	AdjustWindowRect(&wr, WS_BORDER | WS_SYSMENU, FALSE);
	return CreateWindowW(L"PomoikaGUI", L"PomoikaGUI",
		WS_BORDER | WS_SYSMENU,
		wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, hInstance, nullptr);
}

//And govnocode begins
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {
	
	create_console();
	
	auto hWnd = create_window(hInstance);
	if (hWnd == nullptr)
		std::cout << "hwnd == nullptr\n";
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	{
		run_client("", "", "");
		gfx = std::make_unique<graphics>(hWnd, gcb);
		
		while (true)
		{
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					return 0;
				std::this_thread::sleep_for(1ms);
			}
		}
	}
	return 0;
}

/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: main.cpp									 */
/*	DESCRIPTION: WinMainを記述する						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/15(f::jndbcph)						 */
/*********************************************************/
#include "WindowProc.h"
#include "Define.h"

#define APP_NAME TEXT("ChirnoBlocker++")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

	//ウィンドウクラス初期化
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = static_cast<HICON>(LoadImage(NULL, TEXT("image\\ChirnoBlocker++.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wc.hCursor = static_cast<HCURSOR>(LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = static_cast<HICON>(LoadImage(NULL, TEXT("image\\ChirnoBlocker++.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	
	//ウィンドウクラス登録
	if(!RegisterClassEx(&wc)) return 0;

	//ウィンドウ生成
	if((hWnd = CreateWindow(APP_NAME, APP_NAME, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL)) == 0) return 0;

	//メッセージループ
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
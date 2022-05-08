/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: WindowProc.cpp								 */
/*	DESCRIPTION: メッセージ処理を行う					 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/15(f::jndbcph)						 */
/*********************************************************/
#include "WindowProc.h"

/*********************************************************/
/*	関数：WindowProc									 */
/*	概要：ウィンドウプロシージャ						 */
/*********************************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	DEVMODE devmode;
	PAINTSTRUCT ps;
	DWORD dwID;
	ifstream config;
	int result, i;
	char ch;
	static THREAD_PARAM param;
	static HBITMAP hBitmap;
	static HFONT hFont;

	switch(uMsg)
	{
	case WM_CREATE:

		//操作方法取得
		config.open("config.dat", ifstream::binary);
		for(i = 0; i < 2; i++)
			config >> ch;

		config.close();

		//フルスクリーン起動確認
		result = MessageBox(hWnd, TEXT("フルスクリーンで起動しますか？\nはい(フルスクリーン)\nいいえ(ウィンドウ)\nキャンセル(終了)"), TEXT("確認"), MB_YESNOCANCEL | MB_ICONQUESTION);
		//フルスクリーン起動
		switch(result)
		{
		case IDYES:
			//ディスプレイ解像度設定
			devmode.dmSize = sizeof(DEVMODE);
			devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
			devmode.dmPelsWidth = CLIENT_WIDTH;
			devmode.dmPelsHeight = CLIENT_HEIGHT;

			//失敗
			if(ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				MessageBox(hWnd, TEXT("ディスプレイの設定を変更できません"), NULL, MB_OK);
			}
			//成功
			else
			{
				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, 0);
			}
			break;
		case IDNO:
			ShowWindow(hWnd, SW_SHOW);
			break;
		case IDCANCEL:
			SendMessage(hWnd, WM_QUIT, 0, 0);
			break;
		}

		//バックバッファ生成
		hdc = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGHT);
		param.hdc = CreateCompatibleDC(hdc);
		SelectObject(param.hdc, hBitmap);
		SelectObject(param.hdc, GetStockObject(BLACK_PEN));
		SelectObject(param.hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(param.hdc, GetStockObject(SYSTEM_FONT));
		SetTextColor(param.hdc, RGB(0xFF, 0xFF, 0xFF));
		SetBkMode(param.hdc, TRANSPARENT);
		ReleaseDC(hWnd, hdc);

		//フォント作成
		hFont = CreateFont(22, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, 0);

		//スレッドパラメータ設定
		param.hWnd		= hWnd;
		param.dwFPS		= 30;
		param.control	= atoi(&ch);
		lstrcpy(param.str, TEXT("FPS計測中..."));
		//Gameクラス領域確保
		try
		{
			param.game = new Game(param.hdc, hWnd);
		} catch(bad_alloc ba)
		{
			return ERR_ALLOC;
		}

		//スレッド起動
		CreateThread(NULL, 0, ThreadFunction, (LPVOID)&param, 0, &dwID);
		return 0;
	case WM_QUIT:
		//バックバッファ解放
		DeleteDC(param.hdc);

		//GDIオブジェクト破棄
		DeleteObject(hBitmap);
		DeleteObject(hFont);

		//クラス領域解放
		delete param.game;

		//エラーメッセージ表示
		if(wParam != 0)
		{
			switch(wParam)
			{
			case ERR_ALLOC:
				MessageBox(NULL, TEXT("エラーが発生した為、ゲームを終了しました"), TEXT("Allocate Error"), MB_OK);
				break;
			case ERR_FILEOPEN:
				MessageBox(NULL, TEXT("エラーが発生した為、ゲームを終了しました"), TEXT("File open error"), MB_OK);
				break;
			case ERR_MCI:
				MessageBox(NULL, TEXT("エラーが発生した為、ゲームを終了しました"), TEXT("MCI error"), MB_OK);
			case ERR_JOY:
				MessageBox(NULL, TEXT("エラーが発生した為、ゲームを終了しました"), TEXT("Joy error"), MB_OK);
			}
		}

		SendMessage(hWnd, WM_DESTROY, 0, 0);

		return 0;
	case WM_DESTROY:
		//終了メッセージポスト
		PostQuitMessage(wParam);
	
		return 0;
	case WM_MOUSEMOVE:
		//操作方法がマウスで無ければ脱出
		if(param.control) return 0;

		//ゲーム画面なら
		if(param.game->GetDisplay() == GAME)
		{
			param.game->player->SetPoint(LOWORD(lParam) - BAR_WIDTH / 2);
			//画面端
			if(LOWORD(lParam) - (BAR_WIDTH / 2) < DX)
				param.game->player->SetPoint(DX);
			if(HIWORD(param.game->player->GetPoint()) + BAR_WIDTH > (DX + DWIDTH))
				param.game->player->SetPoint(DX + DWIDTH - BAR_WIDTH);
		}
		return 0;
	case WM_LBUTTONDOWN:
		//クリックフラグを立てる
		param.game->Clicked(LEFT_CLICKED);
		if(param.game->GetDisplay() == GAME)
			param.game->player->click_flg |= LEFT_CLICKED;
		return 0;
	case WM_RBUTTONDOWN:
		//クリックフラグを立てる
		param.game->Clicked(RIGHT_CLICKED);
		if(param.game->GetDisplay() == GAME)
			param.game->player->click_flg = RIGHT_CLICKED;
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		//エスケープキーが押されたら終了
		case VK_ESCAPE:
			SendMessage(hWnd, WM_QUIT, 0, 0);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//ゲーム画面表示
		param.game->ShowDisplay(param.hdc, hWnd);
		SetTextColor(param.hdc, RGB(0xFF, 0xFF, 0xFF));
		SelectObject(param.hdc, hFont);
		TextOut(param.hdc, DX + DWIDTH + 100, CLIENT_HEIGHT - 24, param.str, lstrlen(param.str));
		BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, param.hdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
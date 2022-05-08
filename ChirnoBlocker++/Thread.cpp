/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Thread.cpp									 */
/*	DESCRIPTION: ゲーム用スレッド						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/15(f::jndbcph)						 */
/*********************************************************/
#include "Thread.h"
#include "MCI.h"

/*********************************************************/
/*	アニメーションスレッド								 */
/*********************************************************/
DWORD WINAPI ThreadFunction(LPVOID vdParam)
{
	THREAD_PARAM *parameter;
	DWORD nowTime = 0, progress = 0, idealTime = 0, frames = 0, beforeTime;
	enum DISP now_disp;
	BOOL load_completed = FALSE;
	
	parameter = static_cast<THREAD_PARAM *>(vdParam);
	
	//時間取得
	beforeTime = timeGetTime();
	
	//ウィンドウが存在する間
	while(IsWindow(parameter->hWnd))
	{
		now_disp = parameter->game->GetDisplay();

		nowTime = timeGetTime();								//現在時間取得
		progress = nowTime - beforeTime;						//経過時間
		idealTime = static_cast<DWORD>(frames * (1000.0f / parameter->dwFPS));	//理想時間

		//音源読み込み
		if(!load_completed && now_disp == READY)
		{
			parameter->game->ball->m_reflect = new MCI(parameter->hWnd, TEXT("SE\\Reflect.wav"), TYPE_SE);
			load_completed = TRUE;
		}

		if(now_disp == GAME)
		{
			//ボールの移動
			parameter->game->ball->Move(parameter->game->player->GetRect());

			if(!parameter->game->ball->IsShot())
			{
				parameter->game->ball->SetPoint(MAKELONG((HIWORD(parameter->game->player->GetPoint()) + BAR_WIDTH / 2) - (BALL_WIDTH / 2), 516));
			}
		}

		//プレイヤーの移動
		if(now_disp == GAME && parameter->control)
			parameter->game->player->Move();

		InvalidateRect(parameter->hWnd, NULL, FALSE);

		//理想時間より経過時間の方が速ければ待つ
		if(idealTime > progress) Sleep(idealTime - progress);

		//1秒経過
		if(progress >= 1000)
		{
			wsprintf(parameter->str, TEXT("FPS=%03d"), frames);
			beforeTime = nowTime;
			frames = 0;
		}
		frames++;
	}

	return TRUE;
}
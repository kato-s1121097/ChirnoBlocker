/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Game.h										 */
/*	DESCRIPTION: ゲーム画面切り替えを行う				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/14(f::jndbcph)						 */
/*********************************************************/
#ifndef __GAME__
#define __GAME__

#include <new>
#include <fstream>
#include "Player.h"
#include "Ball.h"
#include "MCI.h"
#include "Define.h"
#include "Score.h"
#include "Timer.h"
#include "JoyStick.h"
using namespace std;

enum DISP{ TITLE, LOAD, GAME, STAGECLEAR, GAMEOVER, CONTINUE, READY, ALLCLEAR };

class Game
{
	HBITMAP hBlockBmp, hTitleBackBmp, hGameBackBmp, hLoadBackBmp, hGameOverBackBmp, hBackgroundBmp, hClearBackBmp;
	HFONT hBigFont, hNormalFont, hSmallFont;
	BYTE click_flg, load_flg;
	DWORD now_stage, time, t_Score, t_HiScore;
	JoyStick *joystick;
	Score *score;
	Timer *game_timer;
	DISP now_display;
	int control, visible;
	//画面メソッド
	void Title(HDC &hdc, HWND &hWnd);
	void Load(HDC &hdc, HWND &hWnd);
	void Ready(HDC &hdc);
	void GameMain(HDC &hdc, HWND &hWnd);
	void StageClear(HDC &hdc, HWND &hWnd);
	void GameOver(HDC &hdc);
	void Continue(HDC &hdc, HWND &hWnd);
	void AllClear(HDC &hdc);
public:
	Stage *stage;
	Ball *ball;
	Player *player;
	MCI *blockbreak_se, *miss_se, *clear_se, *title, *ending;
	Game(HDC hdc, HWND &hWnd);
	~Game();
	void Clicked(BYTE whitch);
	void SetDisplay(enum DISP);
	enum DISP GetDisplay();
	void ShowDisplay(HDC &hdc, HWND &hWnd);
};

#endif
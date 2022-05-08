/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Thread.h									 */
/*	DESCRIPTION: ゲーム用スレッド						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/01/16(f::jndbcph)						 */
/*********************************************************/
#ifndef __THREAD__
#define __THREAD__

#include "Define.h"
#include "Game.h"
#pragma comment(lib, "winmm.lib")

//アニメーションスレッド用構造体
typedef struct _THREAD_PARAM
{
	HWND hWnd;
	HDC hdc;
	Game *game;
	DWORD dwFPS;
	TCHAR str[64];
	int control;
} THREAD_PARAM;

/*********************************************************/
/*	スレッドファンクション								 */
/*********************************************************/
DWORD WINAPI ThreadFunction(LPVOID vdParam);

#endif
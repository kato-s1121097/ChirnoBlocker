/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: WindowProc.h								 */
/*	DESCRIPTION: メッセージ処理を行う					 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/01/05(f::jndbcph)						 */
/*********************************************************/
#ifndef __WINDOWPROC__
#define __WINDOWPROC__

#include <fstream>	//For_Log
#include <new>		//For_bad_allocation
#include "Game.h"
#include "Define.h"
#include "Thread.h"
using namespace std;

/*********************************************************/
/*	関数：WindowProc									 */
/*	概要：ウィンドウプロシージャ						 */
/*********************************************************/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

#endif
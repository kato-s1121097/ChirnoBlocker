/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Stage.h									 */
/*	DESCRIPTION: ステージの情報を管理する				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/05(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __STAGE__
#define __STAGE__

#include <new>
#include <fstream>
#include "Define.h"
using namespace std;

enum TYPE { CIRCLE9, BROWN, RED, YELLOW, GREEN, PURPLE, PINK, GRAY, ICE, NOTTHING };

class Stage
{
	BOOL *m_isVisible;
	ifstream stage_data;
	enum TYPE *m_type;
public:
	//コンストラクタ
	Stage(HWND &hWnd, DWORD stage);

	//デストラクタ
	~Stage(void);

	//インライン関数
	void SetBlockState(DWORD index, BOOL visible) { m_isVisible[index] = visible; }
	BOOL GetBlockState(DWORD index) { return m_isVisible[index]; }
	enum TYPE GetType(DWORD index)	{ return m_type[index]; }

	//通常メンバ
	BOOL CheckClear();
};

#endif
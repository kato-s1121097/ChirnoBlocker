/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Score.h									 */
/*	DESCRIPTION: �X�R�A�𐧌䂷��						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/11(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#ifndef __SCORE__
#define __SCORE__

#include <windows.h>
#include <fstream>
#include <cstdlib>
using namespace std;

class Score
{
	DWORD m_HiScore, m_Score, m_HiScoret, m_ScoreMax;
public:
	//�R���X�g���N�^
	Score(void);

	//�f�X�g���N�^
	~Score(void);

	//�C�����C���֐�
	void SetScore(DWORD score) { m_Score = score; }
	DWORD GetScore()	{ return m_Score; }
	void SetHiScore(DWORD hiscore) { m_HiScoret = m_HiScore = hiscore; }
	DWORD GetHiScore()	{ return m_HiScore; }
	void SetScoreMax(DWORD max) { m_ScoreMax = max; }

	void ScoreUp(DWORD plus);
	void ScoreDown(DWORD minus);
	BOOL SaveScore();
	BOOL LoadScore();
};

#endif
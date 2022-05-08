/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Score.cpp									 */
/*	DESCRIPTION: スコアを制御する						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/11(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "Score.h"

/*********************************************************/
/*	コンストラクタ										 */
/*********************************************************/
Score::Score(void)
{
	//データ初期化
	m_HiScore = m_Score = m_ScoreMax = 0;
	
	if(!LoadScore()) exit(1);
}

/*********************************************************/
/*	デストラクタ										 */
/*********************************************************/
Score::~Score(void)
{
	SaveScore();
}

/*********************************************************/
/*	メソッド:ScoreUp									 */
/*	機能:スコアを増やす									 */
/*	引数:プラスするスコア								 */
/*********************************************************/
void Score::ScoreUp(DWORD plus)
{
	//スコアをプラス
	m_Score += plus;

	//最大スコアを越えた
	if(m_Score > m_ScoreMax)
		m_Score = m_ScoreMax;

	//スコアがハイスコアを越えていたら
	if(m_Score > m_HiScore)
		m_HiScore = m_Score;	//ハイスコアに現在のスコアを代入
}

/*********************************************************/
/*	メソッド:ScoreDown									 */
/*	機能:スコアを減らす									 */
/*	引数:マイナスするスコア								 */
/*********************************************************/
void Score::ScoreDown(DWORD minus)
{
	//スコアをマイナス
	m_Score -= minus;

	//スコアがハイスコアを越えていたが、減点により以前のハイスコアを下回ったら
	if(m_Score < m_HiScoret)
		m_HiScore = m_HiScoret;
}

/*********************************************************/
/*	メソッド:SaveScore									 */
/*	機能:スコアをファイルに保存する						 */
/*	戻値:TRUE = 成功 FALSE = 失敗						 */
/*********************************************************/
BOOL Score::SaveScore()
{
	ofstream fout("score.dat", ofstream::binary);
	if(!fout.is_open())
		return FALSE;


	fout << m_HiScore;		//データ書き込み
	fout.close();			//ファイルクローズ

	return TRUE;
}

/*********************************************************/
/*	メソッド:LoadScore									 */
/*	機能:スコアをファイルから読み込む					 */
/*	戻値:TRUE = 成功 FALSE = 失敗						 */
/*********************************************************/
BOOL Score::LoadScore()
{
	//データ読み込み
	ifstream fin("score.dat", ifstream::binary);
	
	//開けなかったら
	if(!fin.is_open())
	{
		//書き込み用としてファイルオープン
		ofstream fout("score.dat", ofstream::binary);

		//開けなかったらFALSEを返す
		if(!fout.is_open())
			return FALSE;
		//開けたらデータ書き込み(0)
		else
			fout << m_HiScore;

		fout.close();		//ファイルクローズ
	}
	else
	{
		fin >> m_HiScore >> m_HiScoret;	//ハイスコア読み込み
		fin.close();					//ファイルクローズ
	}

	return TRUE;
}
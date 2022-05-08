/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Score.cpp									 */
/*	DESCRIPTION: �X�R�A�𐧌䂷��						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/11(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "Score.h"

/*********************************************************/
/*	�R���X�g���N�^										 */
/*********************************************************/
Score::Score(void)
{
	//�f�[�^������
	m_HiScore = m_Score = m_ScoreMax = 0;
	
	if(!LoadScore()) exit(1);
}

/*********************************************************/
/*	�f�X�g���N�^										 */
/*********************************************************/
Score::~Score(void)
{
	SaveScore();
}

/*********************************************************/
/*	���\�b�h:ScoreUp									 */
/*	�@�\:�X�R�A�𑝂₷									 */
/*	����:�v���X����X�R�A								 */
/*********************************************************/
void Score::ScoreUp(DWORD plus)
{
	//�X�R�A���v���X
	m_Score += plus;

	//�ő�X�R�A���z����
	if(m_Score > m_ScoreMax)
		m_Score = m_ScoreMax;

	//�X�R�A���n�C�X�R�A���z���Ă�����
	if(m_Score > m_HiScore)
		m_HiScore = m_Score;	//�n�C�X�R�A�Ɍ��݂̃X�R�A����
}

/*********************************************************/
/*	���\�b�h:ScoreDown									 */
/*	�@�\:�X�R�A�����炷									 */
/*	����:�}�C�i�X����X�R�A								 */
/*********************************************************/
void Score::ScoreDown(DWORD minus)
{
	//�X�R�A���}�C�i�X
	m_Score -= minus;

	//�X�R�A���n�C�X�R�A���z���Ă������A���_�ɂ��ȑO�̃n�C�X�R�A�����������
	if(m_Score < m_HiScoret)
		m_HiScore = m_HiScoret;
}

/*********************************************************/
/*	���\�b�h:SaveScore									 */
/*	�@�\:�X�R�A���t�@�C���ɕۑ�����						 */
/*	�ߒl:TRUE = ���� FALSE = ���s						 */
/*********************************************************/
BOOL Score::SaveScore()
{
	ofstream fout("score.dat", ofstream::binary);
	if(!fout.is_open())
		return FALSE;


	fout << m_HiScore;		//�f�[�^��������
	fout.close();			//�t�@�C���N���[�Y

	return TRUE;
}

/*********************************************************/
/*	���\�b�h:LoadScore									 */
/*	�@�\:�X�R�A���t�@�C������ǂݍ���					 */
/*	�ߒl:TRUE = ���� FALSE = ���s						 */
/*********************************************************/
BOOL Score::LoadScore()
{
	//�f�[�^�ǂݍ���
	ifstream fin("score.dat", ifstream::binary);
	
	//�J���Ȃ�������
	if(!fin.is_open())
	{
		//�������ݗp�Ƃ��ăt�@�C���I�[�v��
		ofstream fout("score.dat", ofstream::binary);

		//�J���Ȃ�������FALSE��Ԃ�
		if(!fout.is_open())
			return FALSE;
		//�J������f�[�^��������(0)
		else
			fout << m_HiScore;

		fout.close();		//�t�@�C���N���[�Y
	}
	else
	{
		fin >> m_HiScore >> m_HiScoret;	//�n�C�X�R�A�ǂݍ���
		fin.close();					//�t�@�C���N���[�Y
	}

	return TRUE;
}
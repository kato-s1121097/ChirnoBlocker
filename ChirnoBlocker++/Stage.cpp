/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Stage.cpp									 */
/*	DESCRIPTION: �X�e�[�W�̏����Ǘ�����				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/05(f::jndbcph)						 */
/*	UPDATE: 2010/01/24(f::jndbcph)						 */
/*********************************************************/
#include "Stage.h"

/*********************************************************/
/*	�R���X�g���N�^										 */
/*********************************************************/
Stage::Stage(HWND &hWnd, DWORD stage)
{
	try
	{
		m_isVisible = new BOOL[96];
		m_type = new enum TYPE[96];
	}
	catch(bad_alloc ba)
	{
		SendMessage(hWnd, WM_QUIT, ERR_ALLOC, 0);
	}

	//�t�@�C���I�[�v��
	stage_data.open("stage\\stage.csv");
	if(!stage_data.is_open())
	{
		SendMessage(hWnd, WM_QUIT, ERR_FILEOPEN, 0);
	}
	
	char ch;
	DWORD index = 0;
	DWORD staget;
	staget = stage - 1;
	staget *= 195;
	staget += 15;

	//�ǂݎ̂�
	for(; staget; staget--)
		stage_data >> ch;
	while(index < 96)
	{
		stage_data >> ch;
		//�J���}�Ȃ牽�����Ȃ�
		if(ch == ',')
			continue;

		switch(ch)
		{
		case '0':
			m_type[index] = CIRCLE9;
			break;
		case '1':
			m_type[index] = BROWN;
			break;
		case '2':
			m_type[index] = RED;
			break;
		case '3':
			m_type[index] = YELLOW;
			break;
		case '4':
			m_type[index] = GREEN;
			break;
		case '5':
			m_type[index] = PURPLE;
			break;
		case '6':
			m_type[index] = PINK;
			break;
		case '7':
			m_type[index] = GRAY;
			break;
		case '8':
			m_type[index] = ICE;
			break;
		case '9':
			m_type[index] = NOTTHING;
			break;
		}
		//�����Ȃ���΁A�\�����Ȃ�
		if(m_type[index] == NOTTHING)
			m_isVisible[index] = FALSE;
		else
			m_isVisible[index] = TRUE;

		index++;
	}
}

/*********************************************************/
/*	�f�X�g���N�^										 */
/*********************************************************/
Stage::~Stage(void)
{
	stage_data.close();
	delete [] m_isVisible;
	delete [] m_type;

	m_isVisible = 0;
	m_type = 0;
}

/*********************************************************/
/*	���\�b�h:CheckClear									 */
/*	�@�\:�X�e�[�W���N���A�[���Ă��邩�m�F����			 */
/*********************************************************/
BOOL Stage::CheckClear()
{
	int i = 0;
	BOOL clear_flg = TRUE;

	//�T��
	for(; i < 96; i++)
	{
		if(m_isVisible[i])
		{
			clear_flg = FALSE;
			break;
		}
	}

	return clear_flg;
}
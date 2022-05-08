/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Thread.cpp									 */
/*	DESCRIPTION: �Q�[���p�X���b�h						 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/15(f::jndbcph)						 */
/*********************************************************/
#include "Thread.h"
#include "MCI.h"

/*********************************************************/
/*	�A�j���[�V�����X���b�h								 */
/*********************************************************/
DWORD WINAPI ThreadFunction(LPVOID vdParam)
{
	THREAD_PARAM *parameter;
	DWORD nowTime = 0, progress = 0, idealTime = 0, frames = 0, beforeTime;
	enum DISP now_disp;
	BOOL load_completed = FALSE;
	
	parameter = static_cast<THREAD_PARAM *>(vdParam);
	
	//���Ԏ擾
	beforeTime = timeGetTime();
	
	//�E�B���h�E�����݂����
	while(IsWindow(parameter->hWnd))
	{
		now_disp = parameter->game->GetDisplay();

		nowTime = timeGetTime();								//���ݎ��Ԏ擾
		progress = nowTime - beforeTime;						//�o�ߎ���
		idealTime = static_cast<DWORD>(frames * (1000.0f / parameter->dwFPS));	//���z����

		//�����ǂݍ���
		if(!load_completed && now_disp == READY)
		{
			parameter->game->ball->m_reflect = new MCI(parameter->hWnd, TEXT("SE\\Reflect.wav"), TYPE_SE);
			load_completed = TRUE;
		}

		if(now_disp == GAME)
		{
			//�{�[���̈ړ�
			parameter->game->ball->Move(parameter->game->player->GetRect());

			if(!parameter->game->ball->IsShot())
			{
				parameter->game->ball->SetPoint(MAKELONG((HIWORD(parameter->game->player->GetPoint()) + BAR_WIDTH / 2) - (BALL_WIDTH / 2), 516));
			}
		}

		//�v���C���[�̈ړ�
		if(now_disp == GAME && parameter->control)
			parameter->game->player->Move();

		InvalidateRect(parameter->hWnd, NULL, FALSE);

		//���z���Ԃ��o�ߎ��Ԃ̕���������Α҂�
		if(idealTime > progress) Sleep(idealTime - progress);

		//1�b�o��
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
/*********************************************************/
/*	TYPE: C++											 */
/*	PROJECT: ChirnoBlocker++							 */
/*	PROGRAM: Game.cpp									 */
/*	DESCRIPTION: �Q�[����ʐ؂�ւ����s��				 */
/*	VERSION:Beta										 */
/*	USED: VC++2008										 */
/*	CREATE: 2010/01/04(f::jndbcph)						 */
/*	UPDATE: 2010/02/15(f::jndbcph)						 */
/*********************************************************/
#include "Game.h"

template <class T>
inline void SafeRelease(T* p) { if(p) delete p; p = 0;}

/*********************************************************/
/*	�R���X�g���N�^										 */
/*********************************************************/
Game::Game(HDC hdc, HWND &hWnd)
{
	//�R���t�B�O�ǂݍ���
	ifstream fconfig("config.dat", ifstream::binary);
	char ch;
	if(fconfig.is_open())
	{
		fconfig >> ch;
		visible = atoi(&ch);
		fconfig >> ch;
		control = atoi(&ch);
		fconfig.close();
	}
	else
	{
		visible = 1;
	}
	//�J�[�\����\��
	if(!visible)
		ShowCursor(FALSE);

	//GDI�I�u�W�F�N�g����
	hBigFont	= CreateFont(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, 0);
	hNormalFont = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, 0);
	hSmallFont	= CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, 0);

	//�r�b�g�}�b�v�ǂݍ���
	hBlockBmp		 = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\Block.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hGameBackBmp	 = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\BackTest.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hBackgroundBmp	 = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\Background.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hTitleBackBmp	 = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\TitleBack.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hLoadBackBmp	 = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\LoadBack.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hGameOverBackBmp = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\GameOverBack.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	hClearBackBmp	 = static_cast<HBITMAP>(LoadImage(NULL, TEXT("image\\ClearBack.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	//�t���O������
	click_flg &= 0x00;
	load_flg = 0;
	load_flg |= LOADFLG_INITIALIZE;

	//�X�e�[�W�ԍ�������
	now_stage = 1;

	//�X�R�A������
	score = new Score();
	score->SetScoreMax(9999999);
	t_Score = 0;
	t_HiScore = score->GetHiScore();

	//�W���C�X�e�B�b�N������
	if(control == 2)
	{
		joystick = new JoyStick();
		if(!joystick->CheckSet())
			SendMessage(hWnd, WM_QUIT, ERR_JOY, 0);
	}
	
	//�^�C�}�[������
	game_timer = new Timer();

	//�����ǂݍ���
	clear_se = new MCI(hWnd, TEXT("SE\\Clear.mid"), TYPE_BGM);
	blockbreak_se = new MCI(hWnd, TEXT("SE\\BlockBreak.wav"), TYPE_SE);
	title = new MCI(hWnd, TEXT("BGM\\Title.mid"), TYPE_BGM);

	//�^�C�g���\��
	SetDisplay(LOAD);
}

/*********************************************************/
/*	�f�X�g���N�^										 */
/*********************************************************/
Game::~Game(void)
{
	//GDI�I�u�W�F�N�g�j��
	DeleteObject(hBlockBmp);
	DeleteObject(hTitleBackBmp);
	DeleteObject(hGameBackBmp);
	DeleteObject(hLoadBackBmp);
	DeleteObject(hGameOverBackBmp);
	DeleteObject(hClearBackBmp);
	DeleteObject(hBackgroundBmp);
	DeleteObject(hBigFont);
	DeleteObject(hNormalFont);
	DeleteObject(hSmallFont);

	//�N���X���������
	if(now_display != TITLE)
	{
		SafeRelease<Player>(player);
		SafeRelease<Ball>(ball);
		SafeRelease<MCI>(miss_se);
		SafeRelease<Stage>(stage);
		SafeRelease<MCI>(ending);
	}
	SafeRelease<MCI>(title);
	SafeRelease<JoyStick>(joystick);
	SafeRelease<Timer>(game_timer);
	SafeRelease<Score>(score);
	SafeRelease<MCI>(blockbreak_se);
	SafeRelease<MCI>(clear_se);
}

/*********************************************************/
/*	�֐�:Title											 */
/*	�@�\:�^�C�g����ʂ�`�悷��							 */
/*********************************************************/
void Game::Title(HDC &hdc, HWND &hWnd)
{
	static DWORD select = 0;
	static BOOL entry_flg = TRUE, wait_flg = FALSE;
	static Timer wait_timer;
	HDC hBmpDC = CreateCompatibleDC(NULL);
	POINT point;
	TCHAR title_str[] = TEXT("CirnoBlocker++"), hiscore[31];
	TEXTMETRIC tm;

	//��ʃN���A
	SelectObject(hBmpDC, hTitleBackBmp);
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hBmpDC, 0, 0, SRCCOPY);

	wsprintf(hiscore, TEXT("HiScore %d"), score->GetHiScore());

	//������@�ŕ���
	switch(control)
	{
	//�}�E�X
	case 0:
		//�}�E�X���W�擾
		GetCursorPos(&point);
		ScreenToClient(hWnd, &point);	//�N���C�A���g���W�֕ϊ�

		//�}�E�X���W�ɂ���đI���ʒu��ύX
		if(point.x > CLIENT_WIDTH / 2 - 42 && point.x < CLIENT_WIDTH / 2 + 50 && point.y > CLIENT_HEIGHT - 212 && point.y < CLIENT_HEIGHT - 174)
			select = 0;
		else if(point.x > CLIENT_WIDTH / 2 - 44 && point.x < CLIENT_WIDTH / 2 + 169 && point.y > CLIENT_HEIGHT - 149 && point.y < CLIENT_HEIGHT - 111)
			select = 1;

		//���N���b�N�����ꂽ��
		if(click_flg & LEFT_CLICKED)
		{
			if(point.x > STARTRECT_LEFT && point.x < STARTRECT_RIGHT && point.y > STARTRECT_TOP && point.y < STARTRECT_BOTTOM)
			{
				SetDisplay(LOAD);
				load_flg = LOADFLG_FROMTITLE;
				blockbreak_se->Play();
				title->Stop();
			}
			else if(point.x > QUITRECT_LEFT && point.x < QUITRECT_RIGHT && point.y > QUITRECT_TOP && point.y < QUITRECT_BOTTOM)
				SendMessage(hWnd, WM_QUIT, 0, 0);
		}

		break;
	//�L�[�{�[�h
	case 1:
		//�G���^�[�L�[�������ꂽ
		if(GetKeyState(VK_RETURN) & 0x8000)
		{
			if(!select)
			{
				SetDisplay(LOAD);
				load_flg = LOADFLG_FROMTITLE;
				blockbreak_se->Play();
				title->Stop();
			}
			else
			{
				SendMessage(hWnd, WM_QUIT, 0, 0);
			}
		}

		//�҂�
		if(wait_flg && wait_timer.GetInterval() > 300) wait_flg = FALSE;
		if(wait_flg) break;

		//��A�܂��͉��J�[�\���L�[�������ꂽ
		if(GetKeyState(VK_UP) & 0x8000 || GetKeyState(VK_DOWN) & 0x8000)
		{
			switch(select)
			{
			case 0:
				select = 1;
				break;
			case 1:
				select = 0;
				break;
			}

			//�҂��t���O�𗧂Ă�
			wait_timer.ResetTime();
			wait_flg = TRUE;
		}

		break;
	//�Q�[���p�b�h
	case 2:
		//1�{�^���������ꂽ�猈��
		if(joystick->CheckButton(JOY_BUTTON1))
		{
			if(!select)
			{
				SetDisplay(LOAD);
				load_flg = LOADFLG_FROMTITLE;
				blockbreak_se->Play();
				title->Stop();
			}
			else
			{
				SendMessage(hWnd, WM_QUIT, 0, 0);
			}
		}

		//�҂�
		if(wait_flg && wait_timer.GetInterval() > 300) wait_flg = FALSE;
		if(wait_flg) break;

		//��A�܂��͉��ɓ��͂����ꂽ
		if(joystick->CheckStick(JoyStick::JOY_VECTORDOWN) || joystick->CheckStick(JoyStick::JOY_VECTORUP))
		{
			//�I������Ă��鍀�ڂɂ���ĕ���
			switch(select)
			{
			case 0:
				select = 1;
				break;
			case 1:
				select = 0;
				break;
			}

			//�҂��t���O�𗧂Ă�
			wait_timer.ResetTime();
			wait_flg = TRUE;
		}
	}

	//�n�C�X�R�A�`��
	SelectObject(hdc, hSmallFont);
	SetTextColor(hdc, RGB(0, 0, 0xFF));
	GetTextMetrics(hdc, &tm);
	TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(hiscore) / 2), 5, hiscore, lstrlen(hiscore));

	//�^�C�g���`��
	SelectObject(hdc, hBigFont);
	SetTextColor(hdc, RGB(0, 0x88, 0));
	GetTextMetrics(hdc, &tm);
	TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(title_str) / 2), CLIENT_HEIGHT / 2 - 200, title_str, lstrlen(title_str));

	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	switch(select)
	{
	case 0:
		Rectangle(hdc, STARTRECT_LEFT, STARTRECT_TOP, STARTRECT_RIGHT, STARTRECT_BOTTOM);
		break;
	case 1:
		Rectangle(hdc, QUITRECT_LEFT, QUITRECT_TOP, QUITRECT_RIGHT, QUITRECT_BOTTOM);
		break;
	}

	//���j���[�`��
	SetTextColor(hdc, RGB(0, 0, 0xFF));
	SelectObject(hdc, hNormalFont);
	TextOut(hdc, CLIENT_WIDTH / 2 - 42, CLIENT_HEIGHT - 210, TEXT("START"), lstrlen(TEXT("START")));
	TextOut(hdc, CLIENT_WIDTH / 2 - 33, CLIENT_HEIGHT - 146, TEXT("QUIT"), lstrlen(TEXT("QUIT")));

	//�r�b�g�}�b�v�f�o�C�X�j��
	DeleteDC(hBmpDC);
}

/*********************************************************/
/*	�֐�:Load											 */
/*	�@�\:���[�h��ʂ�`�悷��							 */
/*********************************************************/
void Game::Load(HDC &hdc, HWND &hWnd)
{
	static DWORD beforeTime = timeGetTime();
	static BOOL entry_flg = FALSE;
	HDC hBmpDC = CreateCompatibleDC(NULL);
			
	//��ʃN���A
	SelectObject(hBmpDC, hLoadBackBmp);
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hBmpDC, 0, 0, SRCCOPY);

	SelectObject(hdc, hNormalFont);
	SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
	TextOut(hdc, CLIENT_WIDTH - 230, CLIENT_HEIGHT - 60, TEXT("Now Loading..."), lstrlen(TEXT("Now Loading...")));
	
	//���Ԍp���΍�
	if(entry_flg)
	{
		beforeTime = timeGetTime();
		entry_flg = FALSE;
	}

	//5�b�������玟�̉�ʂ�
	if((timeGetTime() - beforeTime) > 5000)
	{
		//�ړ���
		
		//�����N���܂��̓Q�[���I�[�o�[��ʂ���̈ړ�
		if(load_flg & LOADFLG_INITIALIZE || load_flg & LOADFLG_FROMGAMEOVER)
		{
			if(load_flg & LOADFLG_INITIALIZE && visible == 0 && control == 0) ShowCursor(TRUE);
			title->Play();
			score->SetScore(0);
			t_Score = 0;
			t_HiScore = score->GetHiScore();
			SetDisplay(TITLE);
		}
		
		//�^�C�g������̈ړ��I�����[
		if(load_flg & LOADFLG_FROMTITLE)
		{
			//�J�[�\���\��
			if(control == 0 && visible == 0) ShowCursor(FALSE);

			//�N���X������
			try
			{
				player = new Player();
				ball = new Ball();
				miss_se	= new MCI(hWnd, TEXT("SE\\Miss.wav"), TYPE_SE);
				stage = new Stage(hWnd, now_stage);
			}
			catch(bad_alloc ba)
			{
				SendMessage(hWnd, WM_QUIT, ERR_ALLOC, 0);
				DeleteDC(hBmpDC);
				return;
			}

			SetDisplay(READY);
		}

		//�X�e�[�W�N���A����̈ړ��I�����[
		if(load_flg & LOADFLG_FROMSTAGECLEAR || load_flg & LOADFLG_FROMCONTINUE)
		{
			//�X�R�A�Z�[�u
			score->SaveScore();

			//�X�e�[�W�̐؂�ւ�
			delete stage;

			//�X�R�A�e���v���[�g�̏���
			if(load_flg & LOADFLG_FROMCONTINUE)
			{
				score->SetScore(t_Score);
				score->SetHiScore(t_HiScore);
			}
			else
			{
				t_Score = score->GetScore();
				t_HiScore = score->GetHiScore();
			}

			//�J�[�\���\��
			if(load_flg & LOADFLG_FROMCONTINUE && control == 0 && visible == 0) ShowCursor(FALSE);

			try
			{
				stage = new Stage(hWnd, now_stage);
			}
			catch(bad_alloc ba)
			{
				SendMessage(hWnd, WM_QUIT, ERR_ALLOC, 0);
				DeleteDC(hBmpDC);
				return;
			}

			SetDisplay(READY);
		}

		//�X�e�[�W�I�[���N���A
		if(load_flg & LOADFLG_ALLCLEAR)
		{
			//�G���f�B���O����
			try
			{
				ending = new MCI(hWnd, TEXT("BGM\\Ending.mid"), TYPE_BGM);
			}
			catch(bad_alloc ba)
			{
				SendMessage(hWnd, WM_QUIT, ERR_ALLOC, 0);
			}

			//�X�R�A�Z�[�u
			score->SaveScore();

			//�X�R�A���Z�b�g
			score->SetScore(0);
			t_Score = 0;
			t_HiScore = score->GetHiScore();

			delete player;
			delete ball;
			delete miss_se;
			SetDisplay(ALLCLEAR);
		}

		entry_flg = TRUE;
		load_flg &= 0x00;
	}

	//�r�b�g�}�b�v�f�o�C�X�j��
	DeleteDC(hBmpDC);
}

/*********************************************************/
/*	�֐�:Ready											 */
/*	�@�\:�X�e�[�W�ɓ���O�̉��							 */
/*********************************************************/
void Game::Ready(HDC &hdc)
{
	TCHAR str[30];
	TEXTMETRIC tm;
	static POINT *stage_info = new POINT();
	static BOOL entry_flg = TRUE;
	static Timer *anime_timer = new Timer();
	blockbreak_se->Stop();

	//�f�o�C�X�R���e�L�X�g���ύX
	SelectObject(hdc, hBigFont);
	SetTextColor(hdc, RGB(0, 0, 0xFF));
	GetTextMetrics(hdc, &tm);
	
	//��ʃN���A
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);

	if(entry_flg)
	{
		anime_timer = new Timer();
		stage_info = new POINT();
		stage_info->x = CLIENT_WIDTH;
		stage_info->y = CLIENT_HEIGHT / 2 - 40;
		entry_flg = FALSE;
	}


	//�X�e�[�W������`��
	wsprintf(str, TEXT("STAGE %d"), now_stage);
	TextOut(hdc, stage_info->x, stage_info->y, str, lstrlen(str));
	
	//�����X�N���[��
	stage_info->x -= 64;

	//�����~��
	if(stage_info->x < (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(str) / 2) && anime_timer->GetInterval() < 4000)
	{
		stage_info->x = (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(str) / 2);
	}

	//���Z�b�g
	if(stage_info->x < -(tm.tmAveCharWidth * lstrlen(str)))
	{
		SafeRelease<POINT>(stage_info);
		SafeRelease<Timer>(anime_timer);
		entry_flg = TRUE;
		SetDisplay(GAME);
		//�^�C�}�[�J�n
		game_timer->ResetTime();
	}
}

/*********************************************************/
/*	�֐�:Game											 */
/*	�@�\:�Q�[����ʂ�`�悷��							 */
/*********************************************************/
void Game::GameMain(HDC &hdc, HWND &hWnd)
{
	HDC hBmpDC = CreateCompatibleDC(NULL);
	TCHAR hiscore_str[60], score_str[60];
	TCHAR player_str[] = TEXT("Player");
	BYTE credit = player->GetCredit();
	static BOOL miss_flg = FALSE, blockbreak_flg = FALSE;
	static Timer blockbreak_timer, miss_timer;

	//�Q�[����ʕ`��
	SelectObject(hBmpDC, hGameBackBmp);
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hBmpDC, 0, 0, SRCCOPY);
	SelectObject(hBmpDC, hBackgroundBmp);
	BitBlt(hdc, DX, DY, DWIDTH, DHEIGHT, hBmpDC, 0, 2240 - now_stage * DHEIGHT, SRCCOPY);

	//���`��
	wsprintf(hiscore_str, TEXT("HiScore %07d"), score->GetHiScore());
	wsprintf(score_str, TEXT("Score   %07d"), score->GetScore());
	SelectObject(hdc, hNormalFont);
	TextOut(hdc, 538, 44, hiscore_str, lstrlen(hiscore_str));
	TextOut(hdc, 538, 85, score_str, lstrlen(score_str));
	TextOut(hdc, 538, 153, player_str, lstrlen(player_str));
	int i = 0;


	switch(control)
	{
	//�}�E�X
	case 0:
		//���N���b�N�Ŕ���
		if(!ball->IsShot() && click_flg & LEFT_CLICKED)
			ball->SetShotFlag(TRUE);
		break;
	//�L�[�{�[�h
	case 1:
		//Enter�L�[�Ŕ���
		if(!ball->IsShot() && GetKeyState(VK_RETURN) & 0x8000)
			ball->SetShotFlag(TRUE);
		break;
	case 2:
		//1�{�^���Ŕ���
		if(!ball->IsShot() && joystick->CheckButton(JOY_BUTTON1))
			ball->SetShotFlag(TRUE);
		break;
	}

	//�~�X����
	if(ball->IsFall())
	{
		credit--;
		player->SetCredit(credit);
		if(!credit)
		{
			//�t���O���Z�b�g
			miss_flg = FALSE;

			//�{�[���ݒ�
			ball->SetPoint(MAKELONG(253, 516));
			ball->SetShotFlag(FALSE);

			//�`���m������
			player->SetVisible(FALSE);

			//�J�[�\���\��
			if(control == 0 && visible == 0) ShowCursor(TRUE);

			SetDisplay(CONTINUE);
		}
		ball->SetShotFlag(FALSE);
		miss_se->Play();
		miss_timer.ResetTime();
		miss_flg = TRUE;
	}

	//�c�@�`��
	for(; i < credit; i++)
	{
		SelectObject(hBmpDC, player->GetMask(2));
		BitBlt(hdc, 657 + i * 22, 162, 16, 20, hBmpDC, 0, 0, SRCAND);
		SelectObject(hBmpDC, player->GetBmp(2));
		BitBlt(hdc, 657 + i * 22, 162, 16, 20, hBmpDC, 0, 0, SRCPAINT);
	}

	/*****************************************************/
	/* �u���b�N�`��										 */
	/*****************************************************/
	i = 0;
	SelectObject(hBmpDC, hBlockBmp);
	for(; i < 96; i++)
	{
		if(stage->GetBlockState(i))
		{
			//�u���b�N�̎�ނ���������
			switch(stage->GetType(i))
			{
			case CIRCLE9:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 0, 0, SRCCOPY);
				break;
			case BROWN:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 64, 0, SRCCOPY);
				break;
			case RED:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 128, 0, SRCCOPY);
				break;
			case YELLOW:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 192, 0, SRCCOPY);
				break;
			case GREEN:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 256, 0, SRCCOPY);
				break;
			case PURPLE:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 320, 0, SRCCOPY);
				break;
			case PINK:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 384, 0, SRCCOPY);
				break;
			case GRAY:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 448, 0, SRCCOPY);
				break;
			case ICE:
				BitBlt(hdc, DX + (i % 8 * 64), DY + (i / 8 * 32), 64, 32, hBmpDC, 512, 0, SRCCOPY);
				break;
			}
		}

	}
	
	/*****************************************************/
	/* �v���C���[�`��									 */
	/*****************************************************/
	//�v���C���[���W���擾
	DWORD player_point = player->GetPoint();
	
	//�v���C���[�����Ȃ�
	if(player->IsVisible())
	{	
		//�o�[�`��
		SelectObject(hBmpDC, player->GetMask(TRUE));
		BitBlt(hdc, HIWORD(player_point), BAR_Y, BAR_WIDTH, BAR_HEIGHT, hBmpDC, 0, 0, SRCAND);
		SelectObject(hBmpDC, player->GetBmp(TRUE));
		BitBlt(hdc, HIWORD(player_point), BAR_Y, BAR_WIDTH, BAR_HEIGHT, hBmpDC, 0, 0, SRCPAINT);
	
		//�L�����`��
		SelectObject(hBmpDC, player->GetMask(FALSE));
		BitBlt(hdc, LOWORD(player_point), CHAR_Y, CHAR_WIDTH, CHAR_HEIGHT, hBmpDC, 0, 0, SRCAND);
		SelectObject(hBmpDC, player->GetBmp(FALSE));
		BitBlt(hdc, LOWORD(player_point), CHAR_Y, CHAR_WIDTH, CHAR_HEIGHT, hBmpDC, 0, 0, SRCPAINT);
	

		/*****************************************************/
		/*	�{�[���`��										 */
		/*****************************************************/
		//�{�[�����W���擾
		DWORD ball_point = ball->GetPoint();

		//�{�[���`��
		SelectObject(hBmpDC, ball->GetMask());
		BitBlt(hdc, LOWORD(ball_point), HIWORD(ball_point), BALL_WIDTH, BALL_HEIGHT, hBmpDC, 0, 0, SRCAND);
		SelectObject(hBmpDC, ball->GetBmp());
		BitBlt(hdc, LOWORD(ball_point), HIWORD(ball_point), BALL_WIDTH, BALL_HEIGHT, hBmpDC, 0, 0, SRCPAINT);

		//�Փ˔���
		if(ball->Collision(stage))
		{
			//�|�C���g������
			score->ScoreUp(50);
			
			//SE��炷
			if(!blockbreak_flg)
			{
				blockbreak_se->Play();
				blockbreak_timer.ResetTime();
				blockbreak_flg = TRUE;
			}
		}
	}

	//�~�X���𗬂���1.2�b�o�������~
	if(miss_flg && miss_timer.GetInterval() > 1200)
	{
		miss_se->Stop();
		miss_flg = FALSE;
	}

	if(blockbreak_flg && blockbreak_timer.GetInterval() > 100)
	{
		blockbreak_se->Stop();
		blockbreak_flg = FALSE;
	}

	//�N���A���Ă�����N���A��ʂ�
	if(stage->CheckClear())
	{
		//�t���O���Z�b�g
		miss_flg = FALSE;
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		
		//�v���C���[�ʒu���Z�b�g
		ball->SetPoint(MAKELONG(253, 516));
		player->SetPoint(197);
		
		//4���z����
		if(now_stage > 4)
		{
			//���݃X�e�[�W���Z�b�g
			now_stage = 1;

			//�I�[���N���A
			load_flg |= LOADFLG_ALLCLEAR;
			SetDisplay(STAGECLEAR);
		}

		//�X�e�[�W�N���A
		SetDisplay(STAGECLEAR);
		ball->SetShotFlag(FALSE);
	}

	//�r�b�g�}�b�v�f�o�C�X�j��
	DeleteDC(hBmpDC);
}

/*********************************************************/
/*	�֐�:StageClear										 */
/*	�@�\:�X�e�[�W�N���A��ʂ�`�悷��					 */
/*********************************************************/
void Game::StageClear(HDC &hdc, HWND &hWnd)
{
	LONG time_score = 0, credit_score = 0;
	TCHAR clear_str[] = TEXT("STAGE CLEAR!!"), clearbonus_str[] = TEXT("�N���A�{�[�i�X\t1000");
	TEXTMETRIC tm;
	static BOOL entry_flg = TRUE;
	static TCHAR timebonus_str[50], creditbonus_str[50], sum_score[60], now_score[60];
	HDC hBmpDC = CreateCompatibleDC(NULL);

	//��ʃN���A
	SelectObject(hBmpDC, hClearBackBmp);
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hBmpDC, 0, 0, SRCCOPY);

	//�F��񏉊���
	if(entry_flg)
	{
		clear_se->Play();
		entry_flg = FALSE;

		//�X�R�A���Z
		credit_score = player->GetCredit() * 5;				//�N���W�b�g�{�[�i�X�v�Z
		time_score = 180000 - game_timer->GetInterval();	//�^�C���{�[�i�X�v�Z
		if(time_score < 0) time_score = 0;					//3���ȓ��ɃN���A���Ă��Ȃ�������^�C���{�[�i�X��0
		score->ScoreUp(time_score);
		score->ScoreUp(1000);
		score->ScoreUp(credit_score);

		wsprintf(creditbonus_str, TEXT("�c�@�{�[�i�X\t%d * 5"), player->GetCredit());
		wsprintf(timebonus_str, TEXT("�^�C���{�[�i�X\t%d"), time_score);
		wsprintf(sum_score, TEXT("���v\t%d"), time_score + credit_score + 1000);
		wsprintf(now_score, TEXT("�X�R�A\t%d"), score->GetScore());
	}

	//STAGE CLEAR!!�`��(��X�����Ƃ����X�R�A��ʂɂ���)
	SelectObject(hdc, hBigFont);
	GetTextMetrics(hdc, &tm);
	SetTextColor(hdc, RGB(0xFF, 0xFF, 0));
	TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(clear_str) / 2), 35, clear_str, lstrlen(clear_str));

	//�X�R�A���Z���ʕ`��
	SelectObject(hdc, hNormalFont);
	SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
	TabbedTextOut(hdc, 223, 145, clearbonus_str, lstrlen(clearbonus_str), 0, NULL, 0);
	TabbedTextOut(hdc, 223, 219, creditbonus_str, lstrlen(creditbonus_str), 0, NULL, 0);
	TabbedTextOut(hdc, 223, 293, timebonus_str, lstrlen(timebonus_str), 0, NULL, 0);
	TabbedTextOut(hdc, 223, 367, sum_score, lstrlen(sum_score), 0, NULL, 0);
	TabbedTextOut(hdc, 223, 441, now_score, lstrlen(now_score), 0, NULL, 0);

	//����{�^���Ői��
	if(click_flg & LEFT_CLICKED || GetKeyState(VK_RETURN) & 0x8000 || joystick->CheckButton(JOY_BUTTON1))
	{
		//�N���A���X�g�b�v
		clear_se->Stop();

		now_stage++;
		if(now_stage > 4)
			load_flg |= LOADFLG_ALLCLEAR;
		else
			load_flg |= LOADFLG_FROMSTAGECLEAR;
		SetDisplay(LOAD);
		entry_flg = TRUE;
	}

	//�r�b�g�}�b�v�p�f�o�C�X���
	DeleteDC(hBmpDC);
}

/*********************************************************/
/*	�֐�:GameOver										 */
/*	�@�\:�Q�[���I�[�o�[��ʂ�`�悷��					 */
/*********************************************************/
void Game::GameOver(HDC &hdc)
{
	static DWORD beforeTime = timeGetTime();
	static BOOL entry_flg = FALSE;
	static int *color = new int(0);
	HDC hBmpDC = CreateCompatibleDC(NULL);
	TCHAR gameover[] = TEXT("GAME OVER");
	TEXTMETRIC tm;
	
	//��ʃN���A
	SelectObject(hBmpDC, hGameOverBackBmp);
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, hBmpDC, 0, 0, SRCCOPY);

	//���Ԍp���΍�
	if(entry_flg)
	{
		color = new int(0);
		beforeTime = timeGetTime();
		entry_flg = FALSE;
	}
	
	//GAME OVER�`��(��X�����ƍl����)
	SelectObject(hdc, hBigFont);
	SetTextColor(hdc, RGB(*color, 0x00, 0x00));
	GetTextMetrics(hdc, &tm);
	TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * 9 / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent / 2), gameover, lstrlen(gameover));
	
	//�J���[�C���N�������g(��X�����邩��)
	*color += 2;
	if(*color > 0xFF) *color = 0xFF;

	
	//5�b�o������^�C�g����
	if((timeGetTime() - beforeTime) > 5000)
	{
		load_flg |= LOADFLG_FROMGAMEOVER;
		SetDisplay(LOAD);
		now_stage = 1;
		entry_flg = TRUE;
		//�I�u�W�F�N�g���
		delete player;
		delete ball;
		delete miss_se;
		
		//�J���[�̈���
		delete color;
		color = 0;		//0�|�C���^
	}

	//�r�b�g�}�b�v�f�o�C�X�j��
	DeleteDC(hBmpDC);
}

/*********************************************************/
/*	�֐�:Continue										 */
/*	�@�\:�R���e�B�j���[��ʂ�`�悷��					 */
/*********************************************************/
void Game::Continue(HDC &hdc, HWND &hWnd)
{
	TCHAR continue_str[] = TEXT("Continue?");
	TCHAR yes[] = TEXT("Yes!");
	TCHAR no[] = TEXT("No");
	TEXTMETRIC tm;
	POINT cursor_point;
	static Timer wait_timer;
	static BOOL select = 0, wait_flg = FALSE;

	//������\��
	SelectObject(hdc, hBigFont);
	GetTextMetrics(hdc, &tm);
	SetTextColor(hdc, RGB(0, 0, 0xFF));
	TextOut(hdc, ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 9 / 2), (CLIENT_HEIGHT / 2) - 200, continue_str, lstrlen(continue_str));

	SelectObject(hdc, hNormalFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	GetTextMetrics(hdc, &tm);
	if(!select) SetTextColor(hdc, RGB(0, 0x88, 0));
	TextOut(hdc, ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 4 / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10), yes, lstrlen(yes));
	if(!select) SetTextColor(hdc, RGB(0, 0, 0));
	if(select)  SetTextColor(hdc, RGB(0, 0x88, 0));
	TextOut(hdc, ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 2 / 2), (CLIENT_HEIGHT / 2) + 10, no, lstrlen(no));
	
	switch(control)
	{
	//�}�E�X
	case 0:
		//�}�E�X�J�[�\������
		GetCursorPos(&cursor_point);
		ScreenToClient(hWnd, &cursor_point);
		if(cursor_point.x > ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 4 / 2) && cursor_point.x < ((DX + DWIDTH) / 2) + (tm.tmAveCharWidth / 2) && cursor_point.y > (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10) && cursor_point.y < (CLIENT_HEIGHT / 2) - 10)
			select = 0;
		else if(cursor_point.x > ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 2 / 2) && cursor_point.x < ((DX + DWIDTH) / 2) + (tm.tmAveCharWidth / 2) && cursor_point.y > (CLIENT_HEIGHT / 2) + 10 && cursor_point.y < (CLIENT_HEIGHT / 2) + (tm.tmAscent + 10))
			select = 1;
	
		//���N���b�N���ꂽ
		if(click_flg & LEFT_CLICKED)
		{
			//Yes���I������Ă��ăJ�[�\���������Ă���΃R���e�B�j���[
			if(!select && cursor_point.x > ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 4 / 2) && cursor_point.x < ((DX + DWIDTH) / 2) + (tm.tmAveCharWidth / 2) && cursor_point.y > (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10) && cursor_point.y < (CLIENT_HEIGHT / 2) - 10)
			{
				player->SetVisible(TRUE);
				player->SetCredit(3);
				load_flg |= LOADFLG_FROMCONTINUE;
				SetDisplay(LOAD);
			}
			//No���I������Ă��ăJ�[�\���������Ă���΃Q�[���I�[�o�[
			else if(select && cursor_point.x > ((DX + DWIDTH) / 2) - (tm.tmAveCharWidth * 2 / 2) && cursor_point.x < ((DX + DWIDTH) / 2) + (tm.tmAveCharWidth / 2) && cursor_point.y > (CLIENT_HEIGHT / 2) + 10 && cursor_point.y < (CLIENT_HEIGHT / 2) + (tm.tmAscent + 10))
			{
				SetDisplay(GAMEOVER);
			}
		}

		break;
	//�L�[�{�[�h
	case 1:
		//�҂��̏���
		if(wait_flg && wait_timer.GetInterval() > 200) wait_flg = FALSE;
		if(wait_flg) break;

		//��A�܂��͉��J�[�\���L�[�������ꂽ
		if(GetKeyState(VK_UP) & 0x8000 || GetKeyState(VK_DOWN) & 0x8000)
		{
			if(!select) select = 1;
			else		select = 0;
			wait_flg = TRUE;
			wait_timer.ResetTime();
		}

		//�G���^�[�L�[�������ꂽ
		if(GetKeyState(VK_RETURN) & 0x8000)
		{
			//Yes���I������Ă���
			if(!select)
			{
				//�v���C���[��񃊃Z�b�g
				player->SetVisible(TRUE);
				player->SetCredit(3);

				//���[�h�t���O�Z�b�g
				load_flg |= LOADFLG_FROMCONTINUE;
				SetDisplay(LOAD);
			}
			else
			{
				SetDisplay(GAMEOVER);
			}
		}

		break;
	case 2:
		//�҂��̏���
		if(wait_flg && wait_timer.GetInterval() > 200) wait_flg = FALSE;
		if(wait_flg) break;

		//��A�܂��͉��ɓ��͂��ꂽ
		if(joystick->CheckStick(JoyStick::JOY_VECTORUP) || joystick->CheckStick(JoyStick::JOY_VECTORDOWN))
		{
			if(!select) select = 1;
			else		select = 0;
			wait_flg = TRUE;
			wait_timer.ResetTime();
		}

		//1�{�^�������͂��ꂽ
		if(joystick->CheckButton(JOY_BUTTON1))
		{
			//Yes���I������Ă���
			if(!select)
			{
				//�v���C���[��񃊃Z�b�g
				player->SetVisible(TRUE);
				player->SetCredit(3);

				//���[�h�t���O�Z�b�g
				load_flg |= LOADFLG_FROMCONTINUE;
				SetDisplay(LOAD);
			}
			else
			{
				SetDisplay(GAMEOVER);
			}
		}
	}
}

/*********************************************************/
/*	�֐�:AllClear										 */
/*	�@�\:�I�[���N���A��ʂ�\������						 */
/*********************************************************/
void Game::AllClear(HDC &hdc)
{
	static Timer change_timer;
	static SHORT cur = 0;
	static SHORT color = 0, color2 = 0;
	static BOOL entry_flg = TRUE;
	DWORD interval;
	TCHAR congratulation[] = TEXT("�̌��ł͂����܂łł�");
	TCHAR author1[] = TEXT("Program");
	TCHAR author2[] = TEXT("f::jndbcph");
	TCHAR author3[] = TEXT("Dots Graphics");
	TCHAR background[] = TEXT("Background");
	TCHAR background2[] = TEXT("��.com");
	TCHAR SE[] = TEXT("SE");
	TCHAR SE2[] = TEXT("�U�E�}�b�`���C�J�@�Y2nd");
	TCHAR BGM[] = TEXT("BGM");
	TCHAR BGM2[] = TEXT("���I�����y");
	TCHAR message1[] = TEXT("Thank you for playing!");
	TCHAR message2[] = TEXT("ChirnoBlocker++�����ł��y���݂ɁI");
	TEXTMETRIC tm;

	//�G���g��
	if(entry_flg)
	{
		change_timer.ResetTime();
		//�G���f�B���O�Đ�
		ending->Play();
		entry_flg = FALSE;
	}

	//�C���^�[�o���擾
	interval = change_timer.GetInterval();

	//�f�o�C�X�R���e�L�X�g���ύX
	SelectObject(hdc, hNormalFont);
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));

	//�e�L�X�g���g���b�N�擾
	GetTextMetrics(hdc, &tm);

	//0.2�b�o���ƂɐF�̖��x����
	if(interval > 200)
	{
		color += 2;
		if(cur == 2 || cur == 4 || cur == 6 || cur == 8 || cur == 10)
			color2 += 2;
		if(color > 0xFF) color = 0xFF;
		if(color2 > 0xFF) color2 = 0xFF;
	}

	//8�b�o�����玟�̕������
	if(interval > 8000)
	{
		color = 0;
		color2 = 0;

		//�\�������؂�ւ�
		cur++;

		//�S�ĕ\��������^�C�g��
		if(cur > 12)
		{

			SetDisplay(TITLE);

			//�����X�g�b�v & ���Z�b�g
			blockbreak_se->Stop();
			ending->Stop();

			//�G���f�B���O�|�C���^���
			delete ending;

			//�X�e�[�W���Z�b�g
			now_stage = 1;
			//�^�C�g��BGM�Đ�
			title->Play();

			return;
		}

		//�^�C�}�[���Z�b�g
		change_timer.ResetTime();
	}

	//�����\��
	switch(cur)
	{
	case 0:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - ((tm.tmAveCharWidth * lstrlen(congratulation) * 2) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent / 2), congratulation, lstrlen(congratulation));
		break;
	case 1:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(author1) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10), author1, lstrlen(author1));
		break;
	case 2:
	case 4:
		SetTextColor(hdc, RGB(0, color2, 0));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(author2) / 2), CLIENT_HEIGHT / 2 + tm.tmAscent, author2, lstrlen(author2));
		break;
	case 3:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(author3) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10), author3, lstrlen(author3));
		break;
	case 5:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(background) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10), background, lstrlen(background));
		break;
	case 6:
		SetTextColor(hdc, RGB(color2, 0, color2));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * (lstrlen(background2) + 2) / 2), (CLIENT_HEIGHT / 2) + tm.tmAscent, background2, lstrlen(background2));
		break;
	case 7:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(SE) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10), SE, lstrlen(SE));
		break;
	case 8:
		SetTextColor(hdc, RGB(color2, 0, 0));
		TextOut(hdc, (CLIENT_WIDTH / 2) - ((tm.tmAveCharWidth * (lstrlen(SE2) + 10)) / 2), CLIENT_HEIGHT / 2 + tm.tmAscent, SE2, lstrlen(SE2));
		break;
	case 9:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(BGM) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent + 10), BGM, lstrlen(BGM));
		break;
	case 10:
		SetTextColor(hdc, RGB(color2, 0, 0));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * (lstrlen(BGM2) + 5) / 2), (CLIENT_HEIGHT / 2) + tm.tmAscent, BGM2, lstrlen(BGM2));
		break;
	case 11:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - (tm.tmAveCharWidth * lstrlen(message1) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent / 2), message1, lstrlen(message1));
		break;
	case 12:
		Rectangle(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT);
		SetTextColor(hdc, RGB(0, 0, color));
		TextOut(hdc, (CLIENT_WIDTH / 2) - ((tm.tmAveCharWidth * (lstrlen(message2) + 9)) / 2), (CLIENT_HEIGHT / 2) - (tm.tmAscent / 2), message2, lstrlen(message2));
		break;
	}
}


/*********************************************************/
/*	�֐�:Clicked										 */
/*	�@�\:���b�Z�[�W����N���b�N���ꂽ����`����			 */
/*********************************************************/
void Game::Clicked(BYTE whitch)
{
	click_flg |= whitch;
}

/*********************************************************/
/*	�֐�:SetDisplay										 */
/*	�@�\:�`��Ɏg���֐���ݒ肷��						 */
/*********************************************************/
void Game::SetDisplay(enum DISP next_display)
{
	//�f�B�X�v���C�ύX
	now_display = next_display;
}

/*********************************************************/
/*	�֐�:GetDisplay										 */
/*	�@�\:���݂̉�ʂ��擾����							 */
/*********************************************************/
enum DISP Game::GetDisplay()
{
	return now_display;
}

/*********************************************************/
/*	�֐�:ShowDisplay									 */
/*	�@�\:���݂̉�ʂ�`�悷��							 */
/*********************************************************/
void Game::ShowDisplay(HDC &hdc, HWND &hWnd)
{
	switch(now_display)
	{
	case TITLE:
		Title(hdc, hWnd);
		break;
	case LOAD:
		Load(hdc, hWnd);
		break;
	case READY:
		Ready(hdc);
		break;
	case GAME:
		GameMain(hdc, hWnd);
		break;
	case STAGECLEAR:
		StageClear(hdc, hWnd);
		break;
	case GAMEOVER:
		GameOver(hdc);
		break;
	case CONTINUE:
		Continue(hdc, hWnd);
		break;
	case ALLCLEAR:
		AllClear(hdc);
	}

	//�t���O���Z�b�g
	click_flg &= 0x00;
}
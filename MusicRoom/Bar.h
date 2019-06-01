// include
#pragma once
#include "Main.h"

// const
const int BAR_HEIGHT = 64;

// �X�e�[�^�X�o�[
class Bar
{
private:

	// const
	const int mainRectWidth = 384;
	const int BAR_DRAW_STAYMSEC = 3500;
	const int DEFAULT_mainText_X = 200;
	const int DRAW_mainText_MOVE_X = 48;

	MusicData musicData;
	Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], originPrev[2], originNext[2], originShare[2], originBack[2], originGo[2];
	Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext, displayShare, displayBack, displayGo;
	Rect fieldRect;
	RoundRect mainRect;
	String mainText = U"", hoge = U"", albumName, albumDir;
	Font mainFont, timeFont;
	Audio dog;
	int64 musicSamplesPlayed = 0;
	bool draw_back_flag = false, draw_go_flag = false, draw_mainText_stayFlag = true, musicHasChangedFlag = false;
	int draw_mainText_startMSec, draw_mainText_stayMSec;
	double draw_mainText_x;

public:

	// ������
	Bar();

	// �X�V
	void update(GameData& getData, MyApp& manager);

	// �`��
	void draw(GameData getData);

	// �Ȗ��`��ʒu �X�V
	void Update_drawMainText();

	// �ȕύX����
	void changeMusic(GameData& getData, MyApp& manager, int t);
};

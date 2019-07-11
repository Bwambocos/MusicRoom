// include
#pragma once
#include "Main.h"

// const
const int barHeight = 64;

// �X�e�[�^�X�o�[
class Bar
{
private:

	// const
	const int mainRectWidth = 384;
	const int draw_stayMillisec = 3500;
	const int draw_mainTextDefaultX = 200;
	const int draw_mainTextMoveX = 48;

	MusicData musicData;
	Texture playImage_all[2], pauseImage_all[2], stopImage_all[2], seekImage_all[2], repImage_all[2], prevImage_all[2], nextImage_all[2], shareImage_all[2], backImage_all[2], goImage_all[2];
	Texture playImage_display, pauseImage_display, stopImage_display, seekImage_display, repImage_display, prevImage_display, nextImage_display, shareImage_display, backImage_display, goImage_display;
	Rect fieldRect;
	RoundRect mainRect;
	String mainText = U"", albumName, albumDir;
	Font mainFont;
	Audio dog;
	int64 musicSamplesPlayed = 0;
	Stopwatch draw_mainTextTime;
	bool draw_back_flag = false, draw_go_flag = false, draw_mainText_stayFlag = true, musicHasChangedFlag = false;
	double draw_mainText_x;

public:

	// ������
	Bar();

	// �X�V
	void update(GameData& getData, MyApp& manager);

	// �`��
	void draw(GameData getData);

	// �Ȗ��`��ʒu �X�V
	void draw_mainText_update();

	// �ȕύX����
	void changeMusic(GameData& getData, MyApp& manager, int t);
};

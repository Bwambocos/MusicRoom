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
	const int mainRectWidth = 450;
	const int buttonSize = 52;
	const int draw_stayMillisec = 3500;
	const int draw_mainTextMoveX = 48;

	Rect fieldRect;
	RoundRect mainRect;
	String mainText = U"", albumName, albumDir, musicName, musicDir;
	int64 musicSamplesPlayed = 0;
	Stopwatch draw_mainTextTime;
	Vec2 prevPos, playPos, repPos, stopPos, sharePos, nextPos, backPos, goPos;
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

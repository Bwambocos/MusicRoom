// include
#pragma once
#include "Main.h"

// const
const int barHeight = 64;

// ステータスバー
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

	// 初期化
	Bar();

	// 更新
	void update(GameData& getData, MyApp& manager);

	// 描画
	void draw(GameData getData);

	// 曲名描画位置 更新
	void draw_mainText_update();

	// 曲変更処理
	void changeMusic(GameData& getData, MyApp& manager, int t);
};

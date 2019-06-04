// include
#pragma once
#include "Main.h"

// const
const int BAR_HEIGHT = 64;

// ステータスバー
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

	// 初期化
	Bar();

	// 更新
	void update(GameData& getData, MyApp& manager);

	// 描画
	void draw(GameData getData);

	// 曲名描画位置 更新
	void Update_drawMainText();

	// 曲変更処理
	void changeMusic(GameData& getData, MyApp& manager, int t);
};

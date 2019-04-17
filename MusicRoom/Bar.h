// include
#pragma once
#include "Main.h"

// const
const int BAR_HEIGHT = 64;

// ステータスバー
class Bar : public MyApp::Scene
{
private:

	// const
	const int mainRectWidth = 384;
	const int BAR_DRAW_STAYMSEC = 3500;
	const int DEFAULT_mainText_X = 200;
	const int DRAW_mainText_MOVE_X = 48;

	struct Bar_Music
	{
		Audio music;
		String albumName, text;
		int nowTime, totalTime;
	};

	Bar_Music music;
	Texture originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], originPrev[2], originNext[2], originShare[2], originBack[2], originGo[2];
	Texture displayPlay, displayBrief, displayStop, displaySeek, displayRep, displayPrev, displayNext, displayShare, displayBack, displayGo;
	Rect fieldRect;
	RoundRect mainRect;
	Audio nowMusic, dog;
	String mainText = U"", hoge = U"";
	Font mainFont, timeFont;
	bool stop_flag = false, draw_back_flag = false, draw_go_flag = false, draw_mainText_stayFlag, musicLoopFlag = false;
	int draw_mainText_startMSec, draw_mainText_stayMSec;
	double draw_mainText_x;

public:

	// 初期化
	Bar(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// 再生中か取得
	bool is_nowMusicPlaying();

	// 曲詳細データ受け渡し
	void giveMusicData(String albumName, String musicName, Audio musicData);

	// 停止フラグ 設定
	void set_stopFlag(bool flag);

	// 曲名描画位置 更新
	void Update_drawMainText();
};

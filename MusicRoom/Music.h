// include
#pragma once
#include "Main.h"

// 曲詳細画面
class Music : public MyApp::Scene
{
private:

	// const
	const int draw_musicNameDefaultX = 33;
	const int draw_stayMillisec = 3500;
	const int draw_moveXPerSec = 48;

	// グローバル変数
	String albumName, albumDir, musicName, musicDir, musicComment, musicTotalTime;
	RoundRect musicNameRect, musicTimeRect, musicFavRect, musicSeekRect;
	Rect musicExplRect;
	FFTResult musicFFT;
	Stopwatch draw_musicNameTime;
	Vec2 playPos, stopPos, seekPos, repPos, favPos;
	double draw_musicNameX;
	bool draw_musicNameStayFlag, seekButtonFlag = false;

public:

	// 初期化
	Music(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// 曲名描画位置 更新
	void draw_musicName_update();
};

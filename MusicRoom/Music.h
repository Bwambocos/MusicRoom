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
	Texture backgroundImage, favedImage, notFavedImage, playImage_all[2], pauseImage_all[2], stopImage_all[2], seekImage_all[2], repImage_all[2], playImage_display, pauseImage_display, stopImage_display, seekImage_display, repImage_display;
	String albumName, albumDir, musicName, musicDir, musicComment, musicTotalTime;
	Font musicNameAndTimeFont, musicExplFont;
	RoundRect musicNameRect, musicTimeRect, musicFavRect, musicSeekRect, musicExplRect;
	FFTResult musicFFT;
	Stopwatch draw_musicNameTime;
	Array<String> musicComment_separeted;
	Audio tempAudio1; const Audio tempAudio2;
	double draw_musicNameX;
	bool draw_musicNameStayFlag, seekButtonFlag = false;

public:

	// 初期化
	Music(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// 曲説明 描画
	void musicExpl_draw() const;

	// 曲名描画位置 更新
	void draw_musicName_update();
};

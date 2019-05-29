// include
#pragma once
#include "Main.h"

// 曲詳細画面
class Music : public MyApp::Scene
{
private:

	// const
	const int DEFAULT_musicName_X = 33;
	const int DRAW_STAYMSEC = 3500;
	const int DRAW_MUSICNAME_MOVE_X = 48;

	// グローバル変数
	Texture music_Main, faved, not_faved, originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], displayPlay, displayBrief, displayStop, displaySeek, displayRep;
	String albumName, albumDir, musicName, musicDir, musicComment, musicTotalTime;
	Font music_NameTime, music_Exp;
	RoundRect rect_musicName, rect_musicTime, rect_music_isFav, rect_musicBar, rect_musicExp;
	FFTResult fft;
	int music_musicTime, draw_musicName_startMSec, draw_musicName_stayMSec, prev_or_next;
	double draw_musicName_x;
	bool draw_musicName_stayFlag, button_flag = false;

public:

	// 初期化
	Music(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// 曲説明 描画
	void musicExpl_Draw() const;

	// 曲名描画位置 更新
	void Update_drawMusicDetailStrings();

	// 他画面中フラグセット
	void set_stillFlag(bool flag);
};

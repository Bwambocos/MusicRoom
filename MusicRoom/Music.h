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
	String music_albumName = U"", music_albumBName = U"", music_musicName = U"", music_musicFileName = U"", music_musicExp = U"", music_musicLength = U"";
	Audio music_Music;
	Font music_NameTime, music_Exp;
	RoundRect rect_musicName, rect_musicTime, rect_music_isFav, rect_musicBar, rect_musicExp;
	FFTResult fft;
	int music_musicTime, draw_musicName_startMSec, draw_musicName_stayMSec, prev_or_next;
	double draw_musicName_x;
	bool draw_musicName_stayFlag, favLoop_flag = false, stop_flag = false, still_flag = true, button_flag = false, musicLoopFlag = false;

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

	// 曲手動遷移
	void changeMusic(int flag);

	// 曲操作
	// kind: 0->一時停止, 1->再生, 2->停止, 3->繰り返し切り替え
	void changeMusicStats(int kind);

	// 他画面中フラグセット
	void set_stillFlag(bool flag);

	// お気に入りループ中か否か
	bool isFavLooping();
};

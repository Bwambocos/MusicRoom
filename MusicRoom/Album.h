// include
#pragma once
#include "Main.h"
#include "Bar.h"

// アルバム詳細画面
class Album : public MyApp::Scene
{
private:

	// const
	const size_t albumListRows = 16;
	const int drawStayMillisec = 3500;
	const int draw_moveXPerSec = 48;
	const int draw_moveYPerSec = 12;

	// グローバル変数
	Rect albumImageRect, albumNameRect, albumCreatorRect, albumExplRect, albumList_FlagRect, albumList_NameRect, albumList_FavRect, albumListAllRect, audioWaveRect;
	Quad rectHeader;
	Stopwatch draw_albumNameTime, draw_albumCreatorTime, draw_albumExplTime;
	String albumName, albumDir, albumCreator, albumComment;
	Array<String> albumComment_Separeted;
	Vec2 goUpPos, goDownPos;
	FFTResult musicFFT;
	int MusicListFirstIndex;
	double draw_albumNameX, draw_albumCreatorX, draw_albumExpl_y;
	bool draw_albumNameStayFlag, draw_albumCreatorStayFlag, draw_albumExplStayFlag;

public:

	// 初期化
	Album(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// アルバム情報描画 更新
	void draw_albumDetails_update();

	// アルバム説明 描画
	void albumExpl_draw() const;

	// ボタン描画
	void drawButton(Rect rect, bool highlight) const;

	// 文字背景描画
	void drawStrBackground(Rect rect, String header, String icon) const;
};

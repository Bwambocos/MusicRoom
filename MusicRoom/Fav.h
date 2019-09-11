// include
#pragma once
#include "main.h"

// お気に入り画面
class Fav : public MyApp::Scene
{
private:

	// const
	const size_t albumListRows = 16;

	// グローバル変数
	Rect albumList_FlagRect, albumList_NameRect, albumList_FavRect, albumList_AllRect;
	Vec2 goUpPos, goDownPos;
	int FavMusicListFirstIndex;

public:

	// 初期化
	Fav(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// ボタン描画
	void drawButton(Rect rect, bool highlight) const;
};

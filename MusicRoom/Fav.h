// include
#pragma once
#include "main.h"

// お気に入り画面
class Fav : public MyApp::Scene
{
private:

	// const
	const int maxCellNum = 12;

	// グローバル変数
	Texture backgroundImage, playImage, pauseImage, notFavImage, favImage;
	Font albumListFont;
	RoundRect albumList_FlagRect, albumList_NameRect, albumList_TimeRect, albumList_FavRect, albumList_AllRect, albumList_CellRect;
	Triangle goUpButton, goDownButton;
	int FavMusicListFirstIndex;

public:

	// 初期化
	Fav(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// 曲名短縮
	String compressMusicName(String text) const;
};

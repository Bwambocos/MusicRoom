// include
#pragma once
#include "Main.h"

// お気に入り画面
class Fav : public MyApp::Scene
{
private:

	// const
	const int MAX_CELL_NUM = 12;

	// グローバル変数
	Texture main, playing, pausing, not_fav, fav;
	Font font_albumList;
	RoundRect rect_albumList_Flag, rect_albumList_Name, rect_albumList_Time, rect_albumList_Fav, rect_albumListAll, rect_albumListCell;
	Triangle goUp, goDown;
	int albumList_begin;

public:

	// 初期化
	Fav(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// 曲名短縮
	String Fav_musicNameBeShort(String text) const;

	// 曲リスト開始位置 操作
	void changeFavList_Begin();

	// お気に入りリスト 保存
	void saveFavList();
};

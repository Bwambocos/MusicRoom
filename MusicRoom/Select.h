// include
#pragma once
#include "Main.h"

// アルバム選択画面

class Select : public MyApp::Scene
{
private:

	// const
	const int commentMillisec = 1000;
	const int scrollMillisec = 500;

	std::vector<std::pair<int, int>>commentTime;
	Texture backgroundImage, noimgImage, favImage;
	Grid<double_t>albumGrid;
	Triangle goUpButton, goDownButton;
	Stopwatch scrollTime;
	int AlbumListFirstIndex = 0, scrollFlag = 0;
	double scrollAddY = 0;

public:

	// 初期化
	Select(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// アルバム一覧 正方形区画を作成
	Rect makeRect(int x, int y) const;

	// アルバム画像を返す
	Texture getSelectedImage(int cnt) const;

	// アルバム詳細 描画
	void drawDetails(int cnt) const;
};

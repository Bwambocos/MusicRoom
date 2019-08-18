// include
#pragma once
#include "Main.h"

// アルバム選択画面

class Select : public MyApp::Scene
{
private:

	// const
	const size_t albumGridWidth = 5;
	const int commentMillisec = 1000;
	const int scrollMillisec = 500;
	const int tileSize = 226;

	Grid<double_t>albumGrid;
	Array<std::pair<int, int>>commentTime;
	Stopwatch scrollTime;
	Vec2 goUpPos, goDownPos;
	int scrollFlag = 0;
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

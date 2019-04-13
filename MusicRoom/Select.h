// include
#pragma once
#include "Main.h"

// アルバム選択画面

// const
const int COM_MESSAGE_MILLISEC = 1000;
const int SCROLL_MSEC = 500;

class Select : public MyApp::Scene
{
private:

	// アルバム構造体
	struct Album
	{
		String name, dname, creator, comment;
		Texture image;
	};

	std::vector<std::pair<int, int>>comTime;
	std::vector<Album> AlbumList;
	Texture main, no_img, fav;
	String setAlbum, setAlbumB;
	Grid<double_t> z;
	Triangle goUp, goDown;
	int startTime, nowTime, first_cou = 0, selectedAlbumNum, scrollStartTime, scrollNowTime, scrollY = 0;
	bool scr_flag = false, scr_upflag;

public:

	Select(const InitData& init);
	void update() override;
	void draw() const override;

	// アルバム一覧 正方形区画を作成
	Rect MakeRect(int x, int y) const;

	// アルバム画像を返す
	Texture SelectImage(int cou) const;

	// 選択されたアルバム名を返す
	std::pair<String, String> getSetAlbum();

	// 次のアルバムを返す
	void getNextAlbum();

	// アルバム詳細 描画
	void DrawDetails(int cou) const;
};

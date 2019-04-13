// include
#pragma once
#include "Main.h"

// �A���o���I�����

// const
const int COM_MESSAGE_MILLISEC = 1000;
const int SCROLL_MSEC = 500;

class Select : public MyApp::Scene
{
private:

	// �A���o���\����
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

	// �A���o���ꗗ �����`�����쐬
	Rect MakeRect(int x, int y) const;

	// �A���o���摜��Ԃ�
	Texture SelectImage(int cou) const;

	// �I�����ꂽ�A���o������Ԃ�
	std::pair<String, String> getSetAlbum();

	// ���̃A���o����Ԃ�
	void getNextAlbum();

	// �A���o���ڍ� �`��
	void DrawDetails(int cou) const;
};

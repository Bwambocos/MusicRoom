// include
#pragma once
#include "Main.h"

// �A���o���I�����

class Select : public MyApp::Scene
{
private:

	// const
	const int COM_MESSAGE_MILLISEC = 1000;
	const int SCROLL_MSEC = 500;

	std::vector<std::pair<int, int>>comTime;
	Texture bgImage, no_img, fav;
	Grid<double_t>z;
	Triangle goUp, goDown;
	int startTime, nowTime, first_cnt = 0, scrollStartTime, scrollNowTime, scrollY = 0, scrollFlag = 0;

public:

	// ������
	Select(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �A���o���ꗗ �����`�����쐬
	Rect makeRect(int x, int y) const;

	// �A���o���摜��Ԃ�
	Texture getSelectedImage(int cnt) const;

	// �A���o���ڍ� �`��
	void drawDetails(int cnt) const;
};

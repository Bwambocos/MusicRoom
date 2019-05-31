// include
#pragma once
#include "Main.h"

// �A���o���I�����

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

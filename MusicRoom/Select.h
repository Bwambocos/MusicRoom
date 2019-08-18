// include
#pragma once
#include "Main.h"

// �A���o���I�����

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

// include
#pragma once
#include "main.h"

// ���C�ɓ�����
class Fav : public MyApp::Scene
{
private:

	// const
	const size_t albumListRows = 16;

	// �O���[�o���ϐ�
	Rect albumList_FlagRect, albumList_NameRect, albumList_FavRect, albumList_AllRect;
	Vec2 goUpPos, goDownPos;
	int FavMusicListFirstIndex;

public:

	// ������
	Fav(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �{�^���`��
	void drawButton(Rect rect, bool highlight) const;
};

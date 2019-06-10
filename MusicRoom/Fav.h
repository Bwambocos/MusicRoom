// include
#pragma once
#include "main.h"

// ���C�ɓ�����
class Fav : public MyApp::Scene
{
private:

	// const
	const int maxCellNum = 12;

	// �O���[�o���ϐ�
	Texture backgroundImage, playImage, pauseImage, notFavImage, favImage;
	Font albumListFont;
	RoundRect albumList_FlagRect, albumList_NameRect, albumList_TimeRect, albumList_FavRect, albumList_AllRect, albumList_CellRect;
	Triangle goUpButton, goDownButton;
	int FavMusicListFirstIndex;

public:

	// ������
	Fav(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �Ȗ��Z�k
	String compressMusicName(String text) const;
};

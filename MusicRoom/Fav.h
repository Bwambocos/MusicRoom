// include
#pragma once
#include "Main.h"

// ���C�ɓ�����
class Fav : public MyApp::Scene
{
private:

	// const
	const int MAX_CELL_NUM = 12;

	// �O���[�o���ϐ�
	Texture main, playing, pausing, not_fav, fav;
	Font font_albumList;
	RoundRect rect_albumList_Flag, rect_albumList_Name, rect_albumList_Time, rect_albumList_Fav, rect_albumListAll, rect_albumListCell;
	Triangle goUp, goDown;
	int albumList_begin;

public:

	// ������
	Fav(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �Ȗ��Z�k
	String Fav_musicNameBeShort(String text) const;

	// �ȃ��X�g�J�n�ʒu ����
	void changeFavList_Begin();

	// ���C�ɓ��胊�X�g �ۑ�
	void saveFavList();
};

// include
#pragma once
#include "Main.h"
#include "Bar.h"

// �A���o���ڍ׉��
class Album : public MyApp::Scene
{
private:

	// const
	const size_t albumListRows = 16;
	const int drawStayMillisec = 3500;
	const int draw_moveXPerSec = 48;
	const int draw_moveYPerSec = 12;

	// �O���[�o���ϐ�
	Rect albumImageRect, albumNameRect, albumCreatorRect, albumExplRect, albumList_FlagRect, albumList_NameRect, albumList_FavRect, albumListAllRect, audioWaveRect;
	Quad rectHeader;
	Stopwatch draw_albumNameTime, draw_albumCreatorTime, draw_albumExplTime;
	String albumName, albumDir, albumCreator, albumComment;
	Array<String> albumComment_Separeted;
	Vec2 goUpPos, goDownPos;
	FFTResult musicFFT;
	int MusicListFirstIndex;
	double draw_albumNameX, draw_albumCreatorX, draw_albumExpl_y;
	bool draw_albumNameStayFlag, draw_albumCreatorStayFlag, draw_albumExplStayFlag;

public:

	// ������
	Album(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �A���o�����`�� �X�V
	void draw_albumDetails_update();

	// �A���o������ �`��
	void albumExpl_draw() const;

	// �{�^���`��
	void drawButton(Rect rect, bool highlight) const;

	// �����w�i�`��
	void drawStrBackground(Rect rect, String header, String icon) const;
};

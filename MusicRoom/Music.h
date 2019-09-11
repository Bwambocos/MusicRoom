// include
#pragma once
#include "Main.h"

// �ȏڍ׉��
class Music : public MyApp::Scene
{
private:

	// const
	const int draw_stayMillisec = 3500;
	const int draw_moveXPerSec = 48;

	// �O���[�o���ϐ�
	String albumName, albumDir, albumCreator, albumExpl, musicName, musicDir, musicComment;
	Rect albumImageRect, albumNameRect, albumCreatorRect, albumExplRect, musicNameRect, musicExplRect;
	RoundRect musicSeekRect;
	Quad rectHeader;
	FFTResult musicFFT;
	Stopwatch draw_musicNameTime;
	Vec2 playPos, stopPos, seekPos, repPos, favPos;
	double draw_musicNameX;
	bool draw_musicNameStayFlag, seekButtonFlag = false;

public:

	// ������
	Music(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �Ȗ��`��ʒu �X�V
	void draw_musicName_update();

	// �����w�i�`��
	void drawStrBackground(Rect rect, String header, String icon) const;
};

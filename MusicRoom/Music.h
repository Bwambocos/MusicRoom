// include
#pragma once
#include "Main.h"

// �ȏڍ׉��
class Music : public MyApp::Scene
{
private:

	// const
	const int draw_musicNameDefaultX = 33;
	const int draw_stayMillisec = 3500;
	const int draw_moveXPerSec = 48;

	// �O���[�o���ϐ�
	String albumName, albumDir, musicName, musicDir, musicComment, musicTotalTime;
	RoundRect musicNameRect, musicTimeRect, musicFavRect, musicSeekRect;
	Rect musicExplRect;
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
};

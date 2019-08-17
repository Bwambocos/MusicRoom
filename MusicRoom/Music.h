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
	Texture backgroundImage, favedImage, notFavedImage, playImage_all[2], pauseImage_all[2], stopImage_all[2], seekImage_all[2], repImage_all[2], playImage_display, pauseImage_display, stopImage_display, seekImage_display, repImage_display;
	String albumName, albumDir, musicName, musicDir, musicComment, musicTotalTime;
	Font musicNameAndTimeFont, musicExplFont;
	RoundRect musicNameRect, musicTimeRect, musicFavRect, musicSeekRect, musicExplRect;
	FFTResult musicFFT;
	Stopwatch draw_musicNameTime;
	Array<String> musicComment_separeted;
	Audio tempAudio1; const Audio tempAudio2;
	double draw_musicNameX;
	bool draw_musicNameStayFlag, seekButtonFlag = false;

public:

	// ������
	Music(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �Ȑ��� �`��
	void musicExpl_draw() const;

	// �Ȗ��`��ʒu �X�V
	void draw_musicName_update();
};

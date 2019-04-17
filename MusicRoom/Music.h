// include
#pragma once
#include "Main.h"

// �ȏڍ׉��
class Music : public MyApp::Scene
{
private:

	// const
	const int DEFAULT_musicName_X = 33;
	const int DRAW_STAYMSEC = 3500;
	const int DRAW_MUSICNAME_MOVE_X = 48;

	// �O���[�o���ϐ�
	Texture music_Main, faved, not_faved, originPlay[2], originBrief[2], originStop[2], originSeek[2], originRep[2], displayPlay, displayBrief, displayStop, displaySeek, displayRep;
	String music_albumName = U"", music_albumBName = U"", music_musicName = U"", music_musicFileName = U"", music_musicExp = U"", music_musicLength = U"";
	Audio music_Music;
	Font music_NameTime, music_Exp;
	RoundRect rect_musicName, rect_musicTime, rect_music_isFav, rect_musicBar, rect_musicExp;
	FFTResult fft;
	int music_musicTime, draw_musicName_startMSec, draw_musicName_stayMSec, prev_or_next;
	double draw_musicName_x;
	bool draw_musicName_stayFlag, favLoop_flag = false, stop_flag = false, still_flag = true, button_flag = false, musicLoopFlag = false;

public:

	// ������
	Music(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �Ȑ��� �`��
	void musicExpl_Draw() const;

	// �Ȗ��`��ʒu �X�V
	void Update_drawMusicDetailStrings();

	// �Ȏ蓮�J��
	void changeMusic(int flag);

	// �ȑ���
	// kind: 0->�ꎞ��~, 1->�Đ�, 2->��~, 3->�J��Ԃ��؂�ւ�
	void changeMusicStats(int kind);

	// ����ʒ��t���O�Z�b�g
	void set_stillFlag(bool flag);

	// ���C�ɓ��胋�[�v�����ۂ�
	bool isFavLooping();
};

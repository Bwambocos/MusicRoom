// include
#pragma once
#include "Main.h"
#include "Bar.h"

// �A���o���ڍ׉��
class Album : public MyApp::Scene
{
private:

	// const
	const int draw_albumNameDefaultX = 333;
	const int draw_albumCreatorDefaultX = 333;
	const int draw_albumExplDefaultX = 143 + BAR_HEIGHT;
	const int drawStayMillisec = 3500;
	const int draw_moveXPerSec = 48;
	const int draw_moveYPerSec = 12;

	// �O���[�o���ϐ�
	Texture backgroundImage, playImage, pauseImage, notFavImage, favImage, albumImage;
	Font albumNameFont, albumCreatorFont, albumExplFont, albumListFont;
	RoundRect albumImageRRect, albumNameRRect, albumCreatorRRect, albumExplRRect, albumList_FlagRRect, albumList_NameRRect, albumList_TimeRRect, albumList_FavRRect, albumListAllRRect, albumListCellRRect;
	Triangle goUpButton, goDownButton;
	Stopwatch draw_albumNameTime, draw_albumCreatorTime, draw_albumExplTime;
	String albumName, albumDir, albumCreator, albumComment;
	Array<String> albumComment_Separeted;
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

	// �Ȗ��Z�k
	String Album_compressMusicName(String text) const;
};

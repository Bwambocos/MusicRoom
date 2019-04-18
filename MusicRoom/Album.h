// include
#pragma once
#include "Main.h"
#include "Bar.h"

// �A���o���ڍ׉��
class Album : public MyApp::Scene
{
private:

	// const
	const int DEFAULT_albumName_X = 333;
	const int DEFAULT_albumCreator_X = 333;
	const int DEFAULT_albumExpl_Y = 143 + BAR_HEIGHT;
	const int DRAW_STAYMSEC = 3500;
	const int DRAW_MOVE_X_PER_SEC = 48;
	const int DRAW_MOVE_Y_PER_SEC = 12;


	// �O���[�o���ϐ�
	std::map<String, std::vector<MusicData>>albums;
	Array<String> texts;
	Texture main, playing, pausing, not_fav, fav, albumImg;
	Font font_albumName, font_albumCreator, font_albumExpl, font_albumList;
	String albumName = U"", albumBName = U"", albumCreator = U"", albumExpl = U"";
	RoundRect rect_albumImage, rect_albumName, rect_albumCreator, rect_albumExpl, rect_albumList_Flag, rect_albumList_Name, rect_albumList_Time, rect_albumList_Fav, rect_albumListAll, rect_albumListCell;
	Triangle goUp, goDown;
	int64 draw_albumName_startMSec, draw_albumCreator_startMSec, draw_albumExpl_startMSec, draw_albumName_stayMSec, draw_albumCreator_stayMSec, draw_albumExpl_stayMSec;
	int albumList_begin, selectedMusic_num;
	double draw_albumName_x, draw_albumCreator_x, draw_albumExpl_y;
	bool draw_albumName_stayFlag, draw_albumCreator_stayFlag, draw_albumExpl_stayFlag, reloadFlag = false;

public:

	// ������
	Album(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// �A���o������ �X�V
	void albumExpl_Update();

	// �A���o������ �`��
	void albumExpl_Draw() const;

	// �e������ �`��
	void Update_drawAlbumDetailStrings();

	// �Ȗ��Z�k
	String Detail_musicNameBeShort(String text);
};

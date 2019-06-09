// include
#pragma once
#include "Main.h"
#include "Bar.h"

// アルバム詳細画面
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

	// グローバル変数
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

	// 初期化
	Album(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// アルバム情報描画 更新
	void draw_albumDetails_update();

	// アルバム説明 描画
	void albumExpl_draw() const;

	// 曲名短縮
	String Album_compressMusicName(String text) const;
};

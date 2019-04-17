// include
#pragma once
#include "Main.h"

// お気に入り画面
class Fav : public MyApp::Scene
{
private:

	// const
	const int MAX_CELL_NUM = 12;

	// 曲リスト 構造体
	struct List_fav
	{
		Audio music;
		String albumName, albumBName, musicOriginName, musicBName;
		int totalTime;
	};

	// グローバル変数
	std::vector<List_fav>musics;
	Texture main, playing, pausing, not_fav, fav;
	Font font_albumList;
	String selectedAlbumName = U"", selectedAlbumBName = U"", selectedMusicName = U"", selectedMusicBName = U"";
	RoundRect rect_albumList_Flag, rect_albumList_Name, rect_albumList_Time, rect_albumList_Fav, rect_albumListAll, rect_albumListCell;
	Triangle goUp, goDown;
	Audio selectedMusic;
	int albumList_begin, selectedMusic_num;
	bool musicLoopFlag = false;

public:

	// 初期化
	Fav(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// お気に入りか確認する
	bool isFav(String albumName, String musicName) const;

	// お気に入りに追加する
	void addFav(String albumName, String albumBName, String musicName, String fileName, Audio music);

	// お気に入りから削除する
	void removeFav(String albumName, String musicName);

	// 曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
	void setFavMusicName(String& album_Name, String& album_BName, String& musicName, Audio& music);
	void setFavMusicName(int flag, String& album_Name, String& album_BName, String& musicName, Audio& music);

	// 曲名短縮
	String Fav_musicNameBeShort(String text) const;

	// 曲リスト開始位置 操作
	void changeFavList_Begin();

	// お気に入りリスト 保存
	void saveFavList();

	// 曲操作
	// kind: 0->一時停止, 1->再生, 2->停止, 3->繰り返し切り替え
	void changeFavMusicStats(int kind);
};

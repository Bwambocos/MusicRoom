// include
#pragma once
#include "Main.h"

// ���C�ɓ�����
class Fav : public MyApp::Scene
{
private:

	// const
	const int MAX_CELL_NUM = 12;

	// �ȃ��X�g �\����
	struct List_fav
	{
		Audio music;
		String albumName, albumBName, musicOriginName, musicBName;
		int totalTime;
	};

	// �O���[�o���ϐ�
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

	// ������
	Fav(const InitData& init);

	// �X�V
	void update() override;

	// �`��
	void draw() const override;

	// ���C�ɓ��肩�m�F����
	bool isFav(String albumName, String musicName) const;

	// ���C�ɓ���ɒǉ�����
	void addFav(String albumName, String albumBName, String musicName, String fileName, Audio music);

	// ���C�ɓ��肩��폜����
	void removeFav(String albumName, String musicName);

	// �ȏ�� �󂯓n���iflag == 1 -> �� : -1 -> �O�j
	void setFavMusicName(String& album_Name, String& album_BName, String& musicName, Audio& music);
	void setFavMusicName(int flag, String& album_Name, String& album_BName, String& musicName, Audio& music);

	// �Ȗ��Z�k
	String Fav_musicNameBeShort(String text) const;

	// �ȃ��X�g�J�n�ʒu ����
	void changeFavList_Begin();

	// ���C�ɓ��胊�X�g �ۑ�
	void saveFavList();

	// �ȑ���
	// kind: 0->�ꎞ��~, 1->�Đ�, 2->��~, 3->�J��Ԃ��؂�ւ�
	void changeFavMusicStats(int kind);
};

// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

// �Q�[���̊�{���
namespace GameInfo
{
	const int32 Width = 1280;
	const int32 Height = 720;
	const String Title = U"MusicRoom v5.0";
	const int32 FadeInTime = 500;
	const bool FadeCrossFlag = true;
}

// �A���o���\����
struct AlbumData
{
	String name, dir, creator, comment;
};

// �ȍ\����
struct MusicData
{
	String name, dir, comment;
};

// ���C�ɓ���ȍ\����
struct FavMusicData
{
	String album_name, album_dir, album_creator, album_comment, music_name, music_dir, comment;
};

// �S�ẴV�[������A�N�Z�X�ł���f�[�^
struct GameData
{
	String prevScene, nowScene;

	std::vector<AlbumData> AlbumList;
	int selectedAlbumIndex = -1, AlbumListFirstIndex = 0;

	std::map<String, std::vector<MusicData>> MusicList;
	int selectedMusicIndex = -1;
	bool selectedMusicLoopFlag;

	std::vector<FavMusicData> FavMusicList;
	int selectedFavMusicIndex = -1;

	// ���C�ɓ��肩�m�F����
	bool isFav(String albumName, String musicName) const
	{
		for (auto i : FavMusicList) if (i.album_name == albumName && i.music_name == musicName) return true;
		return false;
	}

	// ���C�ɓ���ɒǉ�����
	void addFav(String albumName, String albumDir, String albumCreator, String albumComment, String musicName, String musicDir, String musicComment)
	{
		FavMusicList.push_back({ albumName, albumDir,albumCreator, albumComment,musicName, musicDir, musicComment });
	}

	// ���C�ɓ��肩��폜����
	void removeFav(String albumName, String musicName)
	{
		for (int i = 0; i < (signed)FavMusicList.size(); ++i)
		{
			if (FavMusicList[i].album_name == albumName && FavMusicList[i].music_name == musicName)
			{
				if (i == selectedFavMusicIndex) selectedFavMusicIndex = -1;
				FavMusicList.erase(FavMusicList.begin() + i);
				break;
			}
		}
	}

	int drawMode = 0;
	Color schemeColor1 = Color(22, 22, 22);
	Color schemeColor2 = Color(66, 66, 66);
	Color schemeColor3 = Color(110, 110, 110);
	Color schemeColor4 = Color(154, 154, 154);
	Color schemeColor5 = Color(198, 198, 198);
	Color stringColor = Color(Palette::White);
};

using MyApp = SceneManager<String, GameData>;

// ���C�ɓ��胊�X�g �Ǎ�
void loadFavList(GameData& getData);

// ���C�ɓ��胊�X�g �ۑ�
void saveFavList(GameData& getData);

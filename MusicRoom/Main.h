// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

// �Q�[���̊�{���
namespace GameInfo
{
	const int32 Width = 768;
	const int32 Height = 512;
	const String Title = U"MusicRoom v3.0";
	const ColorF FadeInColor = ColorF(0.0, 0.0, 0.0);
	const int32 FadeInTime = 500;
	const bool FadeCrossFlag = true;
}

// �A���o���\����
struct AlbumData
{
	String name, dir, creator, comment;
	Texture image;
};

// �ȍ\����
struct MusicData
{
	Audio music;
	String name, dir, comment, compressedName;
	int totalTime;
};

// ���C�ɓ���ȍ\����
struct FavMusicData
{
	Audio music;
	String album_name, album_dir, music_name, music_dir, comment, music_compressedName;
	int totalTime;
};

// �S�ẴV�[������A�N�Z�X�ł���f�[�^
struct GameData
{
	String prevScene, nowScene;

	std::vector<AlbumData> AlbumList;
	int selectedAlbumIndex = -1;

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
	void addFav(String albumName, String albumDir, String musicName, String musicDir, String musicComment, Audio music)
	{
		FavMusicList.push_back({ music, albumName, albumDir, musicName, musicDir, musicComment, compressMusicName(musicName), (int)music.lengthSec() });
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

	// �Ȗ��Z�k
	String compressMusicName(String text) const
	{
		const String dots(U"...");
		size_t fixedLength = 0;
		while (Font(16)(text.substr(0, fixedLength) + dots).region().w < 537 && fixedLength <= text.length()) ++fixedLength;
		--fixedLength;
		return (fixedLength == text.length() ? text : text.substr(0, fixedLength) + dots);
	}
};

using MyApp = SceneManager<String, GameData>;

// ���C�ɓ��胊�X�g �Ǎ�
void loadFavList(GameData& getData);

// ���C�ɓ��胊�X�g �ۑ�
void saveFavList(GameData& getData);

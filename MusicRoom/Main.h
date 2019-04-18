// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

// �A���o���\����
struct AlbumData
{
	String name, dname, creator, comment;
	Texture image;
};

// �S�ẴV�[������A�N�Z�X�ł���f�[�^
struct GameData
{
	String prevScene;

	std::vector<AlbumData> AlbumList;
	String selectedAlbumName, selectedAlbumDir;
	int selectedAlbumIndex;

	String selectedMusicName, selectedMusicDir;
	Audio selectedMusic;
};

using MyApp = SceneManager<String, GameData>;

// �Q�[���̊�{���
namespace GameInfo
{
	const int32 Width = 768;
	const int32 Height = 512;
	const String Title = U"MusicRoom v3.0";
	const ColorF BackgroundColor = ColorF(0.4, 0.7, 0.5);
	const ColorF FadeInColor = ColorF(1.0, 1.0, 1.0);
}

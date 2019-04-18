// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

using MyApp = SceneManager<String, GameData>;
MyApp manager;

// �Q�[���̊�{���
namespace GameInfo
{
	const int32 Width = 768;
	const int32 Height = 512;
	const String Title = U"MusicRoom v3.0";
	const ColorF BackgroundColor = ColorF(0.4, 0.7, 0.5);
	const ColorF FadeInColor = ColorF(1.0, 1.0, 1.0);
}

// �A���o���\����
struct AlbumData
{
	String name, dname, creator, comment;
	Texture image;
};

// �ȍ\����
struct MusicData
{
	Audio music;
	String displayName, originName, fileName;
	int totalTime;
};

// �S�ẴV�[������A�N�Z�X�ł���f�[�^
struct GameData
{
	String prevScene;

	std::vector<AlbumData> AlbumList;
	String selectedAlbumName, selectedAlbumDir;
	int selectedAlbumIndex;

	std::vector<MusicData> MusicList;
	String selectedMusicName, selectedMusicDir;
	Audio selectedMusic;
	int selectedMusicIndex;

	// �A���o���E�ȏ�� �󂯓n���iflag == 1 -> �� : -1 -> �O�j
	void setAlbumMusicName(String& album_Name, String& album_BName, String& musicName, Audio& musicData)
	{
		album_Name = selectedAlbumName;
		album_BName = selectedAlbumDir;
		musicName = selectedMusicName;
		musicData = selectedMusic;
	};
	void setAlbumMusicName(int flag, String& album_Name, String& album_BName, String& musicName, Audio& music)
	{
		if (selectedMusicIndex + flag >= (int)MusicList.size())
		{
			++selectedAlbumIndex;
			selectedAlbumName = AlbumList[selectedAlbumIndex % AlbumList.size()].name;
			selectedAlbumDir = AlbumList[selectedAlbumIndex % AlbumList.size()].dname;
			selectedMusicIndex = -1;
			manager.changeScene(U"Album", 1000, false);
		}
		selectedMusicIndex = (selectedMusicIndex + flag + (int)MusicList.size()) % (int)MusicList.size();
		const auto data = MusicList[selectedMusicIndex];
		album_Name = selectedAlbumName;
		album_BName = selectedAlbumDir;
		musicName = selectedMusicDir = data.fileName;
		music = selectedMusic = data.music;
	};

	// �ȑ���
	// kind: 0->�ꎞ��~, 1->�Đ�, 2->��~
	void setMusicStats(int kind)
	{
		if (MusicList.empty()) return;
		switch (kind)
		{
		case 0:
			MusicList[selectedMusicIndex].music.pause();
			break;
		case 1:
			MusicList[selectedMusicIndex].music.play();
			break;
		case 2:
			MusicList[selectedMusicIndex].music.stop();
			break;
		}
	};
};

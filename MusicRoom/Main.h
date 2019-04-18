// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

using MyApp = SceneManager<String, GameData>;
MyApp manager;

// ゲームの基本情報
namespace GameInfo
{
	const int32 Width = 768;
	const int32 Height = 512;
	const String Title = U"MusicRoom v3.0";
	const ColorF BackgroundColor = ColorF(0.4, 0.7, 0.5);
	const ColorF FadeInColor = ColorF(1.0, 1.0, 1.0);
}

// アルバム構造体
struct AlbumData
{
	String name, dname, creator, comment;
	Texture image;
};

// 曲構造体
struct MusicData
{
	Audio music;
	String displayName, originName, fileName;
	int totalTime;
};

// 全てのシーンからアクセスできるデータ
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

	// アルバム・曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
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

	// 曲操作
	// kind: 0->一時停止, 1->再生, 2->停止
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

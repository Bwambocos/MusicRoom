// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

// ゲームの基本情報
namespace GameInfo
{
	const int32 Width = 1280;
	const int32 Height = 720;
	const String Title = U"MusicRoom v5.0";
	const int32 FadeInTime = 500;
	const bool FadeCrossFlag = true;
}

// アルバム構造体
struct AlbumData
{
	String name, dir, creator, comment;
};

// 曲構造体
struct MusicData
{
	String name, dir, comment;
};

// お気に入り曲構造体
struct FavMusicData
{
	String album_name, album_dir, album_creator, album_comment, music_name, music_dir, comment;
};

// 全てのシーンからアクセスできるデータ
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

	// お気に入りか確認する
	bool isFav(String albumName, String musicName) const
	{
		for (auto i : FavMusicList) if (i.album_name == albumName && i.music_name == musicName) return true;
		return false;
	}

	// お気に入りに追加する
	void addFav(String albumName, String albumDir, String albumCreator, String albumComment, String musicName, String musicDir, String musicComment)
	{
		FavMusicList.push_back({ albumName, albumDir,albumCreator, albumComment,musicName, musicDir, musicComment });
	}

	// お気に入りから削除する
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

// お気に入りリスト 読込
void loadFavList(GameData& getData);

// お気に入りリスト 保存
void saveFavList(GameData& getData);

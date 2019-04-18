// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"
#include "Fav.h"

// お気に入り画面

// 初期化
Fav::Fav(const InitData& init) : IScene(init)
{
	if (!main)
	{
		main = Texture(U"data\\Fav\\main.png");
		playing = Texture(U"data\\Detail\\playing.png");
		pausing = Texture(U"data\\Detail\\pausing.png");
		not_fav = Texture(U"data\\Detail\\not_fav.png");
		fav = Texture(U"data\\Detail\\fav.png");
		rect_albumList_Flag = RoundRect(25, 25 + BAR_HEIGHT, 36, 36, 5);
		rect_albumList_Name = RoundRect(64, 25 + BAR_HEIGHT, 537, 36, 5);
		rect_albumList_Time = RoundRect(604, 25 + BAR_HEIGHT, 100, 36, 5);
		rect_albumList_Fav = RoundRect(707, 25 + BAR_HEIGHT, 36, 36, 5);
		rect_albumListAll = RoundRect(25, 25 + BAR_HEIGHT, 718, 456, 5);
		rect_albumListCell = RoundRect(64, 25 + BAR_HEIGHT, 582, 36, 5);
		goUp = Triangle({ 384,75 }, { 414,85 }, { 354,85 });
		goDown = Triangle({ 354,560 }, { 414,560 }, { 384,570 });
		font_albumList = Font(16);
		CSVData csv(U"data\\Fav\\list.csv");
		if (csv)
		{
			for (int i = 0; i < (signed)csv.rows; ++i)
			{
				auto albumDName = csv.get<String>(i, 0);
				auto musicDName = csv.get<String>(i, 1);
				String albumName, musicName;
				Audio music;
				TextReader albumTextReader(U"music\\" + albumDName + U"\\" + albumDName + U".txt");
				TextReader musicTextReader(U"music\\" + albumDName + U"\\" + musicDName + U"\\" + musicDName + U".txt");
				albumTextReader.readLine(albumName);
				musicTextReader.readLine(musicName);
				const String extensions[] = { U".wav",U".ogg",U".mp3" };
				for (auto ext : extensions)
				{
					if (FileSystem::IsFile(U"music\\" + albumDName + U"\\" + musicDName + U"\\" + musicDName + ext))
					{
						music = Audio(U"music\\" + albumDName + U"\\" + musicDName + U"\\" + musicDName + ext);
						break;
					}
				}
				addFav(albumName, albumDName, musicName, musicDName, music);
			}
		}
	}
	if (get_prevScene() != Scene_Music) albumList_begin = 0;
}

// 更新
void Fav::update()
{
	if (KeyB.pressed()) changeScene(U"Album");

	// 曲リスト 更新
	{
		if (goUp.leftClicked()) --albumList_begin;
		if (goDown.leftClicked()) ++albumList_begin;
		if (rect_albumListAll.mouseOver())
		{
			if ((albumList_begin + MAX_CELL_NUM <= (signed)musics.size()) || (albumList_begin > 0)) albumList_begin += Mouse::Wheel();
		}
		albumList_begin = Max(albumList_begin, 0);
		albumList_begin = Min<int>(albumList_begin, Max<int>((int)musics.size() - MAX_CELL_NUM, 0));

		for (int i = albumList_begin; ((i - albumList_begin) < Min<int>(MAX_CELL_NUM, (signed)musics.size())) && (i < (signed)musics.size()); ++i)
		{
			auto num = i - albumList_begin;
			auto music = musics[i];
			RoundRect rect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
			if (rect.leftClicked)
			{
				(music.music.isPlaying() ? music.music.pause() : music.music.play());
				if (selectedMusic_num != i) musics[selectedMusic_num].music.stop();
				selectedMusic_num = i;
				selectedAlbumName = music.albumName;
				selectedAlbumBName = music.albumBName;
				selectedMusicName = music.musicOriginName;
				selectedMusicBName = music.musicBName;
				selectedMusic = music.music;
				giveMusicData(music.albumName, music.musicOriginName, music.music);
			}
			rect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
			if (rect.leftClicked)
			{
				(isFav(music.albumName, music.musicOriginName) ? removeFav(music.albumName, music.musicOriginName) : addFav(music.albumName, music.albumBName, music.musicOriginName, music.musicBName, music.music));
			}
			rect = RoundRect(rect_albumListCell.x, rect_albumListCell.y + num * 39, rect_albumListCell.w, rect_albumListCell.h, rect_albumListCell.r);
			if (rect.leftClicked)
			{
				if (selectedMusic_num != i) musics[selectedMusic_num].music.stop();
				selectedMusic_num = i;
				selectedAlbumName = music.albumName;
				selectedAlbumBName = music.albumBName;
				selectedMusicName = music.musicOriginName;
				selectedMusicBName = music.musicBName;
				selectedMusic = music.music;
				set_stillFlag(true);
				changeScene(U"Music");
			}
		}
	}
}

// 描画
void Fav::draw() const
{
	main.draw(0, BAR_HEIGHT);

	// 曲リスト 描画
	if (albumList_begin > 0)
	{
		goUp.draw((goUp.mouseOver ? Palette::Orange : Palette::White));
		goUp.drawFrame(2, Palette::Black);
	}
	if (albumList_begin + MAX_CELL_NUM < (signed)musics.size())
	{
		goDown.draw((goDown.mouseOver ? Palette::Orange : Palette::White));
		goDown.drawFrame(2, Palette::Black);
	}
	for (int i = 0; i < MAX_CELL_NUM; ++i)
	{
		RoundRect(rect_albumList_Flag.x, rect_albumList_Flag.y + i * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Name.x, rect_albumList_Name.y + i * 39, rect_albumList_Name.w, rect_albumList_Name.h, rect_albumList_Name.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Time.x, rect_albumList_Time.y + i * 39, rect_albumList_Time.w, rect_albumList_Time.h, rect_albumList_Time.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + i * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r).draw(Color(32, 32, 32, 200));
	}
	for (int i = albumList_begin; (i - albumList_begin) < Min<int>(MAX_CELL_NUM, (int)musics.size() - albumList_begin); ++i)
	{
		auto num = i - albumList_begin;
		auto tmp = musics[i];
		RoundRect tmpRRect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
		if (tmp.music.isPlaying()) pausing.drawAt(43, 43 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White));
		else playing.drawAt(43, 43 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White));
		font_albumList(Fav_musicNameBeShort(tmp.musicOriginName)).draw(70, 29 + BAR_HEIGHT + num * 39);
		auto str = Format(Pad(tmp.totalTime / 60, { 2,L'0' }), L":", Pad(tmp.totalTime % 60, { 2,L'0' }));
		font_albumList(str).draw(610, 29 + BAR_HEIGHT + num * 39);
		tmpRRect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
		((isFav(tmp.albumName, tmp.musicOriginName) || tmpRRect.mouseOver) ? fav : not_fav).drawAt(725, 43 + BAR_HEIGHT + num * 39);
	}
}

// お気に入りか確認する
bool Fav::isFav(String albumName, String musicName) const
{
	for (auto i : musics)
	{
		if (i.albumName == albumName && i.musicOriginName == musicName) return true;
	}
	return false;
}

// お気に入りに追加する
void Fav::addFav(String albumName, String albumBName, String musicName, String fileName, Audio music)
{
	auto temp_totalTime = (int)music.lengthSec();
	musics.push_back({ music,albumName,albumBName,musicName,fileName,temp_totalTime });
}

// お気に入りから削除する
void Fav::removeFav(String albumName, String musicName)
{
	for (int i = 0; i < (signed)musics.size(); ++i)
	{
		if (musics[i].albumName == albumName && musics[i].musicOriginName == musicName)
		{
			musics.erase(musics.begin() + i);
			break;
		}
	}
}

// 曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
void Fav::setFavMusicName(String & album_Name, String & album_BName, String & musicName, Audio & music)
{
	const auto data = musics[selectedMusic_num];
	selectedAlbumName = data.albumName;
	selectedAlbumBName = data.albumBName;
	selectedMusicName = data.musicOriginName;
	selectedMusicBName = data.musicBName;
	album_Name = selectedAlbumName;
	musicName = selectedMusicBName;
	album_BName = selectedAlbumBName;
	music = selectedMusic;
}
void Fav::setFavMusicName(int flag, String & album_Name, String & album_BName, String & musicName, Audio & music)
{
	selectedMusic_num = (selectedMusic_num + flag + (int)musics.size()) % (int)musics.size();
	const auto data = musics[selectedMusic_num];
	album_Name = data.albumName;
	album_BName = data.albumBName;
	musicName = data.musicBName;
	music = data.music;
	selectedAlbumName = data.albumName;
	selectedAlbumBName = data.albumBName;
	selectedMusicName = data.musicOriginName;
	selectedMusicBName = data.musicBName;
	changeFavList_Begin();
}

// 曲名短縮
String Fav::Fav_musicNameBeShort(String text) const
{
	static const String dots(U"...");
	const double dotsWidth = font_albumList(dots).region().w;
	// const size_t num_chars = font_albumList.drawableCharacters(text, rect_albumList_Name.w - dotsWidth);
	const size_t num_chars = 25;

	if (font_albumList(text).region().w <= rect_albumList_Name.w) return text;
	if (dotsWidth > rect_albumList_Name.w) return String();
	return text.substr(0, num_chars) + dots;
}

// 曲リスト開始位置 操作
void Fav::changeFavList_Begin()
{
	albumList_begin = Max(albumList_begin, selectedMusic_num - 4);
	albumList_begin = Min(albumList_begin, selectedMusic_num);
}

// お気に入りリスト 保存
void Fav::saveFavList()
{
	CSVData csv(U"data\\Fav\\list.csv");
	for (auto i : musics)
	{
		csv.writeRow(i.albumBName, i.musicBName);
	}
}

// 曲操作
// kind: 0->一時停止, 1->再生, 2->停止, 3->繰り返し切り替え
void Fav::changeFavMusicStats(int kind)
{
	switch (kind)
	{
	case 0:
		selectedMusic.pause();
		break;
	case 1:
		selectedMusic.play();
		break;
	case 2:
		selectedMusic.stop();
		break;
	case 3:
		const int tmpTime = (int)selectedMusic.streamPosSample();
		selectedMusic.pause();
		selectedMusic.setLoop(!musicLoopFlag);
		musicLoopFlag = !musicLoopFlag;
		selectedMusic.play();
		selectedMusic.setPosSample(tmpTime);
		break;
	}
}

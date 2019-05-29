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
		main = Texture(U"data\\bgImage.png");
		playing = Texture(U"data\\Album\\playing.png");
		pausing = Texture(U"data\\Album\\pausing.png");
		not_fav = Texture(U"data\\Album\\not_fav.png");
		fav = Texture(U"data\\Album\\fav.png");
		font_albumList = Font(16);
		rect_albumList_Flag = RoundRect(25, 25 + BAR_HEIGHT, 36, 36, 5);
		rect_albumList_Name = RoundRect(64, 25 + BAR_HEIGHT, 537, 36, 5);
		rect_albumList_Time = RoundRect(604, 25 + BAR_HEIGHT, 100, 36, 5);
		rect_albumList_Fav = RoundRect(707, 25 + BAR_HEIGHT, 36, 36, 5);
		rect_albumListAll = RoundRect(25, 25 + BAR_HEIGHT, 718, 456, 5);
		rect_albumListCell = RoundRect(64, 25 + BAR_HEIGHT, 582, 36, 5);
		goUp = Triangle({ 384,75 }, { 414,85 }, { 354,85 });
		goDown = Triangle({ 354,560 }, { 414,560 }, { 384,570 });
		CSVData csv(U"data\\Fav\\list.csv");
		if (csv)
		{
			for (int i = 0; i < (signed)csv.rows(); ++i)
			{
				auto albumDName = csv.get<String>(i, 0);
				auto musicDName = csv.get<String>(i, 1);
				String albumName, musicName, musicComment;
				Audio music;
				TextReader albumTextReader(U"music\\" + albumDName + U"\\" + albumDName + U".txt");
				TextReader musicTextReader(U"music\\" + albumDName + U"\\" + musicDName + U"\\" + musicDName + U".txt");
				albumTextReader.readLine(albumName);
				musicTextReader.readLine(musicName);
				String temp; while (musicTextReader.readLine(temp)) musicComment += temp + U"\n";
				const String extensions[] = { U".wav",U".ogg",U".mp3" };
				for (auto ext : extensions)
				{
					if (FileSystem::IsFile(U"music\\" + albumDName + U"\\" + musicDName + U"\\" + musicDName + ext))
					{
						music = Audio(U"music\\" + albumDName + U"\\" + musicDName + U"\\" + musicDName + ext);
						break;
					}
				}
				getData().addFav(albumName, albumDName, musicName, musicDName, musicComment, music);
			}
		}
	}
	albumList_begin = 0;

	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Fav";
}

// 更新
void Fav::update()
{
	if (KeyB.pressed()) changeScene(U"Select", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);

	// 曲リスト 更新
	if (goUp.leftClicked()) --albumList_begin;
	if (goDown.leftClicked()) ++albumList_begin;
	if (rect_albumListAll.mouseOver())
	{
		if ((albumList_begin + MAX_CELL_NUM <= (signed)getData().FavMusicList.size()) || (albumList_begin > 0)) albumList_begin += (int)Mouse::Wheel();
	}
	albumList_begin = Max(albumList_begin, 0);
	albumList_begin = Min<int>(albumList_begin, Max<int>((int)getData().FavMusicList.size() - MAX_CELL_NUM, 0));

	for (int i = albumList_begin; ((i - albumList_begin) < Min<int>(MAX_CELL_NUM, (signed)getData().FavMusicList.size())) && (i < (signed)getData().FavMusicList.size()); ++i)
	{
		auto num = i - albumList_begin;
		auto music = getData().FavMusicList[i];
		RoundRect rect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
		if (rect.leftClicked())
		{
			if (getData().selectedFavMusicIndex != i) getData().selectedFavMusicIndex = i;
			(getData().FavMusicList[getData().selectedFavMusicIndex].music.isPlaying() ? getData().FavMusicList[getData().selectedFavMusicIndex].music.pause() : getData().FavMusicList[getData().selectedFavMusicIndex].music.play());
		}
		rect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
		if (rect.leftClicked())
		{
			(getData().isFav(music.album_name, music.music_name) ? getData().removeFav(music.album_name, music.music_name) : getData().addFav(music.album_name, music.album_dir, music.music_name, music.music_dir, music.comment, music.music));
		}
		rect = RoundRect(rect_albumListCell.x, rect_albumListCell.y + num * 39, rect_albumListCell.w, rect_albumListCell.h, rect_albumListCell.r);
		if (rect.leftClicked())
		{
			if (getData().selectedFavMusicIndex != i) getData().selectedFavMusicIndex = i;
			if (!getData().FavMusicList[getData().selectedFavMusicIndex].music.isPlaying()) getData().FavMusicList[getData().selectedFavMusicIndex].music.play();
			changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
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
		goUp.draw((goUp.mouseOver() ? Palette::Orange : Palette::White));
		goUp.drawFrame(2, Palette::Black);
	}
	if (albumList_begin + MAX_CELL_NUM < (signed)getData().FavMusicList.size())
	{
		goDown.draw((goDown.mouseOver() ? Palette::Orange : Palette::White));
		goDown.drawFrame(2, Palette::Black);
	}
	for (int i = 0; i < MAX_CELL_NUM; ++i)
	{
		RoundRect(rect_albumList_Flag.x, rect_albumList_Flag.y + i * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Name.x, rect_albumList_Name.y + i * 39, rect_albumList_Name.w, rect_albumList_Name.h, rect_albumList_Name.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Time.x, rect_albumList_Time.y + i * 39, rect_albumList_Time.w, rect_albumList_Time.h, rect_albumList_Time.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + i * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r).draw(Color(32, 32, 32, 200));
	}
	for (int i = albumList_begin; (i - albumList_begin) < Min<int>(MAX_CELL_NUM, (int)getData().FavMusicList.size() - albumList_begin); ++i)
	{
		auto num = i - albumList_begin;
		auto tmp = getData().FavMusicList[i];
		RoundRect tmpRRect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
		if (tmp.music.isPlaying()) pausing.drawAt(43, 43 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver() ? Palette::Orange : Palette::White));
		else playing.drawAt(43, 43 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver() ? Palette::Orange : Palette::White));
		font_albumList(Fav_musicNameBeShort(tmp.music_name)).draw(70, 29 + BAR_HEIGHT + num * 39);
		auto str = Format(Pad(tmp.totalTime / 60, std::make_pair(2, U'0')), U":", Pad(tmp.totalTime % 60, std::make_pair(2, U'0')));
		font_albumList(str).draw(610, 29 + BAR_HEIGHT + num * 39);
		tmpRRect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
		((getData().isFav(tmp.album_name, tmp.music_name) || tmpRRect.mouseOver()) ? fav : not_fav).drawAt(725, 43 + BAR_HEIGHT + num * 39);
	}
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

// お気に入りリスト 保存
void Fav::saveFavList()
{
	CSVData csv(U"data\\Fav\\list.csv");
	for (auto i : getData().FavMusicList)
	{
		csv.writeRow(i.album_name, i.music_name);
	}
}

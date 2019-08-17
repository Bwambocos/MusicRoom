// include
#include <Siv3D.hpp>
#include "Bar.h"
#include "Fav.h"

// お気に入り画面

// 初期化
Fav::Fav(const InitData& init) : IScene(init)
{
	// シーン状況 更新
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Fav";

	backgroundImage = Texture(U"data\\backgroundImage.png");
	playImage = Texture(U"data\\Album\\playImage.png");
	pauseImage = Texture(U"data\\Album\\pauseImage.png");
	notFavImage = Texture(U"data\\Album\\notFavImage.png");
	favImage = Texture(U"data\\Album\\favImage.png");
	albumListFont = Font(20, U"data\\fontR.ttc");
	albumList_FlagRect = RoundRect(25, 25 + barHeight, 36, 36, 5);
	albumList_NameRect = RoundRect(64, 25 + barHeight, 537, 36, 5);
	albumList_TimeRect = RoundRect(604, 25 + barHeight, 100, 36, 5);
	albumList_FavRect = RoundRect(707, 25 + barHeight, 36, 36, 5);
	albumList_AllRect = RoundRect(25, 25 + barHeight, 718, 456, 5);
	albumList_CellRect = RoundRect(64, 25 + barHeight, 582, 36, 5);
	goUpButton = Triangle({ 384,75 }, { 414,85 }, { 354,85 });
	goDownButton = Triangle({ 354,560 }, { 414,560 }, { 384,570 });

	FavMusicListFirstIndex = 0;
}

// 更新
void Fav::update()
{
	// 曲リスト 更新
	if (goUpButton.leftClicked()) --FavMusicListFirstIndex;
	if (goDownButton.leftClicked()) ++FavMusicListFirstIndex;
	if (albumList_AllRect.mouseOver()) FavMusicListFirstIndex += (int)Mouse::Wheel();
	FavMusicListFirstIndex = Max(FavMusicListFirstIndex, 0);
	FavMusicListFirstIndex = Min<int>(FavMusicListFirstIndex, Max<int>((int)getData().FavMusicList.size() - maxCellNum, 0));

	for (int i = FavMusicListFirstIndex; ((i - FavMusicListFirstIndex) < Min<int>(maxCellNum, (signed)getData().FavMusicList.size())) && (i < (signed)getData().FavMusicList.size()); ++i)
	{
		auto num = i - FavMusicListFirstIndex;
		auto music = getData().FavMusicList[i];
		RoundRect rect(albumList_FlagRect.x, albumList_FlagRect.y + num * 39, albumList_FlagRect.w, albumList_FlagRect.h, albumList_FlagRect.r);
		if (rect.leftClicked())
		{
			if (getData().selectedFavMusicIndex != i) getData().selectedFavMusicIndex = i;
			if (getData().FavMusicList[getData().selectedFavMusicIndex].music.isPlaying()) getData().FavMusicList[getData().selectedFavMusicIndex].music.pause();
			else getData().FavMusicList[getData().selectedFavMusicIndex].music.play();
		}
		rect = RoundRect(albumList_FavRect.x, albumList_FavRect.y + num * 39, albumList_FavRect.w, albumList_FavRect.h, albumList_FavRect.r);
		if (rect.leftClicked()) (getData().isFav(music.album_name, music.music_name) ? getData().removeFav(music.album_name, music.music_name) : getData().addFav(music.album_name, music.album_dir, music.music_name, music.music_dir, music.comment, music.music));
		rect = RoundRect(albumList_CellRect.x, albumList_CellRect.y + num * 39, albumList_CellRect.w, albumList_CellRect.h, albumList_CellRect.r);
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
	backgroundImage.draw(0, barHeight);

	// 曲リスト 描画
	if (FavMusicListFirstIndex > 0)
	{
		goUpButton.draw((goUpButton.mouseOver() ? Palette::Orange : Palette::White));
		goUpButton.drawFrame(2, Palette::Black);
	}
	if (FavMusicListFirstIndex + maxCellNum < (signed)getData().FavMusicList.size())
	{
		goDownButton.draw((goDownButton.mouseOver() ? Palette::Orange : Palette::White));
		goDownButton.drawFrame(2, Palette::Black);
	}
	for (int i = 0; i < maxCellNum; ++i)
	{
		RoundRect(albumList_FlagRect.x, albumList_FlagRect.y + i * 39, albumList_FlagRect.w, albumList_FlagRect.h, albumList_FlagRect.r).draw(Color(32, 32, 32, 200));
		RoundRect(albumList_NameRect.x, albumList_NameRect.y + i * 39, albumList_NameRect.w, albumList_NameRect.h, albumList_NameRect.r).draw(Color(32, 32, 32, 200));
		RoundRect(albumList_TimeRect.x, albumList_TimeRect.y + i * 39, albumList_TimeRect.w, albumList_TimeRect.h, albumList_TimeRect.r).draw(Color(32, 32, 32, 200));
		RoundRect(albumList_FavRect.x, albumList_FavRect.y + i * 39, albumList_FavRect.w, albumList_FavRect.h, albumList_FavRect.r).draw(Color(32, 32, 32, 200));
	}
	for (int i = FavMusicListFirstIndex; (i - FavMusicListFirstIndex) < Min<int>(maxCellNum, (int)getData().FavMusicList.size() - FavMusicListFirstIndex); ++i)
	{
		auto num = i - FavMusicListFirstIndex;
		auto music = getData().FavMusicList[i];
		RoundRect tmpRRect(albumList_FlagRect.x, albumList_FlagRect.y + num * 39, albumList_FlagRect.w, albumList_FlagRect.h, albumList_FlagRect.r);
		if (music.music.isPlaying()) pauseImage.drawAt(43, 43 + barHeight + num * 39, (tmpRRect.mouseOver() ? Palette::Orange : Palette::White));
		else playImage.drawAt(43, 43 + barHeight + num * 39, (tmpRRect.mouseOver() ? Palette::Orange : Palette::White));
		albumListFont(music.music_compressedName).draw(70, 29 + barHeight + num * 39);
		auto str = Format(Pad(music.totalTime / 60, std::make_pair(2, U'0')), U":", Pad(music.totalTime % 60, std::make_pair(2, U'0')));
		albumListFont(str).draw(610, 29 + barHeight + num * 39);
		tmpRRect = RoundRect(albumList_FavRect.x, albumList_FavRect.y + num * 39, albumList_FavRect.w, albumList_FavRect.h, albumList_FavRect.r);
		((getData().isFav(music.album_name, music.music_name) || tmpRRect.mouseOver()) ? favImage : notFavImage).drawAt(725, 43 + barHeight + num * 39);
	}
}

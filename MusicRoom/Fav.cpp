// include
#include <Siv3D.hpp>
#include "Bar.h"
#include "Fav.h"

// お気に入り画面

// 初期化
Fav::Fav(const InitData& init) : IScene(init)
{
	// アセット管理登録
	FontAsset::Register(U"Fav.albumList", 20, U"data\\fontR.ttc");
	TextureAsset::Register(U"Fav.play", Icon(0xf144, 36));
	TextureAsset::Register(U"Fav.pause", Icon(0xf28b, 36));
	TextureAsset::Register(U"Fav.fav", Icon(0xf005, 36));
	TextureAsset::Register(U"Fav.goUp", Icon(0xf139, 42));
	TextureAsset::Register(U"Fav.goDown", Icon(0xf13a, 42));

	// シーン状況 更新
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Fav";

	albumList_FlagRect = Rect(25, 35 + barHeight, 36, 36);
	albumList_NameRect = Rect(albumList_FlagRect.x + albumList_FlagRect.w + 5, albumList_FlagRect.y, Scene::Width() - (36 * 2 + 5 * 2 + 25 * 2), 36);
	albumList_FavRect = Rect(albumList_NameRect.x + albumList_NameRect.w + 5, albumList_FlagRect.y, 36, 36);
	albumList_AllRect = Rect(albumList_FlagRect.x, albumList_FlagRect.y, Scene::Width() - 50, 36 * (int)albumListRows + 5 * ((int)albumListRows - 1));
	goUpPos = Vec2(albumList_AllRect.center().x, barHeight + 5 + TextureAsset(U"Fav.goUp").height() / 2);
	goDownPos = Vec2(albumList_AllRect.center().x, Scene::Height() - 5 - TextureAsset(U"Fav.goDown").height() / 2);
	FavMusicListFirstIndex = 0;
}

// 更新
void Fav::update()
{
	// 曲リスト 更新
	if (TextureAsset(U"Fav.goUp").regionAt(goUpPos).leftClicked()) --FavMusicListFirstIndex;
	if (TextureAsset(U"Fav.goDown").regionAt(goDownPos).leftClicked()) ++FavMusicListFirstIndex;
	if (albumList_AllRect.mouseOver()) FavMusicListFirstIndex += (int)Mouse::Wheel();
	FavMusicListFirstIndex = Max(FavMusicListFirstIndex, 0);
	FavMusicListFirstIndex = Min<int>(FavMusicListFirstIndex, Max((int)getData().FavMusicList.size() - (int)albumListRows, 0));

	for (int i = FavMusicListFirstIndex; i - FavMusicListFirstIndex < Min((int)albumListRows, (signed)getData().FavMusicList.size()) && i < (signed)getData().FavMusicList.size(); ++i)
	{
		auto num = i - FavMusicListFirstIndex;
		auto music = getData().FavMusicList[i];
		if (albumList_FlagRect.movedBy(0, num * 39).leftClicked())
		{
			if (getData().selectedFavMusicIndex != i && getData().selectedFavMusicIndex != -1) AudioAsset(U"album-" + getData().FavMusicList[getData().selectedFavMusicIndex].album_dir + U".music-" + getData().FavMusicList[getData().selectedFavMusicIndex].music_dir).stop();
			getData().selectedFavMusicIndex = i;
			auto newMusic = AudioAsset(U"album-" + getData().FavMusicList[getData().selectedFavMusicIndex].album_dir + U".music-" + getData().FavMusicList[getData().selectedFavMusicIndex].music_dir);
			if (newMusic.isPlaying()) newMusic.pause();
			else newMusic.play();
		}
		if (albumList_FavRect.movedBy(0, num * 39).leftClicked()) (getData().isFav(music.album_name, music.music_name) ? getData().removeFav(music.album_name, music.music_name) : getData().addFav(music.album_name, music.album_dir, music.album_creator, music.album_comment, music.music_name, music.music_dir, music.comment, music.music));
		if (albumList_NameRect.movedBy(0, num * 39).leftClicked())
		{
			if (getData().selectedFavMusicIndex != i && getData().selectedFavMusicIndex != -1) AudioAsset(U"album-" + getData().FavMusicList[getData().selectedFavMusicIndex].album_dir + U".music-" + getData().FavMusicList[getData().selectedFavMusicIndex].music_dir).stop();
			if (getData().selectedFavMusicIndex != i) getData().selectedFavMusicIndex = i;
			if (!AudioAsset(U"album-" + getData().FavMusicList[getData().selectedFavMusicIndex].album_dir + U".music-" + getData().FavMusicList[getData().selectedFavMusicIndex].music_dir).isPlaying()) AudioAsset(U"album-" + getData().FavMusicList[getData().selectedFavMusicIndex].album_dir + U".music-" + getData().FavMusicList[getData().selectedFavMusicIndex].music_dir).play();
			changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
	}
}

// 描画
void Fav::draw() const
{
	// 曲リスト 描画
	if (FavMusicListFirstIndex > 0) TextureAsset(U"Fav.goUp").drawAt(goUpPos, (TextureAsset(U"Fav.goUp").regionAt(goUpPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
	if (FavMusicListFirstIndex + albumListRows < (signed)getData().FavMusicList.size()) TextureAsset(U"Fav.goDown").drawAt(goDownPos, (TextureAsset(U"Fav.goDown").regionAt(goDownPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
	for (auto i : step(albumListRows))
	{
		drawButton(albumList_FlagRect.movedBy(0, (int)i * 41), false);
		drawButton(albumList_NameRect.movedBy(0, (int)i * 41), false);
		drawButton(albumList_FavRect.movedBy(0, (int)i * 41), false);
	}
	for (int i = FavMusicListFirstIndex; (i - FavMusicListFirstIndex) < Min((int)albumListRows, (int)getData().FavMusicList.size() - FavMusicListFirstIndex); ++i)
	{
		auto num = i - FavMusicListFirstIndex;
		auto music = getData().FavMusicList[i];
		auto rect = albumList_FlagRect.movedBy(0, num * 41);
		if (i == getData().selectedFavMusicIndex && AudioAsset(U"album-" + music.album_dir + U".music-" + music.music_dir).isPlaying()) TextureAsset(U"Fav.pause").drawAt(rect.center(), (rect.mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
		else TextureAsset(U"Fav.play").drawAt(rect.center(), (rect.mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
		FontAsset(U"Fav.albumList")(music.music_name).draw(albumList_NameRect.movedBy(0, num * 41).stretched(-5, -1), getData().stringColor);
		rect = albumList_FavRect.movedBy(0, num * 41);
		TextureAsset(U"Fav.fav").drawAt(rect.center(), (getData().isFav(music.album_name, music.music_name) || rect.mouseOver()) ? Palette::Gold : getData().schemeColor4);
	}
}

// ボタン描画
void Fav::drawButton(Rect rect, bool highlight) const
{
	rect
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(getData().schemeColor3)
		.drawFrame(0, 1.5, ColorF((rect.mouseOver() || highlight ? getData().schemeColor5 : getData().schemeColor4), 0.6 + (rect.mouseOver() ? 1 : 0) * 0.4));
	rect.draw(getData().schemeColor3);
}

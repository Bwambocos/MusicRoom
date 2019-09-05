// include
#include <Siv3D.hpp>
#include "Bar.h"
#include "Music.h"

// 曲詳細画面

// 初期化
Music::Music(const InitData& init) : IScene(init)
{
	// アセット管理登録
	FontAsset::Register(U"Music.albumName", 28, U"data\\fontR.ttc");
	FontAsset::Register(U"Music.albumCreator", 28, U"data\\fontR.ttc");
	FontAsset::Register(U"Music.albumExpl", 18, U"data\\fontR.ttc");
	FontAsset::Register(U"Music.musicName", 28, U"data\\fontR.ttc");
	FontAsset::Register(U"Music.musicExpl", 20, U"data\\fontR.ttc");
	FontAsset::Register(U"Music.header", 18, U"data\\fontR.ttc");
	TextureAsset::Register(U"Music.play", Icon(0xf144, 42));
	TextureAsset::Register(U"Music.pause", Icon(0xf28b, 42));
	TextureAsset::Register(U"Music.rep", Icon(0xf079, 42));
	TextureAsset::Register(U"Music.seek", Icon(0xf192, 42));
	TextureAsset::Register(U"Music.stop", Icon(0xf28d, 42));
	TextureAsset::Register(U"Music.fav", Icon(0xf005, 42));
	TextureAsset::Register(U"Music.image", Icon(0xf03e, FontAsset(U"Music.header").height() - 8));
	TextureAsset::Register(U"Music.name", Icon(0xf001, FontAsset(U"Music.header").height() - 8));
	TextureAsset::Register(U"Music.expl", Icon(0xf075, FontAsset(U"Music.header").height() - 8));
	TextureAsset::Register(U"Music.credit", Icon(0xf0c0, FontAsset(U"Music.header").height() - 8));

	// シーン状況 更新
	if (getData().nowScene != U"Music")
	{
		getData().prevScene = getData().nowScene;
		getData().nowScene = U"Music";
	}

	albumImageRect = Rect(Scene::Width() / 4 - 125, 35 + barHeight, 250, 250);
	albumNameRect = Rect(25, albumImageRect.y + albumImageRect.h + 35, Scene::Width() / 2 - 50, 54);
	albumCreatorRect = Rect(25, albumNameRect.y + albumNameRect.h + 35, albumNameRect.w, 48);
	albumExplRect = Rect(25, albumCreatorRect.y + albumCreatorRect.h + 35, albumNameRect.w, Scene::Height() - (albumCreatorRect.y + albumCreatorRect.h + 35) - 25);
	musicNameRect = Rect(Scene::Width() / 2, albumImageRect.y, Scene::Width() / 2 - 25, FontAsset(U"Music.musicName").height() + 5);
	musicSeekRect = RoundRect(musicNameRect.x + TextureAsset(U"Music.play").width() + TextureAsset(U"Music.rep").width() + 10, musicNameRect.y + musicNameRect.h + 25, musicNameRect.w - (TextureAsset(U"Music.play").width() + TextureAsset(U"Music.rep").width() + TextureAsset(U"Music.stop").width() + TextureAsset(U"Music.fav").width() + 20), TextureAsset(U"Music.stop").height() - 8, (TextureAsset(U"Music.stop").height() - 5) / 2);
	musicExplRect = Rect(musicNameRect.x, (int)musicSeekRect.y + (int)musicSeekRect.h + 25 + FontAsset(U"Music.header").height(), musicNameRect.w, Scene::Height() - ((int)musicSeekRect.y + (int)musicSeekRect.h + 50 + FontAsset(U"Music.header").height()));
	rectHeader = Quad(Vec2(0, 0), Vec2(160, 0), Vec2(160 + FontAsset(U"Music.header").height(), FontAsset(U"Music.header").height()), Vec2(0, FontAsset(U"Music.header").height()));
	playPos = Vec2(musicNameRect.x + TextureAsset(U"Music.play").width() / 2, musicSeekRect.center().y);
	repPos = Vec2(playPos.x + TextureAsset(U"Music.play").width() / 2 + 5 + TextureAsset(U"Music.rep").width() / 2, musicSeekRect.center().y);
	stopPos = Vec2(musicSeekRect.x + musicSeekRect.w + 5 + TextureAsset(U"Music.stop").width() / 2, musicSeekRect.center().y);
	favPos = Vec2(stopPos.x + TextureAsset(U"Music.stop").width() / 2 + 5 + TextureAsset(U"Music.fav").width() / 2, musicSeekRect.center().y);

	// 描画位置 初期化
	draw_musicNameTime.restart();
	draw_musicNameStayFlag = true;
	draw_musicNameX = musicNameRect.x + 5;
	if (getData().prevScene == U"Fav") musicComment = getData().FavMusicList[getData().selectedFavMusicIndex].comment;
	else musicComment = getData().MusicList[getData().AlbumList[getData().selectedAlbumIndex].dir][getData().selectedMusicIndex].comment;
}

// 更新
void Music::update()
{
	// 現在再生中の曲を更新
	if (getData().prevScene == U"Fav")
	{
		if (getData().selectedFavMusicIndex == -1)
		{
			changeScene(U"Fav", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
			return;
		}
		auto favMusicData = getData().FavMusicList[getData().selectedFavMusicIndex];
		albumName = favMusicData.album_name;
		albumDir = favMusicData.album_dir;
		albumCreator = favMusicData.album_creator;
		albumExpl = favMusicData.album_comment;
		musicName = favMusicData.music_name;
		if (!musicDir.isEmpty() && musicDir != favMusicData.music_dir)
		{
			changeScene(U"Music", 0, false);
			return;
		}
		musicDir = favMusicData.music_dir;
		musicComment = favMusicData.comment;
	}
	else
	{
		auto selectedAlbumData = getData().AlbumList[getData().selectedAlbumIndex];
		albumName = selectedAlbumData.name;
		albumDir = selectedAlbumData.dir;
		albumCreator = selectedAlbumData.creator;
		albumExpl = selectedAlbumData.comment;
		auto selectedMusicData = getData().MusicList[albumDir][getData().selectedMusicIndex];
		musicName = selectedMusicData.name;
		if (!musicDir.isEmpty() && musicDir != selectedMusicData.dir)
		{
			changeScene(U"Music", 0, false);
			return;
		}
		musicDir = selectedMusicData.dir;
		musicComment = selectedMusicData.comment;
	}
	auto nowMusic = AudioAsset(U"album-" + albumDir + U".music-" + musicDir);

	// 曲情報 更新
	draw_musicName_update();
	FFT::Analyze(musicFFT, nowMusic);

	// 再生バー 更新
	// バー
	if (musicSeekRect.leftPressed())
	{
		seekButtonFlag = true;
		nowMusic.pause();
		nowMusic.setPosSample(nowMusic.samples() * (Cursor::Pos().x - (int)musicSeekRect.x) / (int)musicSeekRect.w);
	}
	else if (seekButtonFlag)
	{
		nowMusic.play();
		seekButtonFlag = false;
	}
	seekPos = Vec2(musicSeekRect.x + musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples(), musicSeekRect.y + musicSeekRect.h / 2);

	// ボタン
	if (TextureAsset(nowMusic.isPlaying() ? U"Music.pause" : U"Music.play").regionAt(playPos).leftClicked() || KeyEnter.pressed())
	{
		if (nowMusic.isPlaying()) nowMusic.pause();
		else nowMusic.play();
	}
	if (TextureAsset(U"Music.rep").regionAt(repPos).leftClicked() || KeyShift.pressed())
	{
		const int tmpTime = (int)nowMusic.streamPosSample();
		nowMusic.pause();
		getData().selectedMusicLoopFlag = !getData().selectedMusicLoopFlag;
		nowMusic.setLoop(getData().selectedMusicLoopFlag);
		nowMusic.play();
		nowMusic.setPosSample(tmpTime);
	}
	if (TextureAsset(U"Music.stop").regionAt(stopPos).leftClicked() || KeySpace.pressed())
	{
		nowMusic.stop();
		if (getData().nowScene == U"Fav" || getData().prevScene == U"Fav") getData().selectedFavMusicIndex = -1;
		else getData().selectedMusicIndex = -1;
		if (getData().nowScene == U"Music")
		{
			if (getData().prevScene == U"Fav") changeScene(U"Fav", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
			else changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
		return;
	}
	if (TextureAsset(U"Music.fav").regionAt(favPos).leftClicked())
	{
		if (getData().prevScene == U"Album")
		{
			(getData().isFav(albumName, musicName)
				? getData().removeFav(albumName, musicName)
				: getData().addFav(albumName, albumDir, albumCreator, albumExpl, musicName, musicDir, musicComment, nowMusic));
		}
		if (getData().prevScene == U"Fav") (getData().isFav(albumName, musicName) ? getData().removeFav(albumName, musicName) : getData().addFav(albumName, albumDir, albumCreator, albumExpl, musicName, musicDir, musicComment, nowMusic));
	}
}

// 描画
void Music::draw() const
{
	const auto nowMusic = AudioAsset(U"album-" + albumDir + U".music-" + musicDir);

	if (!musicFFT.buffer.empty()) for (auto i : step(256)) RectF(Scene::Width() / 256 * i, Scene::Height(), Scene::Width() / 256, -Pow(musicFFT.buffer[i], 0.3) * Scene::Height()).draw(ColorF(getData().schemeColor3, 0.5));

	// アルバム情報 描画
	drawStrBackground(albumImageRect, U"アルバム画像", U"Music.image");
	drawStrBackground(albumNameRect, U"アルバム名", U"Music.name");
	drawStrBackground(albumCreatorRect, U"クレジット", U"Music.credit");
	drawStrBackground(albumExplRect, U"アルバム説明", U"Music.expl");
	albumImageRect
		.stretched(-5)
		(TextureAsset(U"album-" + albumDir + U".image")).draw();
	FontAsset(U"Music.albumName")(albumName).draw(albumNameRect.stretched(-5, -3), getData().stringColor);
	FontAsset(U"Music.albumCreator")(albumCreator).draw(albumCreatorRect.stretched(-5, -3), getData().stringColor);
	FontAsset(U"Music.albumExpl")(albumExpl).draw(albumExplRect.stretched(-5, -5), getData().stringColor);

	// 曲情報 描画
	drawStrBackground(musicNameRect, U"曲名", U"Music.name");
	drawStrBackground(musicExplRect, U"曲説明", U"Music.expl");
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(musicNameRect);
	FontAsset(U"Music.musicName")(musicName).draw(draw_musicNameX, musicNameRect.y + 3, getData().stringColor);
	Graphics2D::SetScissorRect(Scene::Rect());
	FontAsset(U"Music.musicExpl")(musicComment).draw(musicExplRect.stretched(-5, -5), getData().stringColor);

	// 再生バー
	// バー
	musicSeekRect.draw(getData().schemeColor2);
	RoundRect(musicSeekRect.x, musicSeekRect.y, musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples() + 10, musicSeekRect.h, musicSeekRect.r).draw(Palette::Lightgreen);
	musicSeekRect.drawFrame(1, getData().schemeColor3);
	TextureAsset(U"Music.seek").drawAt(seekPos, nowMusic.isPlaying() ? getData().schemeColor5 : getData().schemeColor4);

	// ボタン
	TextureAsset(nowMusic.isPlaying() ? U"Music.pause" : U"Music.play").drawAt(playPos, TextureAsset(nowMusic.isPlaying() ? U"Music.play" : U"Music.pause").regionAt(playPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4);
	TextureAsset(U"Music.rep").drawAt(repPos, (TextureAsset(U"Music.rep").regionAt(repPos).mouseOver() || getData().selectedMusicLoopFlag) ? Palette::Orange : getData().schemeColor4);
	TextureAsset(U"Music.stop").drawAt(stopPos, TextureAsset(U"Music.stop").regionAt(stopPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4);
	TextureAsset(U"Music.fav").drawAt(favPos, (getData().isFav(albumName, musicName) || TextureAsset(U"Music.fav").regionAt(favPos).mouseOver()) ? Palette::Gold : getData().schemeColor4);
}

// 曲名描画位置 更新
void Music::draw_musicName_update()
{
	auto rect = musicNameRect;
	auto width = FontAsset(U"Music.musicName")(getData().prevScene == U"Fav" ? getData().FavMusicList[getData().selectedFavMusicIndex].music_name : getData().MusicList[albumDir][getData().selectedMusicIndex].name).region().w;
	if (width > musicNameRect.w)
	{
		if (!draw_musicNameStayFlag)
		{
			if (draw_musicNameX + width > rect.x + rect.w) draw_musicNameX = musicNameRect.x + 5 - (double)draw_moveXPerSec * draw_musicNameTime.ms() / 1000;
			else
			{
				draw_musicNameTime.restart();
				draw_musicNameStayFlag = true;
			}
		}
		if (draw_musicNameStayFlag)
		{
			if (draw_musicNameTime.ms() >= draw_stayMillisec)
			{
				draw_musicNameTime.restart();
				if (draw_musicNameX == musicNameRect.x + 5) draw_musicNameStayFlag = false;
				else draw_musicNameX = musicNameRect.x + 5;
			}
		}
	}
}

// 文字背景描画
void Music::drawStrBackground(Rect rect, String header, String icon) const
{
	rectHeader
		.movedBy(rect.x, rect.y - FontAsset(U"Music.header").height() + 3)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
	TextureAsset(icon).drawAt(rect.x + FontAsset(U"Music.header").height() / 2 + 5, rect.y - FontAsset(U"Music.header").height() / 2 + 3, getData().stringColor);
	FontAsset(U"Music.header")(header).draw(rect.x + FontAsset(U"Music.header").height() + 5, rect.y - FontAsset(U"Music.header").height() + 3, getData().stringColor);
	rect
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
}

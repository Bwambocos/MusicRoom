// include
#include <Siv3D.hpp>
#include "Bar.h"
#include "Music.h"

// 曲詳細画面

// 初期化
Music::Music(const InitData& init) : IScene(init)
{
	// アセット管理登録
	TextureAsset::Register(U"Music.play", Icon(0xf144, 36));
	TextureAsset::Register(U"Music.pause", Icon(0xf28b, 36));
	TextureAsset::Register(U"Music.stop", Icon(0xf28d, 36));
	TextureAsset::Register(U"Music.seek", Icon(0xf192, 36));
	TextureAsset::Register(U"Music.rep", Icon(0xf2f1, 36));
	TextureAsset::Register(U"Music.fav", Icon(0xf005, 36));
	FontAsset::Register(U"Music.musicNameAndTime", 28, U"data\\fontR.ttc");
	FontAsset::Register(U"Music.musicExpl", 20, U"data\\fontR.ttc");

	// シーン状況 更新
	if (getData().nowScene != U"Music")
	{
		getData().prevScene = getData().nowScene;
		getData().nowScene = U"Music";
	}

	musicNameRect = RoundRect(25, 25 + barHeight, 468, 48, 5);
	musicTimeRect = RoundRect(496, 25 + barHeight, 199, 48, 5);
	musicFavRect = RoundRect(698, 25 + barHeight, 48, 48, 5);
	musicSeekRect = RoundRect(127, 91 + barHeight, 565, 21, 15);
	musicExplRect = Rect(25, 130 + barHeight, 718, 357);
	playPos = Vec2(45, musicSeekRect.y + musicSeekRect.h / 2);
	stopPos = Vec2(723, musicSeekRect.y + musicSeekRect.h / 2);
	repPos = Vec2(90, musicSeekRect.y + musicSeekRect.h / 2);
	favPos = Vec2(722, 49 + barHeight);

	// 描画位置 初期化
	draw_musicNameTime.restart();
	draw_musicNameStayFlag = true;
	draw_musicNameX = draw_musicNameDefaultX;
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
		musicName = favMusicData.music_name;
		if (!musicDir.isEmpty() && musicDir != favMusicData.music_dir)
		{
			changeScene(U"Music", 0, false);
			return;
		}
		musicDir = favMusicData.music_dir;
		musicComment = favMusicData.comment;
		musicTotalTime = Format(Pad(favMusicData.totalTime / 60, std::make_pair(2, U'0')), U":", Pad(favMusicData.totalTime % 60, std::make_pair(2, U'0')));
	}
	else
	{
		auto selectedAlbumData = getData().AlbumList[getData().selectedAlbumIndex];
		albumName = selectedAlbumData.name;
		albumDir = selectedAlbumData.dir;
		auto selectedMusicData = getData().MusicList[albumDir][getData().selectedMusicIndex];
		musicName = selectedMusicData.name;
		if (!musicDir.isEmpty() && musicDir != selectedMusicData.dir)
		{
			changeScene(U"Music", 0, false);
			return;
		}
		musicDir = selectedMusicData.dir;
		musicComment = selectedMusicData.comment;
		musicTotalTime = Format(Pad(AudioAsset(U"album-" + albumDir + U".music-" + musicDir).lengthSec() / 60, std::make_pair(2, U'0')), U":", Pad((int)AudioAsset(U"album-" + albumDir + U".music-" + musicDir).lengthSec() % 60, std::make_pair(2, U'0')));
	}
	auto nowMusic = AudioAsset(U"album-" + albumDir + U".music-" + musicDir);

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

	// 曲情報 更新
	draw_musicName_update();
	if (TextureAsset(U"Music.fav").regionAt(favPos).leftClicked())
	{
		if (getData().prevScene == U"Album")
		{
			(getData().isFav(albumName, musicName)
				? getData().removeFav(albumName, musicName)
				: getData().addFav(albumName, albumDir, musicName, musicDir, musicComment, nowMusic));
		}
		if (getData().prevScene == U"Fav") (getData().isFav(albumName, musicName) ? getData().removeFav(albumName, musicName) : getData().addFav(albumName, albumDir, musicName, musicDir, musicComment, nowMusic));
	}
	FFT::Analyze(musicFFT, nowMusic);
}

// 描画
void Music::draw() const
{
	const auto nowMusic = AudioAsset(U"album-" + albumDir + U".music-" + musicDir);

	// 背景 描画
	if (!musicFFT.buffer.empty()) for (auto i : step(51)) { RectF(1 + i * 15, Scene::Height(), 15, -Pow(musicFFT.buffer[i], 0.8) * 750).draw(Color(200, 200, 200, 200)); }
	musicNameRect.draw(Color(64, 64, 64, 200));
	musicNameRect.drawFrame(1, Palette::Black);
	musicTimeRect.draw(Color(64, 64, 64, 200));
	musicTimeRect.drawFrame(1, Palette::Black);
	musicFavRect.draw(Color(64, 64, 64, 200));
	musicFavRect.drawFrame(1, Palette::Black);

	// 再生バー
	// バー
	musicSeekRect.draw(Color(64, 64, 64, 200));
	RoundRect(musicSeekRect.x, musicSeekRect.y, musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples(), musicSeekRect.h, musicSeekRect.r).draw(Color(0, 200, 0, 200));
	musicSeekRect.drawFrame(1, Palette::Black);

	// Seek
	TextureAsset(U"Music.seek").drawAt(seekPos, nowMusic.isPlaying() ? Palette::Orange : Palette::White);

	// ボタン
	TextureAsset(nowMusic.isPlaying() ? U"Music.pause" : U"Music.play").drawAt(playPos, TextureAsset(nowMusic.isPlaying() ? U"Music.play" : U"Music.pause").regionAt(playPos).mouseOver() ? Palette::Orange : Palette::White);
	TextureAsset(U"Music.rep").drawAt(repPos, (TextureAsset(U"Music.rep").regionAt(repPos).mouseOver() || getData().selectedMusicLoopFlag) ? Palette::Orange : Palette::White);
	TextureAsset(U"Music.stop").drawAt(stopPos, TextureAsset(U"Music.stop").regionAt(stopPos).mouseOver() ? Palette::Orange : Palette::White);
	musicExplRect.draw(Color(64, 64, 64, 200));
	musicExplRect.drawFrame(1, Palette::Black);

	// 曲情報 描画
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)musicNameRect.x, (int)musicNameRect.y, (int)musicNameRect.w, (int)musicNameRect.h));
	FontAsset(U"Music.musicNameAndTime")(musicName).draw(draw_musicNameX, 28 + barHeight);
	Graphics2D::SetScissorRect(Rect(0, 0, Scene::Width(), Scene::Height()));
	FontAsset(U"Music.musicNameAndTime")(musicTotalTime).draw(504, 28 + barHeight);
	TextureAsset(U"Music.fav").drawAt(favPos, (getData().isFav(albumName, musicName) || musicFavRect.mouseOver()) ? Palette::Orange : Palette::White);
	FontAsset(U"Music.musicExpl")(musicComment).draw(musicExplRect.stretched(-5, -5), Palette::White);
}

// 曲名描画位置 更新
void Music::draw_musicName_update()
{
	auto rect = musicNameRect;
	auto width = FontAsset(U"Music.musicNameAndTime")(getData().prevScene == U"Fav" ? getData().FavMusicList[getData().selectedFavMusicIndex].music_name : getData().MusicList[albumDir][getData().selectedMusicIndex].name).region().w + rect.r;
	if (width > musicNameRect.w)
	{
		if (!draw_musicNameStayFlag)
		{
			if (draw_musicNameX + width > rect.x + rect.w) draw_musicNameX = draw_musicNameDefaultX - (double)draw_moveXPerSec * draw_musicNameTime.ms() / 1000;
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
				if (draw_musicNameX == draw_musicNameDefaultX) draw_musicNameStayFlag = false;
				else draw_musicNameX = draw_musicNameDefaultX;
			}
		}
	}
}

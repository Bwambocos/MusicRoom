// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"
#include "Music.h"

// 曲詳細画面

// 初期化
Music::Music(const InitData& init) : IScene(init)
{
	// シーン状況 更新
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Music";

	// 初回のみ
	if (backgroundImage.isEmpty())
	{
		backgroundImage = Texture(U"data\\backgroundImage.png");
		playImage_all[0] = Texture(U"data\\Music\\play\\normal.png");
		playImage_all[1] = Texture(U"data\\Music\\play\\active.png");
		pauseImage_all[0] = Texture(U"data\\Music\\pause\\normal.png");
		pauseImage_all[1] = Texture(U"data\\Music\\pause\\active.png");
		stopImage_all[0] = Texture(U"data\\Music\\stop\\normal.png");
		stopImage_all[1] = Texture(U"data\\Music\\stop\\active.png");
		seekImage_all[0] = Texture(U"data\\Music\\seek\\normal.png");
		seekImage_all[1] = Texture(U"data\\Music\\seek\\active.png");
		repImage_all[0] = Texture(U"data\\Music\\rep\\normal.png");
		repImage_all[1] = Texture(U"data\\Music\\rep\\active.png");
		favedImage = Texture(U"data\\Music\\favedImage.png");
		notFavedImage = Texture(U"data\\Music\\notFavedImage.png");
		musicNameAndTimeFont = Font(18);
		musicExplFont = Font(16);
		musicNameRect = RoundRect(25, 25 + BAR_HEIGHT, 468, 48, 10);
		musicTimeRect = RoundRect(496, 25 + BAR_HEIGHT, 199, 48, 10);
		musicFavRect = RoundRect(698, 25 + BAR_HEIGHT, 48, 48, 10);
		musicSeekRect = RoundRect(127, 91 + BAR_HEIGHT, 565, 21, 5);
		musicExplRect = RoundRect(25, 130 + BAR_HEIGHT, 718, 357, 10);
		playImage_display = playImage_all[0];
		pauseImage_display = pauseImage_all[0];
		stopImage_display = stopImage_all[0];
		seekImage_display = seekImage_all[0];
		repImage_display = repImage_all[0];
	}

	// 毎回
	// 描画位置 初期化
	draw_musicNameTime.restart();
	draw_musicNameStayFlag = true;
	draw_musicNameX = draw_musicNameDefaultX;

	const int32 w = (int32)musicExplRect.w - 10;
	const int32 h = (int32)musicExplRect.h;
	size_t pos = 0;
	musicComment_separeted.clear();
	while (pos < (int)musicComment.length())
	{
		for (int i = 0; i + pos < (int)musicComment.length(); ++i)
		{
			if (musicExplFont(musicComment.substr(pos, i)).region().w >= w)
			{
				musicComment_separeted.push_back(musicComment.substr(pos, i - 1));
				pos += (i - 1);
				break;
			}
			if (musicComment[pos + i] == U'\n')
			{
				musicComment_separeted.push_back(musicComment.substr(pos, i + 1));
				pos += (i + 1);
				break;
			}
		}
	} 
	for (size_t i = 0; i < musicComment_separeted.size(); ++i)
	{
		const int32 y = static_cast<int32>(musicExplRect.y + 10 + i * musicExplFont.height());
		const bool overflow = (i + 1 < musicComment_separeted.size()) && (y + musicExplFont.height() * 2 + 10 > musicExplRect.y + musicExplRect.h);
		if (overflow)
		{
			if (musicComment_separeted[i].length() > 2) musicComment_separeted[i].resize(musicComment_separeted[i].length() - (musicComment_separeted[i].length() > 2 ? 2 : 1));
			if (musicComment_separeted[i][musicComment_separeted[i].length() - 1] == U'\n') musicComment_separeted[i][musicComment_separeted[i].length() - 1] = U'\0';
			musicComment_separeted[i].append(U"...");
		}
		if (overflow) break;
	}
}

// 更新
void Music::update()
{
	if (getData().prevScene == U"Fav")
	{
		auto favMusicData = getData().FavMusicList[getData().selectedFavMusicIndex];
		albumName = favMusicData.album_name;
		albumDir = favMusicData.album_dir;
		musicName = favMusicData.music_name;
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
		musicDir = selectedMusicData.dir;
		musicComment = selectedMusicData.comment;
		musicTotalTime = Format(Pad(selectedMusicData.totalTime / 60, std::make_pair(2, U'0')), U":", Pad(selectedMusicData.totalTime % 60, std::make_pair(2, U'0')));
	}
	Audio& nowMusic = (getData().prevScene == U"Fav"
		? (getData().selectedFavMusicIndex == -1 ? Audio() : getData().FavMusicList[getData().selectedFavMusicIndex].music)
		: (getData().selectedMusicIndex == -1 || getData().MusicList[albumDir].empty() ? Audio() : getData().MusicList[albumDir][getData().selectedMusicIndex].music));

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

	// ボタン
	Circle tmpCircle(45, musicSeekRect.y + musicSeekRect.h / 2, 15);
	playImage_display = playImage_all[(tmpCircle.mouseOver() ? 1 : 0)];
	pauseImage_display = pauseImage_all[(tmpCircle.mouseOver() ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeyEnter.pressed()) (nowMusic.isPlaying() ? nowMusic.pause() : nowMusic.play());
	tmpCircle = Circle(90, musicSeekRect.y + musicSeekRect.h / 2, 15);
	repImage_display = repImage_all[((tmpCircle.mouseOver() || getData().selectedMusicLoopFlag) ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeyShift.pressed())
	{
		const int tmpTime = (int)nowMusic.streamPosSample();
		nowMusic.pause();
		getData().selectedMusicLoopFlag = !getData().selectedMusicLoopFlag;
		nowMusic.setLoop(getData().selectedMusicLoopFlag);
		nowMusic.play();
		nowMusic.setPosSample(tmpTime);
	}
	tmpCircle = Circle(723, musicSeekRect.y + musicSeekRect.h / 2, 15);
	stopImage_display = stopImage_all[(tmpCircle.mouseOver() ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeySpace.pressed())
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
	if (musicFavRect.leftClicked())
	{
		(getData().isFav(getData().AlbumList[getData().selectedAlbumIndex].name, musicName)
			? getData().removeFav(getData().AlbumList[getData().selectedAlbumIndex].name, musicName)
			: getData().addFav(getData().AlbumList[getData().selectedAlbumIndex].name, albumDir, musicName, musicDir, musicComment, nowMusic));
	}
	FFT::Analyze(musicFFT, nowMusic);
}

// 描画
void Music::draw() const
{
	Audio& nowMusic = (getData().prevScene == U"Fav"
		? (getData().selectedFavMusicIndex == -1 ? Audio() : getData().FavMusicList[getData().selectedFavMusicIndex].music)
		: (getData().selectedMusicIndex == -1 || getData().MusicList[albumDir].empty() ? Audio() : getData().MusicList[albumDir][getData().selectedMusicIndex].music));

	// 背景 描画
	backgroundImage.draw(0, BAR_HEIGHT);
	if (!musicFFT.buffer.empty()) for (auto i : step(51)) { RectF(1 + i * 15, Window::Height(), 15, -Pow(musicFFT.buffer[i], 0.8) * 750).draw(Color(200, 200, 200, 200)); }
	musicNameRect.drawShadow({ 0,15 }, 32, 10);
	musicNameRect.draw(Color(32, 32, 32, 120));
	musicTimeRect.drawShadow({ 0,15 }, 32, 10);
	musicTimeRect.draw(Color(32, 32, 32, 120));
	musicFavRect.drawShadow({ 0,15 }, 32, 10);
	musicFavRect.draw(Color(32, 32, 32, 120));

	// 再生バー
	// バー
	musicSeekRect.drawShadow({ 0,15 }, 32, 10);
	musicSeekRect.drawFrame(3);
	musicSeekRect.draw(Color(32, 32, 32, 120));
	const RoundRect tmpRect(musicSeekRect.x, musicSeekRect.y, musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples(), musicSeekRect.h, musicSeekRect.r);
	tmpRect.draw(Color(0, 200, 0, 120));

	// Seek
	auto x = musicSeekRect.x + musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples();
	(nowMusic.isPlaying() ? seekImage_all[1] : seekImage_all[0]).drawAt(x, musicSeekRect.y + musicSeekRect.h / 2);

	// ボタン
	(nowMusic.isPlaying() ? pauseImage_display : playImage_display).drawAt(45, musicSeekRect.y + musicSeekRect.h / 2);
	repImage_display.drawAt(90, musicSeekRect.y + musicSeekRect.h / 2);
	stopImage_display.drawAt(723, musicSeekRect.y + musicSeekRect.h / 2);
	musicExplRect.drawShadow({ 0,15 }, 32, 10);
	musicExplRect.drawFrame(3);
	musicExplRect.draw(Color(32, 32, 32, 120));

	// 曲情報 描画
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)musicNameRect.x, (int)musicNameRect.y, (int)musicNameRect.w, (int)musicNameRect.h));
	musicNameAndTimeFont(musicName).draw(draw_musicNameX, 31 + BAR_HEIGHT);
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
	musicNameAndTimeFont(musicTotalTime).draw(504, 31 + BAR_HEIGHT);
	((getData().isFav(albumName, musicName) || musicFavRect.mouseOver()) ? favedImage : notFavedImage).drawAt(722, 49 + BAR_HEIGHT);
	musicExpl_draw();
	musicNameRect.drawFrame(0, 2, Palette::Gray);
	musicTimeRect.drawFrame(0, 2, Palette::Gray);
	musicFavRect.drawFrame(0, 2, Palette::Gray);
}

// 曲説明 描画
void Music::musicExpl_draw() const
{
	for (size_t i = 0; i < musicComment_separeted.size(); ++i)
	{
		const int32 y = static_cast<int32>(musicExplRect.y + 10 + i * musicExplFont.height());
		const bool overflow = (i + 1 < musicComment_separeted.size()) && (y + musicExplFont.height() * 2 + 10 > musicExplRect.y + musicExplRect.h);
		musicExplFont(musicComment_separeted[i]).draw(musicExplRect.x + 10, y);
		if (overflow) break;
	}
}

// 曲名描画位置 更新
void Music::draw_musicName_update()
{
	auto rect = musicNameRect;
	auto width = musicNameAndTimeFont(getData().prevScene == U"Fav" ? getData().FavMusicList[getData().selectedFavMusicIndex].music_name : getData().MusicList[albumDir][getData().selectedMusicIndex].name).region().w + rect.r;
	if (width > musicNameRect.w)
	{
		if (!draw_musicNameStayFlag)
		{
			if (draw_musicNameX + width > rect.x + rect.w) draw_musicNameX = -(double)draw_moveXPerSec * draw_musicNameTime.ms() / 1000;
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

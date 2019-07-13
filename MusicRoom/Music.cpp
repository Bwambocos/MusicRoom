// include
#include <Siv3D.hpp>
#include "Bar.h"
#include "Music.h"

// �ȏڍ׉��

// ������
Music::Music(const InitData& init) : IScene(init)
{
	// �V�[���� �X�V
	if (getData().nowScene != U"Music")
	{
		getData().prevScene = getData().nowScene;
		getData().nowScene = U"Music";
	}

	backgroundImage = Texture(U"data\\backgroundImage.png");
	playImage_all[0] = Texture(U"data\\Music\\playNormal.png");
	playImage_all[1] = Texture(U"data\\Music\\playActive.png");
	pauseImage_all[0] = Texture(U"data\\Music\\pauseNormal.png");
	pauseImage_all[1] = Texture(U"data\\Music\\pauseActive.png");
	stopImage_all[0] = Texture(U"data\\Music\\stopNormal.png");
	stopImage_all[1] = Texture(U"data\\Music\\stopActive.png");
	seekImage_all[0] = Texture(U"data\\Music\\seekNormal.png");
	seekImage_all[1] = Texture(U"data\\Music\\seekActive.png");
	repImage_all[0] = Texture(U"data\\Music\\repNormal.png");
	repImage_all[1] = Texture(U"data\\Music\\repActive.png");
	favedImage = Texture(U"data\\Music\\favedImage.png");
	notFavedImage = Texture(U"data\\Music\\notFavedImage.png");
	musicNameAndTimeFont = Font(28, U"data\\fontR.ttc");
	musicExplFont = Font(20, U"data\\fontR.ttc");
	musicNameRect = RoundRect(25, 25 + barHeight, 468, 48, 5);
	musicTimeRect = RoundRect(496, 25 + barHeight, 199, 48, 5);
	musicFavRect = RoundRect(698, 25 + barHeight, 48, 48, 5);
	musicSeekRect = RoundRect(127, 91 + barHeight, 565, 21, 15);
	musicExplRect = RoundRect(25, 130 + barHeight, 718, 357, 5);
	playImage_display = playImage_all[0];
	pauseImage_display = pauseImage_all[0];
	stopImage_display = stopImage_all[0];
	seekImage_display = seekImage_all[0];
	repImage_display = repImage_all[0];

	// �`��ʒu ������
	draw_musicNameTime.restart();
	draw_musicNameStayFlag = true;
	draw_musicNameX = draw_musicNameDefaultX;

	const int32 w = (int32)musicExplRect.w - 10;
	const int32 h = (int32)musicExplRect.h;
	size_t pos = 0;
	if (getData().prevScene == U"Fav") musicComment = getData().FavMusicList[getData().selectedFavMusicIndex].comment;
	else musicComment = getData().MusicList[getData().AlbumList[getData().selectedAlbumIndex].dir][getData().selectedMusicIndex].comment;
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

// �X�V
void Music::update()
{
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
		musicTotalTime = Format(Pad(selectedMusicData.totalTime / 60, std::make_pair(2, U'0')), U":", Pad(selectedMusicData.totalTime % 60, std::make_pair(2, U'0')));
	}
	Audio& nowMusic = (getData().prevScene == U"Fav"
		? (getData().selectedFavMusicIndex == -1 ? tempAudio1 : getData().FavMusicList[getData().selectedFavMusicIndex].music)
		: (getData().selectedMusicIndex == -1 || getData().MusicList[albumDir].empty() ? tempAudio1 : getData().MusicList[albumDir][getData().selectedMusicIndex].music));

	// �Đ��o�[ �X�V
	// �o�[
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

	// �{�^��
	Circle tmpCircle(45, musicSeekRect.y + musicSeekRect.h / 2, 15);
	playImage_display = playImage_all[(tmpCircle.mouseOver() ? 1 : 0)];
	pauseImage_display = pauseImage_all[(tmpCircle.mouseOver() ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeyEnter.pressed())
	{
		if (nowMusic.isPlaying()) nowMusic.pause();
		else nowMusic.play();
	}
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

	// �ȏ�� �X�V
	draw_musicName_update();
	if (musicFavRect.leftClicked())
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

// �`��
void Music::draw() const
{
	const Audio& nowMusic = (getData().prevScene == U"Fav"
		? (getData().selectedFavMusicIndex == -1 ? tempAudio2 : getData().FavMusicList[getData().selectedFavMusicIndex].music)
		: (getData().selectedMusicIndex == -1 || getData().MusicList[albumDir].empty() ? tempAudio2 : getData().MusicList[albumDir][getData().selectedMusicIndex].music));

	// �w�i �`��
	backgroundImage.draw();
	if (!musicFFT.buffer.empty()) for (auto i : step(51)) { RectF(1 + i * 15, Scene::Height(), 15, -Pow(musicFFT.buffer[i], 0.8) * 750).draw(Color(200, 200, 200, 200)); }
	musicNameRect.draw(Color(64, 64, 64, 200));
	musicNameRect.drawFrame(1, Palette::Black);
	musicTimeRect.draw(Color(64, 64, 64, 200));
	musicTimeRect.drawFrame(1, Palette::Black);
	musicFavRect.draw(Color(64, 64, 64, 200));
	musicFavRect.drawFrame(1, Palette::Black);

	// �Đ��o�[
	// �o�[
	musicSeekRect.draw(Color(64, 64, 64, 200));
	const RoundRect tmpRect(musicSeekRect.x, musicSeekRect.y, musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples(), musicSeekRect.h, musicSeekRect.r);
	tmpRect.draw(Color(0, 200, 0, 200));
	musicSeekRect.drawFrame(1, Palette::Black);

	// Seek
	auto x = musicSeekRect.x + musicSeekRect.w * nowMusic.streamPosSample() / nowMusic.samples();
	(nowMusic.isPlaying() ? seekImage_all[1] : seekImage_all[0]).drawAt(x, musicSeekRect.y + musicSeekRect.h / 2);

	// �{�^��
	(nowMusic.isPlaying() ? pauseImage_display : playImage_display).drawAt(45, musicSeekRect.y + musicSeekRect.h / 2);
	repImage_display.drawAt(90, musicSeekRect.y + musicSeekRect.h / 2);
	stopImage_display.drawAt(723, musicSeekRect.y + musicSeekRect.h / 2);
	musicExplRect.draw(Color(64, 64, 64, 200));
	musicExplRect.drawFrame(1, Palette::Black);

	// �ȏ�� �`��
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)musicNameRect.x, (int)musicNameRect.y, (int)musicNameRect.w, (int)musicNameRect.h));
	musicNameAndTimeFont(musicName).draw(draw_musicNameX, 28 + barHeight);
	Graphics2D::SetScissorRect(Rect(0, 0, Scene::Width(), Scene::Height()));
	musicNameAndTimeFont(musicTotalTime).draw(504, 28 + barHeight);
	((getData().isFav(albumName, musicName) || musicFavRect.mouseOver()) ? favedImage : notFavedImage).drawAt(722, 49 + barHeight);
	musicExpl_draw();
}

// �Ȑ��� �`��
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

// �Ȗ��`��ʒu �X�V
void Music::draw_musicName_update()
{
	auto rect = musicNameRect;
	auto width = musicNameAndTimeFont(getData().prevScene == U"Fav" ? getData().FavMusicList[getData().selectedFavMusicIndex].music_name : getData().MusicList[albumDir][getData().selectedMusicIndex].name).region().w + rect.r;
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

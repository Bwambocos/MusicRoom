// include
#include <Siv3D.hpp>
#include "Bar.h"

// ステータスバー

// 初期化
Bar::Bar()
{
	// 初回のみ
	playImage_all[0] = Texture(U"data\\Bar\\playNormal.png");
	playImage_all[1] = Texture(U"data\\Bar\\playActive.png");
	pauseImage_all[0] = Texture(U"data\\Bar\\pauseNormal.png");
	pauseImage_all[1] = Texture(U"data\\Bar\\pauseActive.png");
	stopImage_all[0] = Texture(U"data\\Bar\\stopNormal.png");
	stopImage_all[1] = Texture(U"data\\Bar\\stopActive.png");
	seekImage_all[0] = Texture(U"data\\Bar\\seekNormal.png");
	seekImage_all[1] = Texture(U"data\\Bar\\seekActive.png");
	repImage_all[0] = Texture(U"data\\Bar\\repNormal.png");
	repImage_all[1] = Texture(U"data\\Bar\\repActive.png");
	prevImage_all[0] = Texture(U"data\\Bar\\prevNormal.png");
	prevImage_all[1] = Texture(U"data\\Bar\\prevActive.png");
	nextImage_all[0] = Texture(U"data\\Bar\\nextNormal.png");
	nextImage_all[1] = Texture(U"data\\Bar\\nextActive.png");
	backImage_all[0] = Texture(U"data\\Bar\\backNormal.png");
	backImage_all[1] = Texture(U"data\\Bar\\backActive.png");
	goImage_all[0] = Texture(U"data\\Bar\\goNormal.png");
	goImage_all[1] = Texture(U"data\\Bar\\goActive.png");
	shareImage_all[0] = Texture(U"data\\Bar\\shareNormal.png");
	shareImage_all[1] = Texture(U"data\\Bar\\shareActive.png");
	playImage_display = playImage_all[0];
	pauseImage_display = pauseImage_all[0];
	stopImage_display = stopImage_all[0];
	seekImage_display = seekImage_all[0];
	repImage_display = repImage_all[0];
	prevImage_display = prevImage_all[0];
	nextImage_display = nextImage_all[0];
	backImage_display = backImage_all[0];
	goImage_display = goImage_all[0];
	shareImage_display = shareImage_all[0];
	fieldRect = Rect(0, 0, GameInfo::Width, barHeight);
	mainRect = RoundRect(192, 0, mainRectWidth, barHeight, 16);
	mainFont = Font(26, U"data\\fontR.ttc");
	dog = Audio(U"data\\Bar\\dog.mp3");
}

// 更新
void Bar::update(GameData& getData, MyApp& manager)
{
	if (getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
	{
		musicData = (getData.selectedFavMusicIndex == -1 ? MusicData() : MusicData{ getData.FavMusicList[getData.selectedFavMusicIndex].music,getData.FavMusicList[getData.selectedFavMusicIndex].music_name,getData.FavMusicList[getData.selectedFavMusicIndex].music_dir,getData.FavMusicList[getData.selectedFavMusicIndex].comment });
		albumName = (getData.selectedFavMusicIndex == -1 ? U"" : getData.FavMusicList[getData.selectedFavMusicIndex].album_name);
		albumDir = (getData.selectedFavMusicIndex == -1 ? U"" : getData.FavMusicList[getData.selectedFavMusicIndex].album_dir);
	}
	else
	{
		albumDir = (getData.selectedAlbumIndex == -1 ? U"" : getData.AlbumList[getData.selectedAlbumIndex].dir);
		musicData = (getData.selectedMusicIndex == -1 || getData.MusicList[albumDir].empty() ? MusicData() : getData.MusicList[albumDir][getData.selectedMusicIndex]);
		albumName = (getData.selectedAlbumIndex == -1 ? U"" : getData.AlbumList[getData.selectedAlbumIndex].name);
	}
	Audio& nowMusic = ((getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
		? (getData.selectedFavMusicIndex == -1 ? tempAudio : getData.FavMusicList[getData.selectedFavMusicIndex].music)
		: (getData.selectedMusicIndex == -1 || getData.MusicList[albumDir].empty() ? tempAudio : getData.MusicList[albumDir][getData.selectedMusicIndex].music));

	if (musicHasChangedFlag)
	{
		nowMusic.play();
		musicHasChangedFlag = false;
	}
	if (!nowMusic.isEmpty() && !nowMusic.isPlaying() && !nowMusic.isPaused() && musicSamplesPlayed > 0 && nowMusic.posSample() == 0) changeMusic(getData, manager, 1);
	else musicSamplesPlayed = nowMusic.posSample();
	if (KeyShift.pressed() && KeyD.pressed() && KeyO.pressed() && KeyG.pressed()) dog.play();

	// ボタン 更新
	const Rect backRect(10, 10, 44, 44);
	const Rect goRect(GameInfo::Width - 54, 10, 44, 44);
	if (getData.nowScene == U"Select") draw_back_flag = draw_go_flag = false;
	if (getData.nowScene == U"Album")
	{
		draw_back_flag = true;
		draw_go_flag = nowMusic.isPlaying();
		if (backRect.leftClicked())
		{
			nowMusic.stop();
			manager.changeScene(U"Select", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
		if (draw_go_flag && goRect.leftClicked()) manager.changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
	}
	if (getData.nowScene == U"Fav")
	{
		draw_back_flag = true;
		draw_go_flag = nowMusic.isPlaying();
		if (backRect.leftClicked())
		{
			nowMusic.stop();
			manager.changeScene(U"Select", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
		if (draw_go_flag && goRect.leftClicked()) manager.changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
	}
	if (getData.nowScene == U"Music")
	{
		draw_back_flag = true;
		draw_go_flag = false;
		if (backRect.leftClicked()) manager.changeScene((getData.prevScene == U"Fav" ? U"Fav" : U"Album"), GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
	}
	backImage_display = backImage_all[(backRect.mouseOver() ? 1 : 0)];
	goImage_display = goImage_all[(goRect.mouseOver() ? 1 : 0)];

	if (nowMusic.isPaused() || nowMusic.isPlaying())
	{
		int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int cou = 0; cou < 6; ++cou)
		{
			const Circle button(x + 20, 12 + 20, 20);
			switch (cou)
			{
			case 0:
				prevImage_display = prevImage_all[button.mouseOver()];
				if (button.leftClicked())
				{
					nowMusic.stop();
					changeMusic(getData, manager, -1);
				}
				break;
			case 1:
				if (nowMusic.isPlaying())
				{
					pauseImage_display = pauseImage_all[button.mouseOver()];
					if (button.leftClicked()) nowMusic.pause();
				}
				else
				{
					playImage_display = playImage_all[button.mouseOver()];
					if (button.leftClicked()) nowMusic.play();
				}
				break;
			case 2:
				repImage_display = repImage_all[button.mouseOver()];
				if (getData.selectedMusicLoopFlag) repImage_display = repImage_all[1];
				if (button.leftClicked())
				{
					const int tmpTime = (int)nowMusic.streamPosSample();
					nowMusic.pause();
					nowMusic.setLoop(!getData.selectedMusicLoopFlag);
					getData.selectedMusicLoopFlag = !getData.selectedMusicLoopFlag;
					nowMusic.play();
					nowMusic.setPosSample(tmpTime);
				}
				x += mainRectWidth;
				break;
			case 3:
				stopImage_display = stopImage_all[button.mouseOver()];
				if (button.leftClicked())
				{
					nowMusic.stop();
					if (getData.nowScene == U"Fav" || getData.prevScene == U"Fav") getData.selectedFavMusicIndex = -1;
					else getData.selectedMusicIndex = -1;
					if (getData.nowScene == U"Music")
					{
						if (getData.prevScene == U"Fav") manager.changeScene(U"Fav", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
						else manager.changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
					}
					stopImage_display = stopImage_all[0];
				}
				break;
			case 4:
				shareImage_display = shareImage_all[button.mouseOver()];
				if (button.leftClicked()) Twitter::OpenTweetWindow(U"#" + GameInfo::Title + U" でアルバム『" + albumName + U"』の曲「" + musicData.name + U"」を聴いています！ダウンロードはこちらから：https://github.com/Bwambocos/MusicRoom/releases");
				break;
			case 5:
				nextImage_display = nextImage_all[button.mouseOver()];
				if (button.leftClicked())
				{
					nowMusic.stop();
					changeMusic(getData, manager, 1);
				}
				break;
			}
			x += 40;
		}
	}
	else
	{
		playImage_display = playImage_all[0];
		pauseImage_display = pauseImage_all[0];
		stopImage_display = stopImage_all[0];
		seekImage_display = seekImage_all[0];
		repImage_display = repImage_all[0];
		prevImage_display = prevImage_all[0];
		nextImage_display = nextImage_all[0];
		shareImage_display = shareImage_all[0];
	}

	// メインテキスト 更新
	auto mainText_old = mainText;
	if (!nowMusic.isPlaying())
	{
		if (getData.nowScene == U"Select") mainText = U"アルバムを選択してください";
		if (getData.nowScene == U"Album") mainText = U"曲を選択してください";
		if (getData.nowScene == U"Fav") mainText = U"曲を選択してください";
	}
	else mainText = U"『" + albumName + U"』" + musicData.name;
	if (mainText != mainText_old)
	{
		draw_mainTextTime.restart();
		draw_mainText_stayFlag = true;
		draw_mainText_x = draw_mainTextDefaultX;
	}
	draw_mainText_update();
}

// 描画
void Bar::draw(GameData getData)
{
	fieldRect.draw(Color(200, 200, 200));
	mainRect.draw(Color(100, 100, 100));
	fieldRect.drawFrame(2, 0, Palette::Black);

	Audio& nowMusic = ((getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
		? (getData.selectedFavMusicIndex == -1 ? tempAudio : getData.FavMusicList[getData.selectedFavMusicIndex].music)
		: (getData.selectedMusicIndex == -1 || getData.MusicList[albumDir].empty() ? tempAudio : getData.MusicList[albumDir][getData.selectedMusicIndex].music));

	// ボタン 描画
	if (draw_back_flag) backImage_display.draw(10, 10);
	if (draw_go_flag) goImage_display.draw(GameInfo::Width - 10 - goImage_display.width(), 10);
	int x = 768 / 2 - mainRectWidth / 2 - 40 * 3 + 20;
	for (int cou = 0; cou < 6; ++cou)
	{
		switch (cou)
		{
		case 0:
			prevImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			break;
		case 1:
			if (nowMusic.isPlaying()) pauseImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			else playImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			break;
		case 2:
			repImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			x += mainRectWidth;
			break;
		case 3:
			stopImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			break;
		case 4:
			shareImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			break;
		case 5:
			nextImage_display.drawAt(x, mainRect.y + mainRect.h / 2);
			break;
		}
		x += 40;
	}

	// メインテキスト 描画
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)mainRect.x, (int)mainRect.y, (int)mainRect.w, (int)mainRect.h));
	mainFont(mainText).draw(draw_mainText_x, 12);
	Graphics2D::SetScissorRect(Rect(0, 0, Scene::Width(), Scene::Height()));
}

// 曲名描画位置 更新
void Bar::draw_mainText_update()
{
	auto rect = mainRect;
	auto width = mainFont(mainText).region().w + rect.r;
	if (width > rect.w)
	{
		if (!draw_mainText_stayFlag)
		{
			if (draw_mainText_x + width > rect.x + rect.w) draw_mainText_x = draw_mainTextDefaultX - (double)draw_mainTextMoveX * draw_mainTextTime.ms() / 1000;
			else
			{
				draw_mainTextTime.restart();
				draw_mainText_stayFlag = true;
			}
		}
		if (draw_mainText_stayFlag)
		{
			if (draw_mainTextTime.ms() >= draw_stayMillisec)
			{
				draw_mainTextTime.restart();
				const Rect tmpRect = mainFont(mainText).region();
				if (draw_mainText_x == draw_mainTextDefaultX) draw_mainText_stayFlag = false;
				else draw_mainText_x = draw_mainTextDefaultX;
			}
		}
	}
	else
	{
		const Rect tempRect = mainFont(mainText).region();
		draw_mainText_x = 384 - (int)tempRect.w / 2;
	}
}

// 曲変更処理
void Bar::changeMusic(GameData& getData, MyApp& manager, int t)
{
	if (getData.nowScene == U"Fav" || getData.prevScene == U"Fav") (getData.selectedFavMusicIndex += (t + (int)getData.FavMusicList.size())) %= getData.FavMusicList.size();
	if (getData.nowScene == U"Album" || getData.prevScene == U"Album")
	{
		getData.selectedMusicIndex += t;
		if (getData.selectedMusicIndex == getData.MusicList[albumDir].size())
		{
			getData.selectedMusicIndex = 0;
			(++getData.selectedAlbumIndex) %= getData.AlbumList.size();
			manager.changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
		if (getData.selectedMusicIndex == -1)
		{
			getData.selectedMusicIndex = 0;
			--getData.selectedAlbumIndex;
			(getData.selectedAlbumIndex += (int)getData.AlbumList.size()) %= getData.AlbumList.size();
			manager.changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
	}
	musicSamplesPlayed = 0;
	musicHasChangedFlag = true;
}

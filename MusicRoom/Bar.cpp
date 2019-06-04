// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"

// ステータスバー

// 初期化
Bar::Bar()
{
	// ボタン 初期化
	if (!dog)
	{
		originPlay[0] = Texture(U"data\\Bar\\play\\normal.png");
		originPlay[1] = Texture(U"data\\Bar\\play\\active.png");
		originBrief[0] = Texture(U"data\\Bar\\brief\\normal.png");
		originBrief[1] = Texture(U"data\\Bar\\brief\\active.png");
		originStop[0] = Texture(U"data\\Bar\\stop\\normal.png");
		originStop[1] = Texture(U"data\\Bar\\stop\\active.png");
		originSeek[0] = Texture(U"data\\Bar\\seek\\normal.png");
		originSeek[1] = Texture(U"data\\Bar\\seek\\active.png");
		originRep[0] = Texture(U"data\\Bar\\rep\\normal.png");
		originRep[1] = Texture(U"data\\Bar\\rep\\active.png");
		originPrev[0] = Texture(U"data\\Bar\\prev\\normal.png");
		originPrev[1] = Texture(U"data\\Bar\\prev\\active.png");
		originNext[0] = Texture(U"data\\Bar\\next\\normal.png");
		originNext[1] = Texture(U"data\\Bar\\next\\active.png");
		originBack[0] = Texture(U"data\\Bar\\Back\\normal.png");
		originBack[1] = Texture(U"data\\Bar\\Back\\active.png");
		originGo[0] = Texture(U"data\\Bar\\Go\\normal.png");
		originGo[1] = Texture(U"data\\Bar\\Go\\active.png");
		originShare[0] = Texture(U"data\\Bar\\Share\\normal.png");
		originShare[1] = Texture(U"data\\Bar\\Share\\active.png");
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
		displayPrev = originPrev[0];
		displayNext = originNext[0];
		displayBack = originBack[0];
		displayGo = originGo[0];
		displayShare = originShare[0];
		fieldRect = Rect(0, 0, GameInfo::Width, BAR_HEIGHT);
		mainRect = RoundRect(192, 0, mainRectWidth, BAR_HEIGHT, 16);
		mainFont = Font(18);
		timeFont = Font(12);
		dog = Audio(U"data\\Bar\\dog.mp3");
	}
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
	Audio & nowMusic = ((getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
		? (getData.selectedFavMusicIndex == -1 ? Audio() : getData.FavMusicList[getData.selectedFavMusicIndex].music)
		: (getData.selectedMusicIndex == -1 || getData.MusicList[albumDir].empty() ? Audio() : getData.MusicList[albumDir][getData.selectedMusicIndex].music));

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
		if (backRect.leftClicked())
		{
			manager.changeScene((getData.prevScene == U"Fav" ? U"Fav" : U"Album"), GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
	}
	displayBack = originBack[(backRect.mouseOver() ? 1 : 0)];
	displayGo = originGo[(goRect.mouseOver() ? 1 : 0)];

	if (nowMusic.isPaused() || nowMusic.isPlaying())
	{
		int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
		for (int cou = 0; cou < 6; ++cou)
		{
			const Circle button(x + 20, 12 + 20, 20);
			switch (cou)
			{
			case 0:
				if (button.mouseOver()) displayPrev = originPrev[1];
				else displayPrev = originPrev[0];
				if (button.leftClicked())
				{
					nowMusic.stop();
					changeMusic(getData, manager, -1);
				}
				break;
			case 1:
				if (nowMusic.isPlaying())
				{
					if (button.mouseOver()) displayBrief = originBrief[1];
					else displayBrief = originBrief[0];
					if (button.leftClicked()) nowMusic.pause();
				}
				else
				{
					if (button.mouseOver()) displayPlay = originPlay[1];
					else displayPlay = originPlay[0];
					if (button.leftClicked()) nowMusic.play();
				}
				break;
			case 2:
				if (button.mouseOver()) displayRep = originRep[1];
				else displayRep = originRep[0];
				if (getData.selectedMusicLoopFlag) displayRep = originRep[1];
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
				if (button.mouseOver()) displayStop = originStop[1];
				else displayStop = originStop[0];
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
					displayStop = originStop[0];
				}
				break;
			case 4:
				if (button.mouseOver()) displayShare = originShare[1];
				else displayShare = originShare[0];
				if (button.leftClicked()) Twitter::OpenTweetWindow(U"#" + GameInfo::Title + U" でアルバム『" + albumName + U"』の曲「" + musicData.name + U"」を聴いています！ダウンロードはこちらから：https://github.com/Bwambocos/MusicRoom/releases");
				break;
			case 5:
				if (button.mouseOver()) displayNext = originNext[1];
				else displayNext = originNext[0];
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
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
		displayPrev = originPrev[0];
		displayNext = originNext[0];
		displayShare = originShare[0];
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
		draw_mainText_stayMSec = draw_mainText_startMSec = (int)Time::GetMillisec();
		draw_mainText_stayFlag = true;
		draw_mainText_x = DEFAULT_mainText_X;
	}
	Update_drawMainText();
}

// 描画
void Bar::draw(GameData getData)
{
	fieldRect.draw(Color(204, 61, 104));
	mainRect.draw(Color(224, 67, 114));

	Audio& nowMusic = ((getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
		? (getData.selectedFavMusicIndex == -1 ? Audio() : getData.FavMusicList[getData.selectedFavMusicIndex].music)
		: (getData.selectedMusicIndex == -1 || getData.MusicList[albumDir].empty() ? Audio() : getData.MusicList[albumDir][getData.selectedMusicIndex].music));

	// ボタン 描画
	if (draw_back_flag) displayBack.draw(10, 10);
	if (draw_go_flag) displayGo.draw(GameInfo::Width - 10 - displayGo.width(), 10);
	int x = 768 / 2 - mainRectWidth / 2 - 40 * 3;
	for (int cou = 0; cou < 6; ++cou)
	{
		switch (cou)
		{
		case 0:
			displayPrev.draw(x, 12);
			break;
		case 1:
			if (nowMusic.isPlaying()) displayBrief.draw(x, 12);
			else displayPlay.draw(x, 12);
			break;
		case 2:
			displayRep.draw(x, 12);
			x += mainRectWidth;
			break;
		case 3:
			displayStop.draw(x, 12);
			break;
		case 4:
			displayShare.draw(x, 12);
			break;
		case 5:
			displayNext.draw(x, 12);
			break;
		}
		x += 40;
	}

	// メインテキスト 描画
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)mainRect.x, (int)mainRect.y, (int)mainRect.w, (int)mainRect.h));
	mainFont(mainText).draw(draw_mainText_x, 15);
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
}

// 曲名描画位置 更新
void Bar::Update_drawMainText()
{
	auto rect = mainRect;
	auto width = mainFont(mainText).region().w + rect.r;
	if (width > rect.w)
	{
		if (!draw_mainText_stayFlag)
		{
			if (draw_mainText_x + width > rect.x + rect.w) draw_mainText_x -= (double)DRAW_mainText_MOVE_X * (Time::GetMillisec() - draw_mainText_stayMSec) / 1000;
			else
			{
				draw_mainText_startMSec = draw_mainText_stayMSec = (int)Time::GetMillisec();
				draw_mainText_stayFlag = true;
			}
		}
		if (draw_mainText_stayFlag)
		{
			if (draw_mainText_stayMSec - draw_mainText_startMSec >= BAR_DRAW_STAYMSEC)
			{
				draw_mainText_startMSec = draw_mainText_stayMSec;
				const Rect tmpRect = mainFont(mainText).region();
				if (draw_mainText_x == DEFAULT_mainText_X) draw_mainText_stayFlag = false;
				else draw_mainText_x = DEFAULT_mainText_X;
			}
		}
		draw_mainText_stayMSec = (int)Time::GetMillisec();
	}
	else
	{
		const Rect tempRect = mainFont(mainText).region();
		draw_mainText_x = 384 - (int)tempRect.w / 2;
	}
}

// 曲変更処理
void Bar::changeMusic(GameData & getData, MyApp & manager, int t)
{
	if (getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
	{
		(getData.selectedFavMusicIndex += (t + (int)getData.FavMusicList.size())) %= getData.FavMusicList.size();
	}
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

// include
#include <Siv3D.hpp>
#include "Bar.h"

// ステータスバー

// 初期化
Bar::Bar()
{
	// アセット管理登録
	TextureAsset::Register(U"Bar.play", Icon(0xf144, 52));
	TextureAsset::Register(U"Bar.pause", Icon(0xf28b, 52));
	TextureAsset::Register(U"Bar.rep", Icon(0xf079, 52));
	TextureAsset::Register(U"Bar.seek", Icon(0xf192, 52));
	TextureAsset::Register(U"Bar.stop", Icon(0xf28d, 52));
	TextureAsset::Register(U"Bar.prev", Icon(0xf100, 52));
	TextureAsset::Register(U"Bar.next", Icon(0xf101, 52));
	TextureAsset::Register(U"Bar.share", Icon(0xf14d, 52));
	TextureAsset::Register(U"Bar.back", Icon(0xf0a8, 56));
	TextureAsset::Register(U"Bar.go", Icon(0xf0a9, 56));
	FontAsset::Register(U"Bar.main", 26, U"data\\fontR.ttc");
	AudioAsset::Register(U"Bar.dog", U"data\\Bar\\dog.mp3");

	fieldRect = Rect(0, 0, Scene::Width(), barHeight);
	mainRect = RoundRect(Scene::Width() / 2 - mainRectWidth / 2, 0, mainRectWidth, barHeight, barHeight / 2);
	prevPos = Vec2(mainRect.x - buttonSize * 2.5 - 30, barHeight / 2);
	playPos = Vec2(prevPos.x + buttonSize + 10, prevPos.y);
	repPos = Vec2(playPos.x + buttonSize + 10, prevPos.y);
	stopPos = Vec2(mainRect.x + mainRect.w + 10 + buttonSize / 2, prevPos.y);
	sharePos = Vec2(stopPos.x + buttonSize + 10, prevPos.y);
	nextPos = Vec2(sharePos.x + buttonSize + 10, prevPos.y);
	backPos = Vec2(5 + TextureAsset(U"Bar.back").width() / 2, prevPos.y);
	goPos = Vec2(Scene::Width() - 5 - TextureAsset(U"Bar.go").width() / 2, prevPos.y);
	draw_mainText_x = mainRect.x + 5;
}

// 更新
void Bar::update(GameData& getData, MyApp& manager)
{
	if (getData.nowScene == U"Fav" || getData.prevScene == U"Fav")
	{
		if (getData.selectedFavMusicIndex != -1)
		{
			auto favMusicData = getData.FavMusicList[getData.selectedFavMusicIndex];
			albumName = favMusicData.album_name;
			albumDir = favMusicData.album_dir;
			musicName = favMusicData.music_name;
			musicDir = favMusicData.music_dir;
		}
	}
	else
	{
		if (getData.selectedAlbumIndex != -1 && getData.selectedMusicIndex != -1)
		{
			auto selectedAlbumData = getData.AlbumList[getData.selectedAlbumIndex];
			albumName = selectedAlbumData.name;
			albumDir = selectedAlbumData.dir;
			auto selectedMusicData = getData.MusicList[albumDir][getData.selectedMusicIndex];
			musicName = selectedMusicData.name;
			musicDir = selectedMusicData.dir;
		}
	}
	auto nowMusic = AudioAsset(U"album-" + albumDir + U".music-" + musicDir);
	if (musicHasChangedFlag)
	{
		nowMusic.play();
		musicHasChangedFlag = false;
	}
	if (!nowMusic.isEmpty() && !nowMusic.isPlaying() && !nowMusic.isPaused() && musicSamplesPlayed > 0 && nowMusic.posSample() == 0) changeMusic(getData, manager, 1);
	else musicSamplesPlayed = nowMusic.posSample();
	if (KeyShift.pressed() && KeyD.pressed() && KeyO.pressed() && KeyG.pressed()) AudioAsset(U"Bar.dog").play();

	// ボタン 更新
	auto backRect = TextureAsset(U"Bar.back").regionAt(backPos);
	auto goRect = TextureAsset(U"Bar.go").regionAt(goPos);
	if (getData.nowScene == U"Select") draw_back_flag = draw_go_flag = false;
	if (getData.nowScene == U"Album")
	{
		draw_back_flag = true;
		draw_go_flag = nowMusic.isPlaying() || nowMusic.isPaused();
		if (backRect.leftClicked())
		{
			nowMusic.stop();
			musicSamplesPlayed = 0;
			getData.selectedMusicIndex = -1;
			manager.changeScene(U"Select", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
		if (draw_go_flag && goRect.leftClicked()) manager.changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
	}
	if (getData.nowScene == U"Fav")
	{
		draw_back_flag = true;
		draw_go_flag = nowMusic.isPlaying() || nowMusic.isPaused();
		if (backRect.leftClicked())
		{
			nowMusic.stop();
			musicSamplesPlayed = 0;
			getData.selectedMusicIndex = -1;
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

	if (nowMusic.isPaused() || nowMusic.isPlaying())
	{
		if (TextureAsset(U"Bar.prev").regionAt(prevPos).leftClicked())
		{
			nowMusic.stop();
			changeMusic(getData, manager, -1);
		}
		if (TextureAsset(U"Bar.play").regionAt(playPos).leftClicked())
		{
			if (nowMusic.isPlaying()) nowMusic.pause();
			else nowMusic.play();
		}
		if (TextureAsset(U"Bar.rep").regionAt(repPos).leftClicked())
		{
			const int tmpTime = (int)nowMusic.streamPosSample();
			nowMusic.pause();
			nowMusic.setLoop(!getData.selectedMusicLoopFlag);
			getData.selectedMusicLoopFlag = !getData.selectedMusicLoopFlag;
			nowMusic.play();
			nowMusic.setPosSample(tmpTime);
		}
		if (TextureAsset(U"Bar.stop").regionAt(stopPos).leftClicked())
		{
			nowMusic.stop();
			musicSamplesPlayed = 0;
			if (getData.nowScene == U"Fav" || getData.prevScene == U"Fav") getData.selectedFavMusicIndex = -1;
			else getData.selectedMusicIndex = -1;
			if (getData.nowScene == U"Music")
			{
				if (getData.prevScene == U"Fav") manager.changeScene(U"Fav", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
				else manager.changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
			}
		}
		if (TextureAsset(U"Bar.share").regionAt(sharePos).leftClicked())
		{
			Twitter::OpenTweetWindow(U"#" + GameInfo::Title + U" でアルバム『" + albumName + U"』の曲「" + musicName + U"」を聴いています！ダウンロードはこちらから：https://github.com/Bwambocos/MusicRoom/releases");
		}
		if (TextureAsset(U"Bar.next").regionAt(nextPos).leftClicked())
		{
			nowMusic.stop();
			changeMusic(getData, manager, 1);
		}
	}

	// メインテキスト 更新
	auto mainText_old = mainText;
	if (!(nowMusic.isPlaying() || nowMusic.isPaused()))
	{
		if (getData.nowScene == U"Select") mainText = U"アルバムを選択してください";
		if (getData.nowScene == U"Album") mainText = U"曲を選択してください";
		if (getData.nowScene == U"Fav") mainText = U"曲を選択してください";
	}
	else mainText = U"『" + albumName + U"』" + musicName;
	if (mainText != mainText_old)
	{
		draw_mainTextTime.restart();
		draw_mainText_stayFlag = true;
		draw_mainText_x = mainRect.x + 5;
	}
	draw_mainText_update();
}

// 描画
void Bar::draw(GameData getData)
{
	fieldRect.draw(getData.schemeColor2);
	mainRect.draw(getData.schemeColor3);
	fieldRect.drawFrame(2, 0, getData.schemeColor3);

	auto nowMusic = AudioAsset(U"album-" + albumDir + U".music-" + musicDir);

	// ボタン 描画
	if (draw_back_flag) TextureAsset(U"Bar.back").drawAt(backPos, (TextureAsset(U"Bar.back").regionAt(backPos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));
	if (draw_go_flag) TextureAsset(U"Bar.go").drawAt(goPos, (TextureAsset(U"Bar.go").regionAt(goPos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));
	TextureAsset(U"Bar.prev").drawAt(prevPos, (TextureAsset(U"Bar.prev").regionAt(prevPos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));
	TextureAsset((nowMusic.isPlaying() ? U"Bar.pause" : U"Bar.play")).drawAt(playPos, (TextureAsset(U"Bar.play").regionAt(playPos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));
	TextureAsset(U"Bar.rep").drawAt(repPos, ((TextureAsset(U"Bar.rep").regionAt(repPos).mouseOver() || getData.selectedMusicLoopFlag) ? Palette::Orange : getData.schemeColor4));
	TextureAsset(U"Bar.stop").drawAt(stopPos, (TextureAsset(U"Bar.stop").regionAt(stopPos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));
	TextureAsset(U"Bar.share").drawAt(sharePos, (TextureAsset(U"Bar.share").regionAt(sharePos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));
	TextureAsset(U"Bar.next").drawAt(nextPos, (TextureAsset(U"Bar.next").regionAt(nextPos).mouseOver() ? getData.schemeColor5 : getData.schemeColor4));

	// メインテキスト 描画
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(mainRect.rect);
	FontAsset(U"Bar.main")(mainText).draw(draw_mainText_x, mainRect.y + mainRect.h / 2 - FontAsset(U"Bar.main").height() / 2);
	Graphics2D::SetScissorRect(Scene::Rect());
}

// 曲名描画位置 更新
void Bar::draw_mainText_update()
{
	auto width = FontAsset(U"Bar.main")(mainText).region().w;
	if (width > mainRect.w)
	{
		if (!draw_mainText_stayFlag)
		{
			if (draw_mainText_x + width > mainRect.x + mainRect.w) draw_mainText_x = mainRect.x + 5 - (double)draw_mainTextMoveX * draw_mainTextTime.ms() / 1000;
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
				if (draw_mainText_x == mainRect.x + 5) draw_mainText_stayFlag = false;
				else draw_mainText_x = mainRect.x + 5;
			}
		}
	}
	else draw_mainText_x = Scene::Width() / 2 - (int)FontAsset(U"Bar.main")(mainText).region().w / 2;
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

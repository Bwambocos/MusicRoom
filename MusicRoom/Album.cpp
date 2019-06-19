// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Album.h"
#include "Select.h"

// アルバム詳細画面

// 初期化
Album::Album(const InitData& init) : IScene(init)
{
	// シーン状況 更新
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Album";

	backgroundImage = Texture(U"data\\backgroundImage.png");
	playImage = Texture(U"data\\Album\\playImage.png");
	pauseImage = Texture(U"data\\Album\\pauseImage.png");
	notFavImage = Texture(U"data\\Album\\notFavImage.png");
	favImage = Texture(U"data\\Album\\favImage.png");
	albumNameFont = Font(28, U"data\\fontR.ttc");
	albumCreatorFont = Font(28, U"data\\fontR.ttc");
	albumExplFont = Font(16, U"data\\fontR.ttc");
	albumListFont = Font(20, U"data\\fontR.ttc");
	albumImageRRect = RoundRect(25, 25 + barHeight, 250, 250, 12.5);
	albumNameRRect = RoundRect(325, 25 + barHeight, 393, 54, 10);
	albumCreatorRRect = RoundRect(325, 82 + barHeight, 393, 48, 10);
	albumExplRRect = RoundRect(325, 133 + barHeight, 393, 142, 10);
	albumList_FlagRRect = RoundRect(25, 300 + barHeight, 36, 36, 5);
	albumList_NameRRect = RoundRect(64, 300 + barHeight, 537, 36, 5);
	albumList_TimeRRect = RoundRect(604, 300 + barHeight, 100, 36, 5);
	albumList_FavRRect = RoundRect(707, 300 + barHeight, 36, 36, 5);
	albumListAllRRect = RoundRect(25, 300 + barHeight, 718, 190, 5);
	albumListCellRRect = RoundRect(64, 300 + barHeight, 582, 36, 5);
	goUpButton = Triangle({ 384,350 }, { 414,360 }, { 354,360 });
	goDownButton = Triangle({ 354,560 }, { 414,560 }, { 384,570 });

	// 描画位置 初期化
	draw_albumNameTime.restart();
	draw_albumCreatorTime.restart();
	draw_albumExplTime.restart();
	draw_albumNameStayFlag = true;
	draw_albumCreatorStayFlag = true;
	draw_albumExplStayFlag = true;
	draw_albumNameX = draw_albumNameDefaultX;
	draw_albumCreatorX = draw_albumCreatorDefaultX;
	draw_albumExpl_y = draw_albumExplDefaultY;

	// アルバム情報 初期化
	albumName = getData().AlbumList[getData().selectedAlbumIndex].name;
	albumDir = getData().AlbumList[getData().selectedAlbumIndex].dir;
	albumCreator = getData().AlbumList[getData().selectedAlbumIndex].creator;
	albumComment = getData().AlbumList[getData().selectedAlbumIndex].comment;
	albumImage = getData().AlbumList[getData().selectedAlbumIndex].image;
	MusicListFirstIndex = 0;
	albumComment_Separeted.clear();
	size_t pos = 0;
	while (pos < (int)albumComment.length())
	{
		for (int i = 0; i + pos < (int)albumComment.length(); ++i)
		{
			if (albumExplFont(albumComment.substr(pos, i)).region().w >= albumExplRRect.w - 10)
			{
				albumComment_Separeted.push_back(albumComment.substr(pos, i - 1));
				pos += (i - 1);
				break;
			}
			if (albumComment[pos + i] == U'\n')
			{
				albumComment_Separeted.push_back(albumComment.substr(pos, i + 1));
				pos += (i + 1);
				break;
			}
		}
	}

	// 曲リスト 初期化
	if (getData().MusicList[albumDir].empty())
	{
		const String extensions[] = { U".ogg", U".mp3" };
		TextReader reader1(U"music\\" + albumDir + U"\\music_list.txt");
		String musicDir;
		getData().MusicList[albumDir].clear();
		while (reader1.readLine(musicDir))
		{
			String musicName, musicComment;
			Audio music = Audio(U"music\\" + albumDir + U"\\" + musicDir + U"\\" + musicDir + U".mp3");
			TextReader reader2(U"music\\" + albumDir + U"\\" + musicDir + U"\\" + musicDir + U".txt");
			reader2.readLine(musicName);
			String temp; while (reader2.readLine(temp)) musicComment += temp + U"\n";
			if (music.isEmpty()) musicName = U"！読み込み失敗！";
			getData().MusicList[albumDir].push_back({ music, musicName, musicDir, musicComment, compressMusicName(musicName), (int)music.lengthSec() });
		}
	}
}

// 更新
void Album::update()
{
	// アルバム情報 更新
	draw_albumDetails_update();

	// 曲リスト 更新
	if (goUpButton.leftClicked()) --MusicListFirstIndex;
	if (goDownButton.leftClicked()) ++MusicListFirstIndex;
	if (albumListAllRRect.mouseOver()) MusicListFirstIndex += (int)Mouse::Wheel();
	MusicListFirstIndex = Max(MusicListFirstIndex, 0);
	MusicListFirstIndex = Min<int>(MusicListFirstIndex, Max<int>((int)getData().MusicList[albumDir].size() - 5, 0));
	for (int i = MusicListFirstIndex; i - MusicListFirstIndex < Min<int>(5, (signed)getData().MusicList[albumDir].size()) && i < (signed)getData().MusicList[albumDir].size(); ++i)
	{
		auto num = i - MusicListFirstIndex;
		auto music = getData().MusicList[albumDir][i];
		RoundRect rect(albumList_FlagRRect.x, albumList_FlagRRect.y + num * 39, albumList_FlagRRect.w, albumList_FlagRRect.h, albumList_FlagRRect.r);
		if (rect.leftClicked())
		{
			if (getData().selectedMusicIndex != i && getData().selectedMusicIndex != -1) getData().MusicList[albumDir][getData().selectedMusicIndex].music.stop();
			getData().selectedMusicIndex = i;
			auto& newMusic = getData().MusicList[albumDir][getData().selectedMusicIndex].music;
			(newMusic.isPlaying() ? newMusic.pause() : newMusic.play());
		}
		rect = RoundRect(albumList_FavRRect.x, albumList_FavRRect.y + num * 39, albumList_FavRRect.w, albumList_FavRRect.h, albumList_FavRRect.r);
		if (rect.leftClicked())
		{
			(getData().isFav(albumName, music.name) ? getData().removeFav(albumName, music.name) : getData().addFav(albumName, albumDir, music.name, music.dir, music.comment, music.music));
		}
		rect = RoundRect(albumListCellRRect.x, albumListCellRRect.y + num * 39, albumListCellRRect.w, albumListCellRRect.h, albumListCellRRect.r);
		if (rect.leftClicked())
		{
			if (getData().selectedMusicIndex != i && getData().selectedMusicIndex != -1) getData().MusicList[albumDir][getData().selectedMusicIndex].music.stop();
			getData().selectedMusicIndex = i;
			auto& newMusic = getData().MusicList[albumDir][getData().selectedMusicIndex].music;
			if (!newMusic.isPlaying()) newMusic.play();
			changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
	}
}

// 描画
void Album::draw() const
{
	// 背景 描画
	backgroundImage.draw(0, barHeight);
	albumImageRRect.drawShadow({ 0,15 }, 32, 10);
	albumImageRRect.drawFrame(0, 3, Palette::Gray);
	albumImageRRect.draw(Color(32, 32, 32, 120));
	albumNameRRect.drawShadow({ 0,15 }, 32, 10);
	albumNameRRect.draw(Color(32, 32, 32, 120));
	albumCreatorRRect.drawShadow({ 0,15 }, 32, 10);
	albumCreatorRRect.draw(Color(32, 32, 32, 120));
	albumExplRRect.drawShadow({ 0,15 }, 32, 10);
	albumExplRRect.draw(Color(32, 32, 32, 120));
	if (MusicListFirstIndex > 0)
	{
		goUpButton.draw((goUpButton.mouseOver() ? Palette::Orange : Palette::White));
		goUpButton.drawFrame(2, Palette::Black);
	}
	if (MusicListFirstIndex + 5 < (signed)getData().MusicList[albumDir].size())
	{
		goDownButton.draw((goDownButton.mouseOver() ? Palette::Orange : Palette::White));
		goDownButton.drawFrame(2, Palette::Black);
	}
	for (int i = 0; i < 5; ++i)
	{
		RoundRect(albumList_FlagRRect.x, albumList_FlagRRect.y + i * 39, albumList_FlagRRect.w, albumList_FlagRRect.h, albumList_FlagRRect.r).draw(Color(32, 32, 32, 200));
		RoundRect(albumList_NameRRect.x, albumList_NameRRect.y + i * 39, albumList_NameRRect.w, albumList_NameRRect.h, albumList_NameRRect.r).draw(Color(32, 32, 32, 200));
		RoundRect(albumList_TimeRRect.x, albumList_TimeRRect.y + i * 39, albumList_TimeRRect.w, albumList_TimeRRect.h, albumList_TimeRRect.r).draw(Color(32, 32, 32, 200));
		RoundRect(albumList_FavRRect.x, albumList_FavRRect.y + i * 39, albumList_FavRRect.w, albumList_FavRRect.h, albumList_FavRRect.r).draw(Color(32, 32, 32, 200));
	}

	// アルバム情報 描画
	const Rect rect((int)37.5, (int)37.5 + barHeight, 225, 225);
	rect(albumImage).draw();
	rect.drawFrame(0, 2, Color(200, 200, 200));
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)albumNameRRect.x, (int)albumNameRRect.y, (int)albumNameRRect.w, (int)albumNameRRect.h));
	albumNameFont(albumName).draw(draw_albumNameX, 29 + barHeight);
	Graphics2D::SetScissorRect(Rect((int)albumCreatorRRect.x, (int)albumCreatorRRect.y, (int)albumCreatorRRect.w, (int)albumCreatorRRect.h));
	albumCreatorFont(albumCreator).draw(draw_albumCreatorX, 85 + barHeight);
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
	albumExpl_draw();
	albumNameRRect.drawFrame(0, 2, Palette::Gray);
	albumCreatorRRect.drawFrame(0, 2, Palette::Gray);
	albumExplRRect.drawFrame(0, 2, Palette::Gray);

	// 曲リスト 描画
	for (int i = MusicListFirstIndex; (i - MusicListFirstIndex) < Min<int>(5, (int)getData().MusicList[albumDir].size() - MusicListFirstIndex); ++i)
	{
		auto num = i - MusicListFirstIndex;
		auto music = getData().MusicList[albumDir][i];
		RoundRect tmpRRect(albumList_FlagRRect.x, albumList_FlagRRect.y + num * 39, albumList_FlagRRect.w, albumList_FlagRRect.h, albumList_FlagRRect.r);
		if (music.music.isPlaying()) pauseImage.drawAt(43, 318 + barHeight + num * 39, (tmpRRect.mouseOver() ? Palette::Orange : Palette::White));
		else playImage.drawAt(43, 318 + barHeight + num * 39, (tmpRRect.mouseOver() ? Palette::Orange : Palette::White));
		albumListFont(music.compressedName).draw(70, 304 + barHeight + num * 39);
		auto str = Format(Pad(music.totalTime / 60, { 2, U'0' }), U":", Pad(music.totalTime % 60, { 2, U'0' }));
		albumListFont(str).draw(610, 304 + barHeight + num * 39);
		tmpRRect = RoundRect(albumList_FavRRect.x, albumList_FavRRect.y + num * 39, albumList_FavRRect.w, albumList_FavRRect.h, albumList_FavRRect.r);
		((getData().isFav(albumName, music.name) || tmpRRect.mouseOver()) ? favImage : notFavImage).drawAt(725, 318 + barHeight + num * 39);
	}
}

// アルバム情報描画 更新
void Album::draw_albumDetails_update()
{
	auto rect = albumNameRRect;
	auto width = albumNameFont(albumName).region().w + rect.r;
	if (width > albumNameRRect.w)
	{
		if (!draw_albumNameStayFlag)
		{
			if (draw_albumNameX + width > rect.x + rect.w) draw_albumNameX = draw_albumNameDefaultX - (double)draw_moveXPerSec * draw_albumNameTime.ms() / 1000;
			else
			{
				draw_albumNameTime.restart();
				draw_albumNameStayFlag = true;
			}
		}
		if (draw_albumNameStayFlag)
		{
			if (draw_albumNameTime.ms() >= drawStayMillisec)
			{
				draw_albumNameTime.restart();
				if (draw_albumNameX == draw_albumNameDefaultX) draw_albumNameStayFlag = false;
				else draw_albumNameX = draw_albumNameDefaultX;
			}
		}
	}
	rect = albumCreatorRRect;
	width = albumCreatorFont(albumCreator).region().w + rect.r;
	if (width > albumCreatorRRect.w)
	{
		if (!draw_albumCreatorStayFlag)
		{
			if (draw_albumCreatorX + width > rect.x + rect.w) draw_albumCreatorX = draw_albumCreatorDefaultX - (double)draw_moveXPerSec * draw_albumCreatorTime.ms() / 1000;
			else
			{
				draw_albumCreatorTime.restart();
				draw_albumCreatorStayFlag = true;
			}
		}
		if (draw_albumCreatorStayFlag)
		{
			if (draw_albumCreatorTime.ms() >= drawStayMillisec)
			{
				draw_albumCreatorTime.restart();
				if (draw_albumCreatorX == draw_albumCreatorDefaultX) draw_albumCreatorStayFlag = false;
				else draw_albumCreatorX = draw_albumCreatorDefaultX;
			}
		}
	}
	auto height = albumExplFont.height() * albumComment_Separeted.size();
	if (albumExplRRect.h < height)
	{
		if (!draw_albumExplStayFlag)
		{
			if (draw_albumExpl_y + height > albumExplRRect.y + albumExplRRect.h) draw_albumExpl_y = draw_albumExplDefaultY - (double)draw_moveYPerSec * draw_albumExplTime.ms() / (double)1000;
			else
			{
				draw_albumExplTime.restart();
				draw_albumExplStayFlag = true;
			}
		}
		if (draw_albumExplStayFlag)
		{
			if (draw_albumExplTime.ms() >= drawStayMillisec)
			{
				draw_albumExplTime.restart();
				if (draw_albumExpl_y == draw_albumExplDefaultY) draw_albumExplStayFlag = false;
				else draw_albumExpl_y = draw_albumExplDefaultY;
			}
		}
	}
}

// アルバム説明 描画
void Album::albumExpl_draw() const
{
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)albumExplRRect.x, (int)albumExplRRect.y, (int)albumExplRRect.w, (int)albumExplRRect.h));
	for (size_t i = 0; i < albumComment_Separeted.size(); ++i)
	{
		const int32 y = static_cast<int32>(draw_albumExpl_y + i * albumExplFont.height());
		albumExplFont(albumComment_Separeted[i]).draw(albumExplRRect.x + 10, y);
	}
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
}

// 曲名短縮
String Album::compressMusicName(String text) const
{
	const String dots(U"...");
	size_t fixedLength = 0;
	while (albumListFont(text.substr(0, fixedLength) + dots).region().w < albumList_NameRRect.w && fixedLength <= text.length()) ++fixedLength;
	--fixedLength;
	return (fixedLength == text.length() ? text : text.substr(0, fixedLength) + dots);
}

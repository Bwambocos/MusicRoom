// include
#include <Siv3D.hpp>
#include "Album.h"
#include "Select.h"

// アルバム詳細画面

// 初期化
Album::Album(const InitData& init) : IScene(init)
{
	// アセット管理登録
	FontAsset::Register(U"Album.albumName", 28, U"data\\fontR.ttc");
	FontAsset::Register(U"Album.albumCreator", 28, U"data\\fontR.ttc");
	FontAsset::Register(U"Album.albumExpl", 18, U"data\\fontR.ttc");
	FontAsset::Register(U"Album.albumList", 20, U"data\\fontR.ttc");
	FontAsset::Register(U"Album.header", 18, U"data\\fontR.ttc");
	TextureAsset::Register(U"Album.play", Icon(0xf144, 36));
	TextureAsset::Register(U"Album.pause", Icon(0xf28b, 36));
	TextureAsset::Register(U"Album.fav", Icon(0xf005, 36));
	TextureAsset::Register(U"Album.image", Icon(0xf03e, FontAsset(U"Album.header").height() - 8));
	TextureAsset::Register(U"Album.name", Icon(0xf02d, FontAsset(U"Album.header").height() - 8));
	TextureAsset::Register(U"Album.credit", Icon(0xf0c0, FontAsset(U"Album.header").height() - 8));
	TextureAsset::Register(U"Album.expl", Icon(0xf075, FontAsset(U"Album.header").height() - 8));
	TextureAsset::Register(U"Album.goUp", Icon(0xf139, 42));
	TextureAsset::Register(U"Album.goDown", Icon(0xf13a, 42));

	// シーン状況 更新
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Album";

	albumImageRect = Rect(Scene::Width() / 4 - 125, 35 + barHeight, 250, 250);
	albumNameRect = Rect(25, albumImageRect.y + albumImageRect.h + 35, Scene::Width() / 2 - 50, 54);
	albumCreatorRect = Rect(25, albumNameRect.y + albumNameRect.h + 35, albumNameRect.w, 48);
	albumExplRect = Rect(25, albumCreatorRect.y + albumCreatorRect.h + 35, albumNameRect.w, Scene::Height() - (albumCreatorRect.y + albumCreatorRect.h + 35) - 25);
	albumList_FlagRect = Rect(Scene::Width() / 2, albumImageRect.y, 36, 36);
	albumList_NameRect = Rect(albumList_FlagRect.x + albumList_FlagRect.w + 5, albumList_FlagRect.y, Scene::Width() / 2 - (36 * 2 + 5 * 2 + 25), 36);
	albumList_FavRect = Rect(albumList_NameRect.x + albumList_NameRect.w + 5, albumList_FlagRect.y, 36, 36);
	albumListAllRect = Rect(albumList_FlagRect.x, albumList_FlagRect.y, Scene::Width() / 2 - 25, 36 * (int)albumListRows + 5 * ((int)albumListRows - 1));
	audioWaveRect = Rect(25, 35 + barHeight, albumNameRect.w, albumImageRect.h);
	rectHeader = Quad(Vec2(0, 0), Vec2(160, 0), Vec2(160 + FontAsset(U"Album.header").height(), FontAsset(U"Album.header").height()), Vec2(0, FontAsset(U"Album.header").height()));
	goUpPos = Vec2(albumListAllRect.center().x, barHeight + 5 + TextureAsset(U"Album.goUp").height() / 2);
	goDownPos = Vec2(albumListAllRect.center().x, Scene::Height() - 5 - TextureAsset(U"Album.goDown").height() / 2);

	// 描画位置 初期化
	draw_albumNameTime.restart();
	draw_albumCreatorTime.restart();
	draw_albumExplTime.restart();
	draw_albumNameStayFlag = true;
	draw_albumCreatorStayFlag = true;
	draw_albumExplStayFlag = true;
	draw_albumNameX = albumNameRect.x + 5;
	draw_albumCreatorX = albumCreatorRect.x + 5;
	draw_albumExpl_y = albumExplRect.y + 5;

	// アルバム情報 初期化
	albumName = getData().AlbumList[getData().selectedAlbumIndex].name;
	albumDir = getData().AlbumList[getData().selectedAlbumIndex].dir;
	albumCreator = getData().AlbumList[getData().selectedAlbumIndex].creator;
	albumComment = getData().AlbumList[getData().selectedAlbumIndex].comment;
	MusicListFirstIndex = 0;
	albumComment_Separeted.clear();
	size_t pos = 0;
	while (pos < (int)albumComment.length())
	{
		for (int i = 0; i + pos < (int)albumComment.length(); ++i)
		{
			if (FontAsset(U"Album.albumExpl")(albumComment.substr(pos, i)).region().w >= albumExplRect.w - 10)
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
			AudioAsset::Register(U"album-" + albumDir + U".music-" + musicDir, U"music\\" + albumDir + U"\\" + musicDir + U"\\" + musicDir + U".mp3");
			TextReader reader2(U"music\\" + albumDir + U"\\" + musicDir + U"\\" + musicDir + U".txt");
			reader2.readLine(musicName);
			String temp; while (reader2.readLine(temp)) musicComment += temp + U"\n";
			getData().MusicList[albumDir].push_back({ Audio(), musicName, musicDir, musicComment, U"", 0 });
		}
	}
}

// 更新
void Album::update()
{
	// アルバム情報 更新
	draw_albumDetails_update();

	// 曲リスト 更新
	if (TextureAsset(U"Album.goUp").regionAt(goUpPos).leftClicked()) --MusicListFirstIndex;
	if (TextureAsset(U"Album.goDown").regionAt(goDownPos).leftClicked()) ++MusicListFirstIndex;
	if (albumListAllRect.mouseOver()) MusicListFirstIndex += (int)Mouse::Wheel();
	MusicListFirstIndex = Max(MusicListFirstIndex, 0);
	MusicListFirstIndex = Min<int>(MusicListFirstIndex, Max((int)getData().MusicList[albumDir].size() - (int)albumListRows, 0));
	for (int i = MusicListFirstIndex; i - MusicListFirstIndex < Min((int)albumListRows, (signed)getData().MusicList[albumDir].size()) && i < (signed)getData().MusicList[albumDir].size(); ++i)
	{
		auto num = i - MusicListFirstIndex;
		auto music = getData().MusicList[albumDir][i];
		if (albumList_FlagRect.movedBy(0, num * 41).leftClicked())
		{
			if (getData().selectedMusicIndex != i && getData().selectedMusicIndex != -1) AudioAsset(U"album-" + albumDir + U".music-" + getData().MusicList[albumDir][getData().selectedMusicIndex].dir).stop();
			getData().selectedMusicIndex = i;
			auto newMusic = AudioAsset(U"album-" + albumDir + U".music-" + getData().MusicList[albumDir][getData().selectedMusicIndex].dir);
			if (newMusic.isPlaying()) newMusic.pause();
			else newMusic.play();
		}
		if (albumList_FavRect.movedBy(0, num * 41).leftClicked())
		{
			(getData().isFav(albumName, music.name) ? getData().removeFav(albumName, music.name) : getData().addFav(albumName, albumDir, albumCreator, albumComment, music.name, music.dir, music.comment, music.music));
		}
		if (albumList_NameRect.movedBy(0, num * 41).leftClicked())
		{
			if (getData().selectedMusicIndex != i && getData().selectedMusicIndex != -1) AudioAsset(U"album-" + albumDir + U".music-" + getData().MusicList[albumDir][getData().selectedMusicIndex].dir).stop();
			getData().selectedMusicIndex = i;
			auto newMusic = AudioAsset(U"album-" + albumDir + U".music-" + getData().MusicList[albumDir][getData().selectedMusicIndex].dir);
			if (!newMusic.isPlaying()) newMusic.play();
			changeScene(U"Music", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
		}
	}
	if (getData().selectedMusicIndex != -1 && AudioAsset(U"album-" + albumDir + U".music-" + getData().MusicList[albumDir][getData().selectedMusicIndex].dir).isPlaying()) FFT::Analyze(musicFFT, AudioAsset(U"album-" + albumDir + U".music-" + getData().MusicList[albumDir][getData().selectedMusicIndex].dir));
}

// 描画
void Album::draw() const
{
	// アルバム情報 描画
	if (!musicFFT.buffer.empty()) audioWaveRect.drawFrame(3, getData().schemeColor2);
	drawStrBackground(albumImageRect, U"アルバム画像", U"Album.image");
	drawStrBackground(albumNameRect, U"アルバム名", U"Album.name");
	drawStrBackground(albumCreatorRect, U"クレジット", U"Album.credit");
	drawStrBackground(albumExplRect, U"アルバム説明", U"Album.expl");
	albumImageRect
		.stretched(-5)
		(TextureAsset(U"album-" + albumDir + U".image")).draw();
	if (!musicFFT.buffer.empty()) for (auto i : step(118)) RectF(audioWaveRect.x + audioWaveRect.w / 118 * i, audioWaveRect.y + audioWaveRect.h, audioWaveRect.w / 118, -Pow(musicFFT.buffer[i], 0.3) * audioWaveRect.h).draw(ColorF(getData().schemeColor3, 0.5));
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(albumNameRect);
	FontAsset(U"Album.albumName")(albumName).draw(draw_albumNameX, albumNameRect.y + 3, getData().stringColor);
	Graphics2D::SetScissorRect(albumCreatorRect);
	FontAsset(U"Album.albumCreator")(albumCreator).draw(draw_albumCreatorX, albumCreatorRect.y + 3, getData().stringColor);
	Graphics2D::SetScissorRect(Scene::Rect());
	albumExpl_draw();

	// 曲リスト 描画
	if (MusicListFirstIndex > 0) TextureAsset(U"Album.goUp").drawAt(goUpPos, (TextureAsset(U"Album.goUp").regionAt(goUpPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
	if (MusicListFirstIndex + albumListRows < (signed)getData().MusicList[albumDir].size()) TextureAsset(U"Album.goDown").drawAt(goDownPos, (TextureAsset(U"Album.goDown").regionAt(goDownPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
	for (auto i : step(albumListRows))
	{
		drawButton(albumList_FlagRect.movedBy(0, (int)i * 41), false);
		drawButton(albumList_NameRect.movedBy(0, (int)i * 41), false);
		drawButton(albumList_FavRect.movedBy(0, (int)i * 41), false);
	}
	for (int i = MusicListFirstIndex; (i - MusicListFirstIndex) < Min((int)albumListRows, (int)getData().MusicList[albumDir].size() - MusicListFirstIndex); ++i)
	{
		auto num = i - MusicListFirstIndex;
		auto music = getData().MusicList[albumDir][i];
		auto rect = albumList_FlagRect.movedBy(0, num * 41);
		if (i == getData().selectedMusicIndex && AudioAsset(U"album-" + albumDir + U".music-" + music.dir).isPlaying()) TextureAsset(U"Album.pause").drawAt(rect.center(), (rect.mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
		else TextureAsset(U"Album.play").drawAt(rect.center(), (rect.mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
		FontAsset(U"Album.albumList")(music.name).draw(albumList_NameRect.movedBy(0, num * 41).stretched(-5, -1), getData().stringColor);
		rect = albumList_FavRect.movedBy(0, num * 41);
		TextureAsset(U"Album.fav").drawAt(rect.center(), (getData().isFav(albumName, music.name) || rect.mouseOver()) ? Palette::Gold : getData().schemeColor4);
	}
}

// アルバム情報描画 更新
void Album::draw_albumDetails_update()
{
	auto rect = albumNameRect;
	auto width = FontAsset(U"Album.albumName")(albumName).region().w;
	if (width > albumNameRect.w)
	{
		if (!draw_albumNameStayFlag)
		{
			if (draw_albumNameX + width > rect.x + rect.w) draw_albumNameX = (albumNameRect.x + 5) - (double)draw_moveXPerSec * draw_albumNameTime.ms() / 1000;
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
				if (draw_albumNameX == albumNameRect.x + 5) draw_albumNameStayFlag = false;
				else draw_albumNameX = albumNameRect.x + 5;
			}
		}
	}
	rect = albumCreatorRect;
	width = FontAsset(U"Album.albumCreator")(albumCreator).region().w;
	if (width > albumCreatorRect.w)
	{
		if (!draw_albumCreatorStayFlag)
		{
			if (draw_albumCreatorX + width > rect.x + rect.w) draw_albumCreatorX = albumCreatorRect.x + 5 - (double)draw_moveXPerSec * draw_albumCreatorTime.ms() / 1000;
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
				if (draw_albumCreatorX == albumCreatorRect.x + 5) draw_albumCreatorStayFlag = false;
				else draw_albumCreatorX = albumCreatorRect.x + 5;
			}
		}
	}
	auto height = FontAsset(U"Album.albumExpl").height() * albumComment_Separeted.size();
	if (albumExplRect.h < height)
	{
		if (!draw_albumExplStayFlag)
		{
			if (draw_albumExpl_y + height > albumExplRect.y + albumExplRect.h) draw_albumExpl_y = albumExplRect.y + 5 - (double)draw_moveYPerSec * draw_albumExplTime.ms() / (double)1000;
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
				if (draw_albumExpl_y == albumExplRect.y + 5) draw_albumExplStayFlag = false;
				else draw_albumExpl_y = albumExplRect.y + 5;
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
	Graphics2D::SetScissorRect(albumExplRect);
	for (size_t i = 0; i < albumComment_Separeted.size(); ++i)
	{
		const int32 y = static_cast<int32>(draw_albumExpl_y + i * FontAsset(U"Album.albumExpl").height());
		FontAsset(U"Album.albumExpl")(albumComment_Separeted[i]).draw(albumExplRect.x + 10, y, getData().stringColor);
	}
	Graphics2D::SetScissorRect(Scene::Rect());
}

// ボタン描画
void Album::drawButton(Rect rect, bool highlight) const
{
	rect
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(getData().schemeColor3)
		.drawFrame(0, 1.5, ColorF((rect.mouseOver() || highlight ? getData().schemeColor5 : getData().schemeColor4), 0.6 + (rect.mouseOver() ? 1 : 0) * 0.4));
	rect.draw(getData().schemeColor3);
}

// 文字背景描画
void Album::drawStrBackground(Rect rect, String header, String icon) const
{
	rectHeader
		.movedBy(rect.x, rect.y - FontAsset(U"Album.header").height() + 3)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
	TextureAsset(icon).drawAt(rect.x + FontAsset(U"Album.header").height() / 2 + 5, rect.y - FontAsset(U"Album.header").height() / 2 + 3, getData().stringColor);
	FontAsset(U"Album.header")(header).draw(rect.x + FontAsset(U"Album.header").height() + 5, rect.y - FontAsset(U"Album.header").height() + 3, getData().stringColor);
	rect
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
}

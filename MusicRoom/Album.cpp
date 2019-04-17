// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Album.h"
#include "Select.h"

// アルバム詳細画面

// 初期化
Album::Album(const InitData& init) : IScene(init)
{
	if (!main)
	{
		main = Texture(U"data\\Detail\\main.png");
		playing = Texture(U"data\\Detail\\playing.png");
		pausing = Texture(U"data\\Detail\\pausing.png");
		not_fav = Texture(U"data\\Detail\\not_fav.png");
		fav = Texture(U"data\\Detail\\fav.png");
	}
	albumBName = getSetAlbum().second;

	// アルバム情報 初期化
	albumName.clear(); albumCreator.clear(); albumExpl.clear(); albumList.clear();
	albumImg = Texture(U"music\\" + albumBName + U"\\" + albumBName + U".png");
	TextReader reader(U"music\\" + albumBName + U"\\" + albumBName + U".txt");
	reader.readLine(albumName); reader.readLine(albumCreator);
	String temp; while (reader.readLine(temp)) albumExpl += temp + U"\n";
	font_albumName = Font(24);
	font_albumCreator = Font(18);
	font_albumExpl = Font(12);
	if (get_prevScene() != Scene_Music) albumList_begin = 0;

	// まだ読み込まれていないアルバム
	if (albums.find(albumBName) == albums.end() || reloadFlag)
	{
		// 曲リスト 初期化
		font_albumList = Font(16);
		const String extensions[] = { U".wav", U".ogg", U".mp3" };
		TextReader reader(U"music\\" + albumBName + U"\\music_list.txt");
		String fileName, tempName; Audio tempMusic; int temp_totalTime;
		while (reader.readLine(fileName))
		{
			for (auto ext : extensions)
			{
				if (FileSystem::IsFile(U"music\\" + albumBName + U"\\" + fileName + U"\\" + fileName + ext))
				{
					tempMusic = Audio(U"music\\" + albumBName + U"\\" + fileName + U"\\" + fileName + ext);
					break;
				}
			}
			TextReader tempReader(U"music\\" + albumBName + U"\\" + fileName + U"\\" + fileName + U".txt");
			tempReader.readLine(tempName);
			if (!tempMusic) tempName = U"！読み込み失敗！";
			temp_totalTime = (int)tempMusic.lengthSec();
			albumList.push_back({ tempMusic,Detail_musicNameBeShort(tempName),tempName,fileName,temp_totalTime });
		}
		albums[albumBName] = albumList;
	}
	else albumList = albums[albumBName];

	// 描画位置 初期化
	draw_albumName_startMSec = Time::GetMillisec();
	draw_albumCreator_startMSec = Time::GetMillisec();
	draw_albumExpl_startMSec = Time::GetMillisec();
	draw_albumName_stayFlag = true;
	draw_albumCreator_stayFlag = true;
	draw_albumExpl_stayFlag = true;
	draw_albumName_x = DEFAULT_albumName_X;
	draw_albumCreator_x = DEFAULT_albumCreator_X;
	draw_albumExpl_y = DEFAULT_albumExpl_Y;
	reloadFlag = false;

	rect_albumImage = RoundRect(25, 25 + BAR_HEIGHT, 250, 250, 12.5);
	rect_albumName = RoundRect(325, 25 + BAR_HEIGHT, 393, 54, 10);
	rect_albumCreator = RoundRect(325, 82 + BAR_HEIGHT, 393, 48, 10);
	rect_albumExpl = RoundRect(325, 133 + BAR_HEIGHT, 393, 142, 10);
	rect_albumList_Flag = RoundRect(25, 300 + BAR_HEIGHT, 36, 36, 5);
	rect_albumList_Name = RoundRect(64, 300 + BAR_HEIGHT, 537, 36, 5);
	rect_albumList_Time = RoundRect(604, 300 + BAR_HEIGHT, 100, 36, 5);
	rect_albumList_Fav = RoundRect(707, 300 + BAR_HEIGHT, 36, 36, 5);
	rect_albumListAll = RoundRect(25, 300 + BAR_HEIGHT, 718, 190, 5);
	rect_albumListCell = RoundRect(64, 300 + BAR_HEIGHT, 582, 36, 5);
	goUp = Triangle({ 384,350 }, { 414,360 }, { 354,360 });
	goDown = Triangle({ 354,560 }, { 414,560 }, { 384,570 });
}

// 更新
void Album::update()
{
	if (KeyB.pressed()) changeScene(U"Select");
	if (KeyF5.pressed()) changeScene(U"Album");

	// アルバム情報 更新
	Update_drawAlbumDetailStrings();
	Album::albumExpl_Update();

	// 曲リスト 更新
	if (goUp.leftClicked) --albumList_begin;
	if (goDown.leftClicked) ++albumList_begin;
	if (rect_albumListAll.mouseOver)
	{
		if ((albumList_begin + 5 <= (signed)albumList.size()) || (albumList_begin > 0)) albumList_begin += Mouse::Wheel();
	}
	albumList_begin = Max(albumList_begin, 0);
	albumList_begin = Min<int>(albumList_begin, Max<int>((int)albumList.size() - 5, 0));
	for (int i = albumList_begin; ((i - albumList_begin) < Min<int>(5, (signed)albumList.size())) && (i < (signed)albumList.size()); ++i)
	{
		auto num = i - albumList_begin;
		auto music = albumList[i];
		RoundRect rect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
		if (rect.leftClicked)
		{
			if (selectedMusic_num != i && selectedMusic_num < (signed)albumList.size()) albumList[selectedMusic_num].music.stop();
			(music.music.isPlaying() ? music.music.pause() : music.music.play());
			selectedMusic_num = i;
			selectedAlbumName = albumName;
			selectedAlbumBName = albumBName;
			selectedMusicName = music.fileName;
			selectedMusic = music.music;
			giveMusicData(albumName, music.originName, music.music);
		}
		rect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
		if (rect.leftClicked)
		{
			(isFav(albumName, music.originName) ? removeFav(albumName, music.originName) : addFav(albumName, albumBName, music.originName, music.fileName, music.music));
		}
		rect = RoundRect(rect_albumListCell.x, rect_albumListCell.y + num * 39, rect_albumListCell.w, rect_albumListCell.h, rect_albumListCell.r);
		if (rect.leftClicked)
		{
			if (selectedMusic_num != i && selectedMusic_num < (signed)albumList.size()) albumList[selectedMusic_num].music.stop();
			selectedMusic_num = i;
			selectedAlbumName = albumName;
			selectedAlbumBName = albumBName;
			selectedMusicName = music.fileName;
			selectedMusic = music.music;
			set_stillFlag(true);
			changeScene(U"Music");
		}
	}
}

// 描画
void Album::draw() const
{
	// 背景 描画
	main.draw(0, BAR_HEIGHT);
	rect_albumImage.drawShadow({ 0,15 }, 32, 10);
	rect_albumImage.drawFrame(0, 3, Palette::Gray);
	rect_albumImage.draw(Color(32, 32, 32, 120));
	rect_albumName.drawShadow({ 0,15 }, 32, 10);
	rect_albumName.draw(Color(32, 32, 32, 120));
	rect_albumCreator.drawShadow({ 0,15 }, 32, 10);
	rect_albumCreator.draw(Color(32, 32, 32, 120));
	rect_albumExpl.drawShadow({ 0,15 }, 32, 10);
	rect_albumExpl.draw(Color(32, 32, 32, 120));
	if (albumList_begin > 0)
	{
		goUp.draw((goUp.mouseOver ? Palette::Orange : Palette::White));
		goUp.drawFrame(2, Palette::Black);
	}
	if (albumList_begin + 5 < (signed)albumList.size())
	{
		goDown.draw((goDown.mouseOver ? Palette::Orange : Palette::White));
		goDown.drawFrame(2, Palette::Black);
	}
	for (int i = 0; i < 5; ++i)
	{
		RoundRect(rect_albumList_Flag.x, rect_albumList_Flag.y + i * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Name.x, rect_albumList_Name.y + i * 39, rect_albumList_Name.w, rect_albumList_Name.h, rect_albumList_Name.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Time.x, rect_albumList_Time.y + i * 39, rect_albumList_Time.w, rect_albumList_Time.h, rect_albumList_Time.r).draw(Color(32, 32, 32, 200));
		RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + i * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r).draw(Color(32, 32, 32, 200));
	}

	// アルバム情報 描画
	const Rect rect((int)37.5, (int)37.5 + BAR_HEIGHT, 225, 225);
	rect(albumImg).draw();
	rect.drawFrame(0, 2, Color(200, 200, 200));
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)rect_albumName.x, (int)rect_albumName.y, (int)rect_albumName.w, (int)rect_albumName.h));
	font_albumName(albumName).draw(draw_albumName_x, 27 + BAR_HEIGHT);
	Graphics2D::SetScissorRect(Rect((int)rect_albumCreator.x, (int)rect_albumCreator.y, (int)rect_albumCreator.w, (int)rect_albumCreator.h));
	font_albumCreator(albumCreator).draw(draw_albumCreator_x, 88 + BAR_HEIGHT);
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
	Album::albumExpl_Draw();
	rect_albumName.drawFrame(0, 2, Palette::Gray);
	rect_albumCreator.drawFrame(0, 2, Palette::Gray);
	rect_albumExpl.drawFrame(0, 2, Palette::Gray);

	// 曲リスト 描画
	for (int i = albumList_begin; (i - albumList_begin) < Min<int>(5, (int)albumList.size() - albumList_begin); ++i)
	{
		auto num = i - albumList_begin;
		auto tmp = albumList[i];
		RoundRect tmpRRect(rect_albumList_Flag.x, rect_albumList_Flag.y + num * 39, rect_albumList_Flag.w, rect_albumList_Flag.h, rect_albumList_Flag.r);
		if (tmp.music.isPlaying()) pausing.drawAt(43, 318 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White));
		else playing.drawAt(43, 318 + BAR_HEIGHT + num * 39, (tmpRRect.mouseOver ? Palette::Orange : Palette::White));
		font_albumList(tmp.displayName).draw(70, 304 + BAR_HEIGHT + num * 39);
		auto str = Format(Pad(tmp.totalTime / 60, { 2,L'0' }), L":", Pad(tmp.totalTime % 60, { 2,L'0' }));
		font_albumList(str).draw(610, 304 + BAR_HEIGHT + num * 39);
		tmpRRect = RoundRect(rect_albumList_Fav.x, rect_albumList_Fav.y + num * 39, rect_albumList_Fav.w, rect_albumList_Fav.h, rect_albumList_Fav.r);
		((isFav(albumName, tmp.originName) || tmpRRect.mouseOver) ? fav : not_fav).drawAt(725, 318 + BAR_HEIGHT + num * 39);
	}
}

// アルバム説明 更新
void Album::albumExpl_Update()
{
	size_t pos = 0;
	const int32 w = (int32)rect_albumExpl.w - 10;
	const int32 h = (int32)rect_albumExpl.h;
	while (pos < albumExpl.length)
	{
		for (int i = 0; i + pos < albumExpl.length; ++i)
		{
			if (font_albumExpl(albumExpl.substr(pos, i)).region().w >= w)
			{
				texts.push_back(albumExpl.substr(pos, i - 1));
				pos += (i - 1);
				break;
			}
			if (albumExpl[pos + i] == L'\n')
			{
				texts.push_back(albumExpl.substr(pos, i + 1));
				pos += (i + 1);
				break;
			}
		}
	}
	auto height = font_albumExpl.height * texts.size();
	if (rect_albumExpl.h < height)
	{
		if (!draw_albumExpl_stayFlag)
		{
			if (draw_albumExpl_y + height > rect_albumExpl.y + rect_albumExpl.h) draw_albumExpl_y -= (double)DRAW_MOVE_Y_PER_SEC * (Time::GetMillisec() - draw_albumExpl_stayMSec) / (double)1000;
			else
			{
				draw_albumExpl_startMSec = draw_albumExpl_stayMSec = Time::GetMillisec();
				draw_albumExpl_stayFlag = true;
			}
		}
		if (draw_albumExpl_stayFlag)
		{
			if (draw_albumExpl_stayMSec - draw_albumExpl_startMSec >= DRAW_STAYMSEC)
			{
				draw_albumExpl_startMSec = draw_albumExpl_stayMSec;
				if (draw_albumExpl_y == DEFAULT_albumExpl_Y) draw_albumExpl_stayFlag = false;
				else draw_albumExpl_y = DEFAULT_albumExpl_Y;
			}
		}
	}
	draw_albumExpl_stayMSec = Time::GetMillisec();
}

// アルバム説明 描画
void Album::albumExpl_Draw() const
{
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)rect_albumExpl.x, (int)rect_albumExpl.y, (int)rect_albumExpl.w, (int)rect_albumExpl.h));
	for (size_t i = 0; i < texts.size(); ++i)
	{
		const int32 y = static_cast<int32>(draw_albumExpl_y + i * font_albumExpl.height);
		font_albumExpl(texts[i]).draw(rect_albumExpl.x + 10, y);
	}
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
}

// アルバム・曲情報 受け渡し（flag == 1 -> 次 : -1 -> 前）
void Album::setAlbumMusicName(String & album_Name, String & album_BName, String & musicName, Audio & musicData)
{
	album_Name = getSetAlbum().first;
	album_BName = getSetAlbum().second;
	musicName = selectedMusicName;
	musicData = selectedMusic;
}
void Album::setAlbumMusicName(int flag, String & album_Name, String & album_BName, String & musicName, Audio & music)
{
	if (selectedMusic_num + flag >= (int)albumList.size())
	{
		getNextAlbum();
		selectedAlbumName = getSetAlbum().first;
		selectedAlbumBName = getSetAlbum().second;
		selectedMusic_num = -1;
		changeScene(U"Album");
	}
	selectedMusic_num = (selectedMusic_num + flag + (int)albumList.size()) % (int)albumList.size();
	const auto data = albumList[selectedMusic_num];
	album_Name = selectedAlbumName;
	album_BName = selectedAlbumBName;
	musicName = selectedMusicName = data.fileName;
	music = selectedMusic = data.music;
	changeMusicList_Begin();
}

// 各文字列 描画
void Album::Update_drawAlbumDetailStrings()
{
	auto rect = rect_albumName;
	auto width = font_albumName(albumName).region().w + rect.r;
	if (width > rect_albumName.w)
	{
		if (!draw_albumName_stayFlag)
		{
			if (draw_albumName_x + width > rect.x + rect.w) draw_albumName_x -= (double)DRAW_MOVE_X_PER_SEC * (Time::GetMillisec() - draw_albumName_stayMSec) / 1000;
			else
			{
				draw_albumName_startMSec = draw_albumName_stayMSec = Time::GetMillisec();
				draw_albumName_stayFlag = true;
			}
		}
		if (draw_albumName_stayFlag)
		{
			if (draw_albumName_stayMSec - draw_albumName_startMSec >= DRAW_STAYMSEC)
			{
				draw_albumName_startMSec = draw_albumName_stayMSec;
				if (draw_albumName_x == DEFAULT_albumName_X) draw_albumName_stayFlag = false;
				else draw_albumName_x = DEFAULT_albumName_X;
			}
		}
		draw_albumName_stayMSec = Time::GetMillisec();
	}
	rect = rect_albumCreator;
	width = font_albumCreator(albumCreator).region().w + rect.r;
	if (width > rect_albumCreator.w)
	{
		if (!draw_albumCreator_stayFlag)
		{
			if (draw_albumCreator_x + width > rect.x + rect.w) draw_albumCreator_x -= (double)DRAW_MOVE_X_PER_SEC * (Time::GetMillisec() - draw_albumCreator_stayMSec) / 1000;
			else
			{
				draw_albumCreator_startMSec = draw_albumCreator_stayMSec = Time::GetMillisec();
				draw_albumCreator_stayFlag = true;
			}
		}
		if (draw_albumCreator_stayFlag)
		{
			if (draw_albumCreator_stayMSec - draw_albumCreator_startMSec >= DRAW_STAYMSEC)
			{
				draw_albumCreator_startMSec = draw_albumCreator_stayMSec;
				if (draw_albumCreator_x == DEFAULT_albumCreator_X) draw_albumCreator_stayFlag = false;
				else draw_albumCreator_x = DEFAULT_albumCreator_X;
			}
		}
		draw_albumCreator_stayMSec = Time::GetMillisec();
	}
}

// 曲名短縮
String Album::Detail_musicNameBeShort(String text)
{
	static const String dots(U"...");
	const double dotsWidth = font_albumList(dots).region().w;
	// const size_t num_chars = font_albumList.drawableCharacters(text, rect_albumList_Name.w - dotsWidth);
	const size_t num_chars = 25;

	if (font_albumList(text).region().w <= rect_albumList_Name.w) return text;
	if (dotsWidth > rect_albumList_Name.w) return String();
	return text.substr(0, num_chars) + dots;
}

// 曲操作
// kind: 0->一時停止, 1->再生, 2->停止
void Album::setMusicStats(int kind)
{
	if (albumList.empty()) return;
	switch (kind)
	{
	case 0:
		albumList[selectedMusic_num].music.pause();
		break;
	case 1:
		albumList[selectedMusic_num].music.play();
		break;
	case 2:
		albumList[selectedMusic_num].music.stop();
		break;
	}
}

// 曲リスト開始位置 操作
void Album::changeMusicList_Begin()
{
	albumList_begin = Max(albumList_begin, selectedMusic_num - 4);
	albumList_begin = Min(albumList_begin, selectedMusic_num);
}

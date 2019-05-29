// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"
#include "Select.h"

// アルバム選択画面

// 初期化
Select::Select(const InitData& init) : IScene(init)
{
	// AlbumList 読み込み
	if (getData().AlbumList.empty())
	{
		String temp;
		TextReader reader = TextReader(U"music\\album_list.txt");
		while (reader.readLine(temp))
		{
			String name, creator, comment;
			TextReader text(U"music\\" + temp + U"\\" + temp + U".txt");
			text.readLine(name);
			text.readLine(creator);
			String temp_of_temp;
			while (text.readLine(temp_of_temp)) comment += temp_of_temp + U"\n";
			Texture image(U"music\\" + temp + U"\\" + temp + U".png");
			if (!image) image = no_img;
			getData().AlbumList.push_back({ name,temp,creator,comment,image });
		}
	}

	if (!bgImage)
	{
		comTime.resize(getData().AlbumList.size() + 8);
		bgImage = Texture(U"data\\bgImage.png");
		fav = Texture(U"data\\Select\\fav.png");
		no_img = Texture(U"data\\Select\\no_img.png");
		z = Grid<double>(3, (getData().AlbumList.size() + 1) / 3 + 3);
		goUp = Triangle({ 384,75 }, { 414,85 }, { 354,85 });
		goDown = Triangle({ 354,560 }, { 414,560 }, { 384,570 });
	}

	startTime = (int)Time::GetMillisec();

	if (getData().nowScene == U"Fav" && !getData().FavMusicList.empty() && getData().selectedFavMusicIndex != -1) getData().FavMusicList[getData().selectedFavMusicIndex].music.stop();
	if (getData().nowScene == U"Album" && getData().selectedMusicIndex != -1) getData().MusicList[getData().AlbumList[getData().selectedAlbumIndex].dir][getData().selectedMusicIndex].music.stop();
	getData().selectedAlbumIndex = -1;
	getData().selectedMusicIndex = -1;
	getData().selectedFavMusicIndex = -1;

	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Select";
}

// 更新
void Select::update()
{
	nowTime = (int)Time::GetMillisec();
	if (KeyF5.pressed()) changeScene(U"Select", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);

	// スクロール 更新
	if (scrollFlag == 0)
	{
		if (first_cnt > 0 && (goUp.leftClicked() || Mouse::Wheel() < 0)) scrollFlag = 1;
		if (first_cnt + 5 <= (signed)getData().AlbumList.size() && (goDown.leftClicked() || Mouse::Wheel() > 0)) scrollFlag = 2;
		if (scrollFlag != 0) scrollNowTime = scrollStartTime = (int)Time::GetMillisec();
	}
	else
	{
		if (scrollNowTime - scrollStartTime > SCROLL_MSEC)
		{
			first_cnt += (scrollFlag == 1 ? -3 : 3);
			scrollY = scrollFlag = 0;
			first_cnt = Max(first_cnt, 0);
			first_cnt = Min<int>(first_cnt, (int)getData().AlbumList.size() / 3 * 3);
		}
		else scrollY = (scrollFlag == 1 ? 246 : -246) * ((scrollNowTime - scrollStartTime)) / SCROLL_MSEC;
		scrollNowTime = (int)Time::GetMillisec();
	}

	// album_list 更新
	int cnt = first_cnt - 3;
	for (int y = -1; y <= (signed)z.height(); ++y)
	{
		for (int x = 0; x < (signed)z.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += scrollY;
			if (cnt >= 0)
			{
				if (rect.mouseOver())
				{
					comTime[cnt].first = (comTime[cnt].first == 0 ? (int)Time::GetMillisec() : comTime[cnt].first);
					comTime[cnt].second = (int)Time::GetMillisec();
					if (MouseL.down())
					{
						if (cnt == (signed)getData().AlbumList.size()) changeScene(U"Fav", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
						else
						{
							getData().selectedAlbumIndex = cnt;
							changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
						}
					}
				}
				else
				{
					z[y + 1][x + 1] = Max(z[y + 1][x + 1] - 0.02, 0.0);
					comTime[cnt].first = comTime[cnt].second = 0;
				}
			}
			if (rect.mouseOver()) z[y + 1][x + 1] = Min(z[y + 1][x + 1] + 0.05, 0.5);
			++cnt;
			if (cnt == (signed)getData().AlbumList.size() + 1) break;
		}
		if (cnt == (signed)getData().AlbumList.size() + 1) break;
	}
}

// 描画
void Select::draw() const
{
	// メイン背景
	bgImage.draw(0, BAR_HEIGHT);
	if (first_cnt > 0)
	{
		goUp.draw((goUp.mouseOver() ? Palette::Orange : Palette::White));
		goUp.drawFrame(2, Palette::Black);
	}
	if (first_cnt + 5 <= (signed)getData().AlbumList.size())
	{
		goDown.draw((goDown.mouseOver() ? Palette::Orange : Palette::White));
		goDown.drawFrame(2, Palette::Black);
	}

	// album_list 描画
	int cnt = first_cnt - 3;
	for (int y = -1; y <= (signed)z.height(); ++y)
	{
		for (int x = 0; x < (signed)z.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += scrollY;
			const double s = z[y + 1][x + 1];
			if (cnt >= 0)
			{
				RectF(rect).stretched(s * 2).drawShadow({ 0,15 * s }, 32 * s, 10 * s);
				RectF(rect).stretched(s * 2)(Select::getSelectedImage(cnt).resized(216, 216)).draw();
				RectF(rect).stretched(s * 2).drawFrame(3, 0, Color(0, 0, 0));
			}
			++cnt;
			if (cnt == (signed)getData().AlbumList.size() + 1) break;
		}
		if (cnt == (signed)getData().AlbumList.size() + 1) break;
	}
	cnt = first_cnt;
	for (int y = 0; y <= (signed)z.height(); ++y)
	{
		for (int x = 0; x < (signed)z.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += scrollY;
			if (rect.mouseOver() && comTime[cnt].second - comTime[cnt].first >= COM_MESSAGE_MILLISEC) drawDetails(cnt);
			++cnt;
			if (cnt == (signed)getData().AlbumList.size() + 1) break;
		}
		if (cnt == (signed)getData().AlbumList.size() + 1) break;
	}
}

// アルバム一覧 正方形区画を作成
Rect Select::makeRect(int x, int y) const
{
	return { 30 + x * 246, BAR_HEIGHT + 30 + y * 246, 216, 216 };
}

// アルバム画像を返す
Texture Select::getSelectedImage(int cnt) const
{
	if (cnt < 0) return Texture();
	return (cnt < (signed)getData().AlbumList.size() ? getData().AlbumList[cnt].image : fav);
}

// アルバム詳細 描画
void Select::drawDetails(int cnt) const
{
	const Point pos = Cursor::Pos();
	static Font font(16);
	static String name, creator;
	if (cnt == (signed)getData().AlbumList.size())
	{
		name = U"お気に入り";
		creator = U"お気に入り登録した曲を表示します。";
	}
	else
	{
		name = getData().AlbumList[cnt].name;
		creator = getData().AlbumList[cnt].creator;
	}
	const auto width = Max(font(name).region().w, font(creator).region().w);
	static int x_addtion;
	if (cnt % 3 == 0) x_addtion = 13;
	if (cnt % 3 == 1) x_addtion = (-width / 2);
	if (cnt % 3 == 2) x_addtion = -width;
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawShadow({ 0,15 }, 32, 10);
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).draw(Color({ 255,255,255 }, 120));
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawFrame(3);
	font(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	font(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}

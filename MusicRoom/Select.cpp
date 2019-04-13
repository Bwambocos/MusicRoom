// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"
#include "Select.h"

// アルバム選択画面

// 初期化
Select::Select(const InitData& init) : IScene(init)
{
	if (main) return;

	// メイン背景
	{
		main = Texture(U"data\\backgroundImage.png");
		fav = Texture(U"data\\Detail\\fav.png");
	}

	fav = Texture(U"data\\Select\\fav.png");
	no_img = Texture(U"data\\Select\\no_img.png");

	// album_list 読み込み
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
			while (text.readLine(temp_of_temp)) comment += temp_of_temp;
			Texture image(U"music\\" + temp + U"\\" + temp + U".png");
			if (!image) image = no_img;
			AlbumList.push_back({ name,temp,creator,comment,image });
		}
		z = Grid<double>(3, (AlbumList.size() + 1) / 3 + 3);
	}
	startTime = (int)Time::GetMillisec();
	comTime.resize(AlbumList.size() + 8);
	goUp = Triangle({ 384,75 }, { 414,85 }, { 354,85 });
	goDown = Triangle({ 354,560 }, { 414,560 }, { 384,570 });
}

// 更新
void Select::update()
{
	nowTime = (int)Time::GetMillisec();
	if (KeyF5.pressed())
	{
		const Rect temprect(0, BAR_HEIGHT, Window::Width(), Window::Height());
		const Font tempfont(32, Typeface::Bold);
		main.draw(0, BAR_HEIGHT);
		temprect.draw(Color(64, 64, 64, 100));
		tempfont(U"再読み込み中・・・").drawAt(Window::Width() / 2, Window::Height() / 2);
		System::Update();
		changeScene(U"Select");
	}
	// スクロール 更新
	{
		if (!scr_flag)
		{
			if (first_cou > 0)
			{
				if (goUp.leftClicked())
				{
					scr_flag = true;
					scr_upflag = true;
				}
				if (Mouse::Wheel() < 0)
				{
					scr_flag = true;
					scr_upflag = true;
				}
			}
			if (first_cou + 5 <= (signed)AlbumList.size())
			{
				if (goDown.leftClicked())
				{
					scr_flag = true;
					scr_upflag = false;
				}
				if (Mouse::Wheel() > 0)
				{
					scr_flag = true;
					scr_upflag = false;
				}
			}
			if (scr_flag) scrollNowTime = scrollStartTime = Time::GetMillisec();
		}
		else
		{
			if (scrollNowTime - scrollStartTime > SCROLL_MSEC)
			{
				first_cou += (!scr_upflag ? 3 : -3);
				scr_flag = false;
				scrollY = 0;
				first_cou = Max(first_cou, 0);
				first_cou = Min<int>(first_cou, (int)AlbumList.size() / 3 * 3);
			}
			else scrollY = (!scr_upflag ? -246 * ((scrollNowTime - scrollStartTime)) / SCROLL_MSEC : 246 * ((scrollNowTime - scrollStartTime)) / SCROLL_MSEC);
			scrollNowTime = Time::GetMillisec();
		}
	}

	// album_list 更新
	int cou = first_cou;
	for (int y = 0; y < (signed)z.height(); ++y)
	{
		for (int x = 0; x < (signed)z.width(); ++x)
		{
			const Rect rect = MakeRect(x, y);
			if (cou == (signed)AlbumList.size() + 2) break;
			if (MouseL.down() && rect.mouseOver())
			{
				if (cou == (signed)AlbumList.size()) changeScene(U"Fav");
				else
				{
					setAlbum = AlbumList[cou].name;
					setAlbumB = AlbumList[cou].dname;
					selectedAlbumNum = cou;
					changeScene(U"Album");
				}
			}
			++cou;
		}
		if (cou == (signed)AlbumList.size() + 1) break;
	}
	cou = first_cou - 3;
	for (int y = -1; y <= (signed)z.height(); ++y)
	{
		for (int x = 0; x < (signed)z.width(); ++x)
		{
			Rect rect = Select::MakeRect(x, y);
			if (scr_flag) rect.y += scrollY;
			if (cou == (signed)AlbumList.size() + 1) break;
			if (cou >= 0)
			{
				if (!rect.mouseOver())
				{
					z[y + 1][x + 1] = Max(z[y + 1][x + 1] - 0.02, 0.0);
				}
			}
			++cou;
		}
		if (cou == (signed)AlbumList.size() + 1) break;
	}
	cou = first_cou - 3;
	for (int y = -1; y <= (signed)z.height(); ++y)
	{
		for (int x = 0; x < (signed)z.width(); ++x)
		{
			Rect rect = MakeRect(x, y);
			rect.y += scrollY;
			if (rect.mouseOver() || z[y + 1][x + 1])
			{
				if (rect.mouseOver()) z[y + 1][x + 1] = Min(z[y + 1][x + 1] + 0.05, 0.5);
			}
			if (cou == (signed)AlbumList.size() + 1) break;
			if (cou >= 0)
			{
				if (rect.mouseOver())
				{
					comTime[cou].first = (comTime[cou].first == 0 ? (int)Time::GetMillisec() : comTime[cou].first);
					comTime[cou].second = (int)Time::GetMillisec();
				}
				else comTime[cou].first = comTime[cou].second = 0;
			}
			++cou;
		}
		if (cou == (signed)AlbumList.size() + 1) break;
	}
}

// 描画
void Select::draw() const
{
	// メイン背景
	{
		main.draw(0, BAR_HEIGHT);
		if (first_cou > 0)
		{
			goUp.draw((goUp.mouseOver() ? Palette::Orange : Palette::White));
			goUp.drawFrame(2, Palette::Black);
		}
		if (first_cou + 5 <= (signed)AlbumList.size())
		{
			goDown.draw((goDown.mouseOver() ? Palette::Orange : Palette::White));
			goDown.drawFrame(2, Palette::Black);
		}
	}

	// album_list 描画
	{
		int cou = first_cou - 3;
		cou = first_cou - 3;
		for (int y = -1; y <= (signed)z.height(); ++y)
		{
			for (int x = 0; x < (signed)z.width(); ++x)
			{
				Rect rect = Select::MakeRect(x, y);
				if (scr_flag) rect.y += scrollY;
				if (cou == (signed)AlbumList.size() + 1) break;
				if (cou >= 0)
				{
					if (!rect.mouseOver())
					{
						rect(Select::SelectImage(cou).resized(216, 216)).draw();
						rect.drawFrame(3, 0, Color(0, 0, 0));
					}
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 1) break;
		}
		cou = first_cou - 3;
		for (int y = -1; y <= (signed)z.height(); ++y)
		{
			for (int x = 0; x < (signed)z.width(); ++x)
			{
				Rect rect = MakeRect(x, y);
				rect.y += scrollY;
				if (cou == (signed)AlbumList.size() + 1) break;
				const double s = z[y + 1][x + 1];
				if (cou >= 0)
				{
					RectF(rect).stretched(s * 2).drawShadow({ 0,15 * s }, 32 * s, 10 * s);
					RectF(rect).stretched(s * 2)(Select::SelectImage(cou).resized(216, 216)).draw();
					RectF(rect).stretched(s * 2).drawFrame(3, 0, Color(0, 0, 0));
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 1) break;
		}
		cou = first_cou - 3;
		for (int y = -1; y <= (signed)z.height(); ++y)
		{
			for (int x = 0; x < (signed)z.width(); ++x)
			{
				Rect rect = MakeRect(x, y);
				rect.y += scrollY;
				if (cou == (signed)AlbumList.size() + 1) break;
				if (cou >= 0)
				{
					if (rect.mouseOver())
					{
						if (comTime[cou].second - comTime[cou].first >= COM_MESSAGE_MILLISEC) DrawDetails(cou);
					}
				}
				++cou;
			}
			if (cou == (signed)AlbumList.size() + 1) break;
		}
	}
}

// アルバム一覧 正方形区画を作成
Rect Select::MakeRect(int x, int y) const
{
	return { 30 + x * 246,BAR_HEIGHT + 30 + y * 246,216,216 };
}

// アルバム画像を返す
Texture Select::SelectImage(int cou) const
{
	Texture res;
	if (cou < 0) return res;
	if (cou < (signed)AlbumList.size()) res = AlbumList[cou].image;	// アルバム
	else if (cou == (signed)AlbumList.size()) res = fav;	// お気に入り
	return res;
}

// 選択されたアルバムの名前を返す
std::pair<String, String> Select::getSetAlbum()
{
	return std::make_pair(setAlbum, setAlbumB);
}

// 次のアルバムを返す
void Select::getNextAlbum()
{
	++selectedAlbumNum;
	setAlbum = AlbumList[selectedAlbumNum % AlbumList.size()].name;
	setAlbumB = AlbumList[selectedAlbumNum % AlbumList.size()].dname;
}

// アルバム詳細 描画
void Select::DrawDetails(int cou) const
{
	const Point pos = Cursor::Pos();
	static Font font(16);
	static String name, creator;
	if (cou == (signed)AlbumList.size())
	{
		name = U"お気に入り";
		creator = U"お気に入り登録した曲を表示します。";
	}
	else
	{
		name = AlbumList[cou].name;
		creator = AlbumList[cou].creator;
	}
	const auto name_width = font(name).region();
	const auto creator_width = font(creator).region();
	const auto width = Max(name_width.w, creator_width.w);
	static int x_addtion;
	if (cou % 3 == 0) x_addtion = 13;
	if (cou % 3 == 1) x_addtion = (-width / 2);
	if (cou % 3 == 2) x_addtion = -width;
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawShadow({ 0,15 }, 32, 10);
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).draw(Color({ 255,255,255 }, 120));
	RoundRect(pos.x + x_addtion, pos.y + 13, width + 27, 72, 27).drawFrame(3);
	font(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	font(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}

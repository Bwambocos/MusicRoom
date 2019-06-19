// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"
#include "Select.h"

// アルバム選択画面

// 初期化
Select::Select(const InitData& init) : IScene(init)
{
	// アルバム・曲選択 初期化
	if (getData().nowScene == U"Fav" && !getData().FavMusicList.empty() && getData().selectedFavMusicIndex != -1) getData().FavMusicList[getData().selectedFavMusicIndex].music.stop();
	if (getData().nowScene == U"Album" && getData().selectedMusicIndex != -1) getData().MusicList[getData().AlbumList[getData().selectedAlbumIndex].dir][getData().selectedMusicIndex].music.stop();
	getData().selectedAlbumIndex = -1;
	getData().selectedMusicIndex = -1;
	getData().selectedFavMusicIndex = -1;

	// シーン状況 更新
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Select";

	// 変数 初期化
	goUpButton = Triangle({ 384,75 }, { 414,85 }, { 354,85 });
	goDownButton = Triangle({ 354,560 }, { 414,560 }, { 384,570 });
	albumDetailFont = Font(18, U"data\\fontR.ttc");

	// データ 読み込み
	backgroundImage = Texture(U"data\\backgroundImage.png");
	favImage = Texture(U"data\\Select\\favImage.png");
	noimgImage = Texture(U"data\\Select\\noimgImage.png");
	albumDetailImage = Texture(U"data\\Select\\albumDetailImage.png");
	nowloadingImage = Texture(U"data\\Select\\nowloadingImage.png");
	TextReader reader = TextReader(U"music\\album_list.txt");
	getData().AlbumList.clear();
	String dir;
	while (reader.readLine(dir))
	{
		String name, creator, comment;
		TextReader text(U"music\\" + dir + U"\\" + dir + U".txt");
		text.readLine(name);
		text.readLine(creator);
		String temp; while (text.readLine(temp)) comment += temp + U"\n";
		Texture image(U"music\\" + dir + U"\\" + dir + U".png");
		if (!image) image = noimgImage;
		getData().AlbumList.push_back({ name,dir,creator,comment,image });
	}
	commentTime.resize(getData().AlbumList.size() + 8);
	albumGrid = Grid<double>(3, (getData().AlbumList.size() + 1) / 3 + 3);
}

// 更新
void Select::update()
{
	// スクロール 更新
	if (scrollFlag == 0)
	{
		if (AlbumListFirstIndex > 0 && (goUpButton.leftClicked() || Mouse::Wheel() < 0))
		{
			scrollFlag = 1;
			scrollTime.restart();
		}
		if (AlbumListFirstIndex + 5 <= (signed)getData().AlbumList.size() && (goDownButton.leftClicked() || Mouse::Wheel() > 0))
		{
			scrollFlag = 2;
			scrollTime.restart();
		}
	}
	else
	{
		if (scrollTime.ms() >= scrollMillisec)
		{
			AlbumListFirstIndex += (scrollFlag == 1 ? -3 : 3);
			scrollAddY = scrollFlag = 0;
			AlbumListFirstIndex = Max(AlbumListFirstIndex, 0);
			AlbumListFirstIndex = Min<int>(AlbumListFirstIndex, (int)getData().AlbumList.size() / 3 * 3);
		}
		else scrollAddY = (double)(scrollFlag == 1 ? 246 : -246) * scrollTime.ms() / scrollMillisec;
	}

	// album_list 更新
	int cnt = AlbumListFirstIndex - 3;
	for (int y = -1; y <= (signed)albumGrid.height(); ++y)
	{
		for (int x = 0; x < (signed)albumGrid.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += scrollAddY;
			if (cnt >= 0)
			{
				if (rect.mouseOver())
				{
					commentTime[cnt].first = (commentTime[cnt].first == 0 ? (int)Time::GetMillisec() : commentTime[cnt].first);
					commentTime[cnt].second = (int)Time::GetMillisec();
					if (MouseL.down())
					{
						if (cnt == (signed)getData().AlbumList.size()) changeScene(U"Fav", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
						else
						{
							getData().selectedAlbumIndex = cnt;
							backgroundImage.draw();
							nowloadingImage.draw();
							System::Update();
							changeScene(U"Album", GameInfo::FadeInTime, GameInfo::FadeCrossFlag);
						}
					}
				}
				else
				{
					albumGrid[y + 1][x + 1] = Max(albumGrid[y + 1][x + 1] - 0.02, 0.0);
					commentTime[cnt].first = commentTime[cnt].second = 0;
				}
			}
			if (rect.mouseOver()) albumGrid[y + 1][x + 1] = Min(albumGrid[y + 1][x + 1] + 0.05, 0.5);
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
	backgroundImage.draw();
	if (AlbumListFirstIndex > 0)
	{
		goUpButton.draw((goUpButton.mouseOver() ? Palette::Orange : Palette::White));
		goUpButton.drawFrame(2, Palette::Black);
	}
	if (AlbumListFirstIndex + 5 <= (signed)getData().AlbumList.size())
	{
		goDownButton.draw((goDownButton.mouseOver() ? Palette::Orange : Palette::White));
		goDownButton.drawFrame(2, Palette::Black);
	}

	// album_list 描画
	int cnt = AlbumListFirstIndex - 3;
	for (int y = -1; y <= (signed)albumGrid.height(); ++y)
	{
		for (int x = 0; x < (signed)albumGrid.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += scrollAddY;
			const double s = albumGrid[y + 1][x + 1];
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
	cnt = AlbumListFirstIndex;
	for (int y = 0; y <= (signed)albumGrid.height(); ++y)
	{
		for (int x = 0; x < (signed)albumGrid.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += scrollAddY;
			if (rect.mouseOver() && commentTime[cnt].second - commentTime[cnt].first >= commentMillisec) drawDetails(cnt);
			++cnt;
			if (cnt == (signed)getData().AlbumList.size() + 1) break;
		}
		if (cnt == (signed)getData().AlbumList.size() + 1) break;
	}
}

// アルバム一覧 正方形区画を作成
Rect Select::makeRect(int x, int y) const
{
	return { 30 + x * 246, barHeight + 30 + y * 246, 216, 216 };
}

// アルバム画像を返す
Texture Select::getSelectedImage(int cnt) const
{
	if (cnt < 0) return Texture();
	return (cnt < (signed)getData().AlbumList.size() ? getData().AlbumList[cnt].image : favImage);
}

// アルバム詳細 描画
void Select::drawDetails(int cnt) const
{
	const Point pos = Cursor::Pos();
	String name, creator;
	if (cnt == (signed)getData().AlbumList.size())
	{
		name = U"お気に入り";
		creator = U"お気に入り登録した曲を表示します。";
	}
	else
	{
		name = U"アルバム名：" + getData().AlbumList[cnt].name;
		creator = U"作者：" + getData().AlbumList[cnt].creator;
	}
	const auto width = Max(albumDetailFont(name).region().w, albumDetailFont(creator).region().w);
	static int x_addtion;
	if (cnt % 3 == 0) x_addtion = 13;
	if (cnt % 3 == 1) x_addtion = (-width / 2);
	if (cnt % 3 == 2) x_addtion = -width;
	Rect(pos.x + x_addtion, pos.y + 13, width + 27, 72).drawShadow({ 5,5 }, 5);
	Rect(pos.x + x_addtion, pos.y + 13, width + 27, 72)(albumDetailImage).draw();
	Rect(pos.x + x_addtion, pos.y + 13, width + 27, 72).drawFrame(1, Palette::Black);
	albumDetailFont(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	albumDetailFont(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}

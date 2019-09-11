// include
#include <Siv3D.hpp>
#include "Bar.h"
#include "Select.h"

// �A���o���I�����

// ������
Select::Select(const InitData& init) : IScene(init)
{
	// �A�Z�b�g�Ǘ��o�^
	FontAsset::Register(U"Select.albumDetail", 18, U"data\\fontR.ttc");
	TextureAsset::Register(U"Select.fav", Icon(0xf005, 216));
	TextureAsset::Register(U"Select.goUp", Icon(0xf139, 42));
	TextureAsset::Register(U"Select.goDown", Icon(0xf13a, 42));

	// �A���o���E�ȑI�� ������
	getData().selectedAlbumIndex = -1;
	getData().selectedMusicIndex = -1;
	getData().selectedFavMusicIndex = -1;

	// �V�[���� �X�V
	getData().prevScene = getData().nowScene;
	getData().nowScene = U"Select";

	// �A���o���ǂݍ���
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
		TextureAsset::Register(U"album-" + dir + U".image", U"music\\" + dir + U"\\" + dir + U".png", TextureDesc::Mipped);
		getData().AlbumList.push_back({ name,dir,creator,comment });
	}

	albumGrid = Grid<double>(albumGridWidth, (getData().AlbumList.size() + albumGridWidth) / albumGridWidth + 2);
	commentTime.resize(getData().AlbumList.size() + 1);
	goUpPos = Vec2(Scene::Width() / 2, barHeight + 5 + TextureAsset(U"Select.goUp").height() / 2);
	goDownPos = Vec2(Scene::Width() / 2,Scene::Height() - 5 - TextureAsset(U"Select.goDown").height() / 2);
}

// �X�V
void Select::update()
{
	// �X�N���[�� �X�V
	if (scrollFlag == 0)
	{
		if (getData().AlbumListFirstIndex > 0 && (TextureAsset(U"Select.goUp").regionAt(goUpPos).leftClicked() || Mouse::Wheel() < 0))
		{
			scrollFlag = 1;
			scrollTime.restart();
		}
		if (getData().AlbumListFirstIndex + albumGridWidth < (signed)getData().AlbumList.size() + 1 && (TextureAsset(U"Select.goDown").regionAt(goDownPos).leftClicked() || Mouse::Wheel() > 0))
		{
			scrollFlag = 2;
			scrollTime.restart();
		}
	}
	else
	{
		if (scrollTime.ms() >= scrollMillisec)
		{
			getData().AlbumListFirstIndex += (scrollFlag == 1 ? -(int)albumGridWidth : (int)albumGridWidth);
			scrollAddY = scrollFlag = 0;
			getData().AlbumListFirstIndex = Max(getData().AlbumListFirstIndex, 0);
			getData().AlbumListFirstIndex = Min(getData().AlbumListFirstIndex, (int)getData().AlbumList.size() / (int)albumGridWidth * (int)albumGridWidth);
		}
		else scrollAddY = (double)(scrollFlag == 1 ? (tileSize+25) : -(tileSize+25)) * scrollTime.ms() / scrollMillisec;
	}

	// album_list �X�V
	int cnt = getData().AlbumListFirstIndex - (int)albumGridWidth;
	for (int y = -1; y <= (signed)albumGrid.height(); ++y)
	{
		for (int x = 0; x < (signed)albumGrid.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += (int)scrollAddY;
			if (cnt >= 0)
			{
				if (rect.mouseOver())
				{
					commentTime[cnt].first = (commentTime[cnt].first == 0 ? (int)Time::GetMillisec() : commentTime[cnt].first);
					commentTime[cnt].second = (int)Time::GetMillisec();
					if (scrollFlag == 0 && MouseL.down())
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

// �`��
void Select::draw() const
{
	// album_list �`��
	int cnt = getData().AlbumListFirstIndex - (int)albumGridWidth;
	for (int y = -1; y <= (signed)albumGrid.height(); ++y)
	{
		for (int x = 0; x < (signed)albumGrid.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += (int)scrollAddY;
			const double s = albumGrid[y + 1][x + 1];
			if (cnt >= 0)
			{
				rect
					.stretched((int)(s * 2))
					.drawShadow(Vec2(0, 15 * s), 32 * s, 10 * s)
					(getSelectedImage(cnt).resized(tileSize, tileSize)).draw()
					.drawFrame(3, 0, (rect.mouseOver() ? Palette::Red : Palette::Black));
			}
			++cnt;
			if (cnt == (signed)getData().AlbumList.size() + 1) break;
		}
		if (cnt == (signed)getData().AlbumList.size() + 1) break;
	}
	cnt = getData().AlbumListFirstIndex;
	for (int y = 0; y <= (signed)albumGrid.height(); ++y)
	{
		for (int x = 0; x < (signed)albumGrid.width(); ++x)
		{
			Rect rect = Select::makeRect(x, y);
			rect.y += (int)scrollAddY;
			if (rect.mouseOver() && commentTime[cnt].second - commentTime[cnt].first >= commentMillisec) drawDetails(cnt);
			++cnt;
			if (cnt == (signed)getData().AlbumList.size() + 1) break;
		}
		if (cnt == (signed)getData().AlbumList.size() + 1) break;
	}

	if (getData().AlbumListFirstIndex > 0) TextureAsset(U"Select.goUp").drawAt(goUpPos, (TextureAsset(U"Select.goUp").regionAt(goUpPos).mouseOver() ? Palette::Orange : Palette::White));
	if (getData().AlbumListFirstIndex + albumGridWidth < (signed)getData().AlbumList.size() + 1) TextureAsset(U"Select.goDown").drawAt(goDownPos, (TextureAsset(U"Select.goDown").regionAt(goDownPos).mouseOver() ? Palette::Orange : Palette::White));
}

// �A���o���ꗗ �����`�����쐬
Rect Select::makeRect(int x, int y) const
{
	return { 25 + x * (tileSize+25), barHeight + 25 + y * (tileSize+25), tileSize, tileSize };
}

// �A���o���摜��Ԃ�
Texture Select::getSelectedImage(int cnt) const
{
	if (cnt < 0) return Texture();
	return (cnt < (signed)getData().AlbumList.size() ? TextureAsset(U"album-" + getData().AlbumList[cnt].dir + U".image") : TextureAsset(U"Select.fav"));
}

// �A���o���ڍ� �`��
void Select::drawDetails(int cnt) const
{
	const Point pos = Cursor::Pos();
	String name, creator;
	if (cnt == (signed)getData().AlbumList.size())
	{
		name = U"���C�ɓ���";
		creator = U"���C�ɓ���o�^�����Ȃ�\�����܂��B";
	}
	else
	{
		name = U"�A���o�����F" + getData().AlbumList[cnt].name;
		creator = U"��ҁF" + getData().AlbumList[cnt].creator;
	}
	const auto width = Max(FontAsset(U"Select.albumDetail")(name).region().w, FontAsset(U"Select.albumDetail")(creator).region().w);
	int x_addtion = 0;
	if (cnt % 3 == 0) x_addtion = 13;
	if (cnt % 3 == 1) x_addtion = (-width / 2);
	if (cnt % 3 == 2) x_addtion = -width;
	Rect(pos.x + x_addtion, pos.y + 13, width + 27, 72)
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(Palette::White)
		.drawFrame(3, 0, Palette::Black);
	FontAsset(U"Select.albumDetail")(name).draw(pos.x + x_addtion + 14, pos.y + 20, Color(16, 16, 16));
	FontAsset(U"Select.albumDetail")(creator).draw(pos.x + x_addtion + 14, pos.y + 50, Color(16, 16, 16));
}

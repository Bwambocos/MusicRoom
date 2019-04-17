// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Bar.h"
#include "Music.h"

// �ȏڍ׉��

// ������
Music::Music(const InitData& init) : IScene(init)
{
	// �w�i ������
	if (!music_Main)
	{
		music_Main = Texture(U"data\\Music\\main.png");
		originPlay[0] = Texture(U"data\\Music\\play\\normal.png");
		originPlay[1] = Texture(U"data\\Music\\play\\active.png");
		originBrief[0] = Texture(U"data\\Music\\brief\\normal.png");
		originBrief[1] = Texture(U"data\\Music\\brief\\active.png");
		originStop[0] = Texture(U"data\\Music\\stop\\normal.png");
		originStop[1] = Texture(U"data\\Music\\stop\\active.png");
		originSeek[0] = Texture(U"data\\Music\\seek\\normal.png");
		originSeek[1] = Texture(U"data\\Music\\seek\\active.png");
		originRep[0] = Texture(U"data\\Music\\rep\\normal.png");
		originRep[1] = Texture(U"data\\Music\\rep\\active.png");
		faved = Texture(U"data\\Music\\fav.png");
		not_faved = Texture(U"data\\Music\\not_fav.png");
		music_NameTime = Font(18);
		music_Exp = Font(16);
		rect_musicName = RoundRect(25, 25 + BAR_HEIGHT, 468, 48, 10);
		rect_musicTime = RoundRect(496, 25 + BAR_HEIGHT, 199, 48, 10);
		rect_music_isFav = RoundRect(698, 25 + BAR_HEIGHT, 48, 48, 10);
		rect_musicBar = RoundRect(127, 91 + BAR_HEIGHT, 565, 21, 5);
		rect_musicExp = RoundRect(25, 130 + BAR_HEIGHT, 718, 357, 10);
		displayPlay = originPlay[0];
		displayBrief = originBrief[0];
		displayStop = originStop[0];
		displaySeek = originSeek[0];
		displayRep = originRep[0];
	}

	// �ȏ�� ������
	switch (get_prevScene())
	{
	case Scene_Detail:
		setAlbumMusicName(music_albumName, music_albumBName, music_musicFileName, music_Music);
		favLoop_flag = false;
		break;

	case Scene_Fav:
		if (!favLoop_flag) setFavMusicName(music_albumName, music_albumBName, music_musicFileName, music_Music);
		else setFavMusicName(prev_or_next, music_albumName, music_albumBName, music_musicFileName, music_Music);
		break;

	case Scene_Music:
		if (!favLoop_flag) setAlbumMusicName(prev_or_next, music_albumName, music_albumBName, music_musicFileName, music_Music);
		else setFavMusicName(prev_or_next, music_albumName, music_albumBName, music_musicFileName, music_Music);
		break;
	}
	TextReader music_reader(U"music\\" + music_albumBName + U"\\" + music_musicFileName + U"\\" + music_musicFileName + U".txt");
	music_reader.readLine(music_musicName);
	music_musicExp.clear();
	String temp;
	while (music_reader.readLine(temp)) music_musicExp += temp + U"\n";
	music_musicTime = (int)music_Music.lengthSec();

	// �`��ʒu ������
	draw_musicName_startMSec = (int)Time::GetMillisec();
	draw_musicName_stayFlag = true;
	draw_musicName_x = DEFAULT_musicName_X;

	giveMusicData(music_albumName, music_musicName, music_Music);
	music_Music.play();
	if (!still_flag) changeScene(get_prevScene());
}

// �X�V
void Music::update()
{
	if (KeyB.pressed())
	{
		changeScene((favLoop_flag ? U"Fav" : U"Album"));
		still_flag = false;
	}
	if (KeyF5.pressed()) changeScene(U"Music");
	if (!music_Music.isPlaying() && !stop_flag && music_Music.samplesPlayed() % music_Music.samples() == 0)
	{
		if (get_prevScene() == Scene_Fav)
		{
			favLoop_flag = true;
		}
		else
		{
			favLoop_flag = false;
		}
		prev_or_next = 1;
		changeScene(U"Music");
	}

	// �Đ��o�[ �X�V
	// �o�[
	if (rect_musicBar.leftPressed())
	{
		button_flag = true;
		music_Music.pause();
		const Point tmpPoint = Cursor::Pos();
		music_Music.setPosSample(music_Music.samples() * (tmpPoint.x - (int)rect_musicBar.x) / (int)rect_musicBar.w);
	}
	else if (button_flag)
	{
		music_Music.play();
		button_flag = false;
	}

	// �{�^��
	Circle tmpCircle(45, rect_musicBar.y + rect_musicBar.h / 2, 15);
	displayPlay = originPlay[(tmpCircle.mouseOver ? 1 : 0)];
	displayBrief = originBrief[(tmpCircle.mouseOver ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeyEnter.pressed())
	{
		(music_Music.isPlaying() ? music_Music.pause() : music_Music.play());
		stop_flag = false;
		set_stopFlag(stop_flag);
	}
	tmpCircle = Circle(90, rect_musicBar.y + rect_musicBar.h / 2, 15);
	displayRep = originRep[((tmpCircle.mouseOver || musicLoopFlag) ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeyShift.pressed())
	{
		const int tmpTime = (int)music_Music.streamPosSample();
		music_Music.pause();
		music_Music.setLoop(!musicLoopFlag);
		music_Music.play();
		music_Music.setPosSample(tmpTime);
		stop_flag = false;
		set_stopFlag(stop_flag);
	}
	tmpCircle = Circle(723, rect_musicBar.y + rect_musicBar.h / 2, 15);
	displayStop = originStop[(tmpCircle.mouseOver ? 1 : 0)];
	if (tmpCircle.leftClicked() || KeySpace.pressed())
	{
		music_Music.stop();
		stop_flag = true;
		set_stopFlag(stop_flag);
	}

	// �ȏ�� �X�V
	Update_drawMusicDetailStrings();
	music_musicLength = Format(Pad(music_musicTime / 60, { 2,U'0' }), U":", Pad(music_musicTime % 60, { 2,U'0' }));
	if (rect_music_isFav.leftClicked)
	{
		(isFav(music_albumName, music_musicName) ? removeFav(music_albumName, music_musicName) : addFav(music_albumName, music_albumBName, music_musicName, music_musicFileName, music_Music));
	}
	FFT::Analyze(fft, music_Music);
}

// �`��
void Music::draw() const
{
	// �w�i �`��
	music_Main.draw(0, BAR_HEIGHT);
	for (auto i : step(51)) { RectF(1 + i * 15, Window::Height(), 15, -Pow(fft.buffer[i], 0.8) * 750).draw(Color(200, 200, 200, 200)); }
	rect_musicName.drawShadow({ 0,15 }, 32, 10);
	rect_musicName.draw(Color(32, 32, 32, 120));
	rect_musicTime.drawShadow({ 0,15 }, 32, 10);
	rect_musicTime.draw(Color(32, 32, 32, 120));
	rect_music_isFav.drawShadow({ 0,15 }, 32, 10);
	rect_music_isFav.draw(Color(32, 32, 32, 120));

	// �Đ��o�[
	// �o�[
	rect_musicBar.drawShadow({ 0,15 }, 32, 10);
	rect_musicBar.drawFrame(3);
	rect_musicBar.draw(Color(32, 32, 32, 120));
	const RoundRect tmpRect(rect_musicBar.x, rect_musicBar.y, rect_musicBar.w * music_Music.streamPosSample() / music_Music.samples(), rect_musicBar.h, rect_musicBar.r);
	tmpRect.draw(Color(0, 200, 0, 120));

	// Seek
	auto x = rect_musicBar.x + rect_musicBar.w * music_Music.streamPosSample() / music_Music.samples();
	(music_Music.isPlaying() ? originSeek[1] : originSeek[0]).drawAt(x, rect_musicBar.y + rect_musicBar.h / 2);

	// �{�^��
	(music_Music.isPlaying() ? displayBrief : displayPlay).drawAt(45, rect_musicBar.y + rect_musicBar.h / 2);
	displayRep.drawAt(90, rect_musicBar.y + rect_musicBar.h / 2);
	displayStop.drawAt(723, rect_musicBar.y + rect_musicBar.h / 2);
	rect_musicExp.drawShadow({ 0,15 }, 32, 10);
	rect_musicExp.drawFrame(3);
	rect_musicExp.draw(Color(32, 32, 32, 120));

	// �ȏ�� �`��
	RasterizerState rasterizer = RasterizerState::Default2D;
	rasterizer.scissorEnable = true;
	Graphics2D::SetRasterizerState(rasterizer);
	Graphics2D::SetScissorRect(Rect((int)rect_musicName.x, (int)rect_musicName.y, (int)rect_musicName.w, (int)rect_musicName.h));
	music_NameTime(music_musicName).draw(draw_musicName_x, 31 + BAR_HEIGHT);
	Graphics2D::SetScissorRect(Rect(0, 0, Window::Width(), Window::Height()));
	music_NameTime(music_musicLength).draw(504, 31 + BAR_HEIGHT);
	((isFav(music_albumName, music_musicName) || rect_music_isFav.mouseOver) ? faved : not_faved).drawAt(722, 49 + BAR_HEIGHT);
	musicExpl_Draw();
	rect_musicName.drawFrame(0, 2, Palette::Gray);
	rect_musicTime.drawFrame(0, 2, Palette::Gray);
	rect_music_isFav.drawFrame(0, 2, Palette::Gray);
}

// �Ȑ��� �`��
void Music::musicExpl_Draw() const
{
	Array<String> texts;
	const int32 w = (int32)rect_musicExp.w - 10;
	const int32 h = (int32)rect_musicExp.h;
	size_t pos = 0;

	while (pos < music_musicExp.length)
	{
		for (int i = 0; i + pos < music_musicExp.length; ++i)
		{
			if (music_Exp(music_musicExp.substr(pos, i)).region().w >= w)
			{
				texts.push_back(music_musicExp.substr(pos, i - 1));
				pos += (i - 1);
				break;
			}
			if (music_musicExp[pos + i] == U'\n')
			{
				texts.push_back(music_musicExp.substr(pos, i + 1));
				pos += (i + 1);
				break;
			}
		}
	}

	for (size_t i = 0; i < texts.size(); ++i)
	{
		const int32 y = static_cast<int32>(rect_musicExp.y + 10 + i * music_Exp.height);
		const bool overflow = (i + 1 < texts.size()) && (y + music_Exp.height * 2 + 10 > rect_musicExp.y + rect_musicExp.h);
		if (overflow)
		{
			if (texts[i].length > 2) texts[i].resize(texts[i].length - (texts[i].length > 2 ? 2 : 1));
			if (texts[i][texts[i].length - 1] == U'\n') texts[i][texts[i].length - 1] = U'\0';
			texts[i].append(U"...");
		}
		music_Exp(texts[i]).draw(rect_musicExp.x + 10, y);
		if (overflow) break;
	}
}

// �Ȗ��`��ʒu �X�V
void Music::Update_drawMusicDetailStrings()
{
	auto rect = rect_musicName;
	auto width = music_NameTime(music_musicName).region().w + rect.r;
	if (width > rect_musicName.w)
	{
		if (!draw_musicName_stayFlag)
		{
			if (draw_musicName_x + width > rect.x + rect.w) draw_musicName_x -= (double)DRAW_MUSICNAME_MOVE_X * (Time::GetMillisec() - draw_musicName_stayMSec) / 1000;
			else
			{
				draw_musicName_startMSec = draw_musicName_stayMSec = (int)Time::GetMillisec();
				draw_musicName_stayFlag = true;
			}
		}
		if (draw_musicName_stayFlag)
		{
			if (draw_musicName_stayMSec - draw_musicName_startMSec >= DRAW_STAYMSEC)
			{
				draw_musicName_startMSec = draw_musicName_stayMSec;
				if (draw_musicName_x == DEFAULT_musicName_X) draw_musicName_stayFlag = false;
				else draw_musicName_x = DEFAULT_musicName_X;
			}
		}
		draw_musicName_stayMSec = (int)Time::GetMillisec();
	}
}

// �Ȏ蓮�J��
void Music::changeMusic(int flag)
{
	favLoop_flag = (get_prevScene() == Scene_Fav || favLoop_flag);
	prev_or_next = flag;
	music_Music.stop();
	if (!favLoop_flag) setAlbumMusicName(prev_or_next, music_albumName, music_albumBName, music_musicFileName, music_Music);
	else setFavMusicName(prev_or_next, music_albumName, music_albumBName, music_musicFileName, music_Music);
	TextReader music_reader(U"music\\" + music_albumBName + U"\\" + music_musicFileName + U"\\" + music_musicFileName + U".txt");
	music_reader.readLine(music_musicName);
	String temp;
	music_musicExp.clear();
	while (music_reader.readLine(temp)) music_musicExp += temp + U"\n";
	music_musicTime = (int)music_Music.lengthSec();
	draw_musicName_startMSec = (int)Time::GetMillisec();
	draw_musicName_stayFlag = true;
	draw_musicName_x = DEFAULT_musicName_X;
	giveMusicData(music_albumName, music_musicName, music_Music);
	music_Music.play();
}

// �ȑ���
// kind: 0->�ꎞ��~, 1->�Đ�, 2->��~, 3->�J��Ԃ��؂�ւ�
void Music::changeMusicStats(int kind)
{
	switch (kind)
	{
	case 0:
		music_Music.pause();
		stop_flag = false;
		break;
	case 1:
		music_Music.play();
		stop_flag = false;
		break;
	case 2:
		music_Music.stop();
		stop_flag = true;
		break;
	case 3:
		const int tmpTime = (int)music_Music.streamPosSample();
		music_Music.pause();
		music_Music.setLoop(!musicLoopFlag);
		music_Music.play();
		music_Music.setPosSample(tmpTime);
		stop_flag = false;
		break;
	}
}

// ����ʒ��t���O�Z�b�g
void Music::set_stillFlag(bool flag)
{
	still_flag = flag;
}

// ���C�ɓ��胋�[�v�����ۂ�
bool Music::isFavLooping()
{
	return favLoop_flag;
}

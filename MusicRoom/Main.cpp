// include
#include <Siv3D.hpp>
#include "Main.h"
#include "Bar.h"
#include "Select.h"
#include "Album.h"
#include "Music.h"
#include "Fav.h"

void Main()
{
	Window::Resize(GameInfo::Width, GameInfo::Height + barHeight);
	Window::SetTitle(GameInfo::Title);

	MyApp manager;
	Scene::SetBackground((*(manager.get())).schemeColor1);
	manager.setFadeColor((*(manager.get())).schemeColor1);
	manager.add<Select>(U"Select");
	manager.add<Album>(U"Album");
	manager.add<Music>(U"Music");
	manager.add<Fav>(U"Fav");

	Bar musicBar;
	loadFavList(*(manager.get()));
	while (System::Update())
	{
		manager.updateScene();
		manager.drawScene();
		musicBar.update(*(manager.get()), manager);
		musicBar.draw(*(manager.get()));
	}
	saveFavList(*(manager.get()));
}

// お気に入りリスト 読込
void loadFavList(GameData& getData)
{
	CSVData csv(U"data\\Fav\\FavMusicList.csv");
	for (int i = 0; i < (signed)csv.rows(); ++i)
	{
		auto albumDir = csv.get<String>(i, 0);
		auto musicDir = csv.get<String>(i, 1);
		String albumName, albumCreator, albumComment, musicName, musicComment;
		TextReader albumTextReader(U"music\\" + albumDir + U"\\" + albumDir + U".txt");
		TextReader musicTextReader(U"music\\" + albumDir + U"\\" + musicDir + U"\\" + musicDir + U".txt");
		albumTextReader.readLine(albumName);
		albumTextReader.readLine(albumCreator);
		String temp; while (albumTextReader.readLine(temp)) albumComment += temp + U"\n";
		musicTextReader.readLine(musicName);
		while (musicTextReader.readLine(temp)) musicComment += temp + U"\n";
		AudioAsset::Register(U"album-" + albumDir + U".music-" + musicDir, U"music\\" + albumDir + U"\\" + musicDir + U"\\" + musicDir + U".mp3");
		getData.addFav(albumName, albumDir, albumCreator, albumComment, musicName, musicDir, musicComment, Audio());
	}
}

// お気に入りリスト 保存
void saveFavList(GameData& getData)
{
	CSVData csv(U"data\\Fav\\FavMusicList.csv");
	csv.clear();
	for (auto i : getData.FavMusicList) csv.writeRow(i.album_dir, i.music_dir);
	csv.save(U"data\\Fav\\FavMusicList.csv");
}

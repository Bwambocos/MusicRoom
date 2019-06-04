// include
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Main.h"
#include "Bar.h"
#include "Select.h"
#include "Album.h"
#include "Music.h"
#include "Fav.h"

void Main()
{
	Window::Resize(GameInfo::Width, GameInfo::Height + BAR_HEIGHT);
	Window::SetTitle(GameInfo::Title);

	MyApp manager;
	manager.setFadeColor(GameInfo::FadeInColor);
	manager.add<Select>(U"Select");
	manager.add<Album>(U"Album");
	manager.add<Music>(U"Music");
	manager.add<Fav>(U"Fav");

	Bar musicBar;
	while (System::Update())
	{
		manager.updateScene();
		manager.drawScene();
		musicBar.update(*(manager.get()), manager);
		musicBar.draw(*(manager.get()));
	}
}

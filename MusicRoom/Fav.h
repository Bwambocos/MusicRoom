// include
#pragma once
#include "Main.h"

// お気に入り画面
class Fav : public MyApp::Scene
{
private:

public:
	Fav(const InitData& init);
	void update() override;
	void draw() const override;
};

// include
#pragma once
#include "Main.h"

// ‚¨‹C‚É“ü‚è‰æ–Ê
class Fav : public MyApp::Scene
{
private:

public:
	Fav(const InitData& init);
	void update() override;
	void draw() const override;
};

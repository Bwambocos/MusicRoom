// include
#pragma once
#include "Main.h"

// ƒAƒ‹ƒoƒ€Ú×‰æ–Ê
class Album : public MyApp::Scene
{
private:

public:
	Album(const InitData& init);
	void update() override;
	void draw() const override;
};

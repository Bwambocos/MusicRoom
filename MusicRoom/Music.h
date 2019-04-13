// include
#pragma once
#include "Main.h"

// ‹ÈÚ×‰æ–Ê
class Music : public MyApp::Scene
{
private:

public:
	Music(const InitData& init);
	void update() override;
	void draw() const override;
};

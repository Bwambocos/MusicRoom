// include
#pragma once
#include "Main.h"

// アルバム詳細画面
class Album : public MyApp::Scene
{
private:

public:
	Album(const InitData& init);
	void update() override;
	void draw() const override;
};

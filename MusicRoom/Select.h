// include
#pragma once
#include "Main.h"

// アルバム選択画面
class Select : public MyApp::Scene
{
private:

public:
	Select(const InitData& init);
	void update() override;
	void draw() const override;
};

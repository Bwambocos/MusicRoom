// include
#pragma once
#include "Main.h"

// �A���o���ڍ׉��
class Album : public MyApp::Scene
{
private:

public:
	Album(const InitData& init);
	void update() override;
	void draw() const override;
};

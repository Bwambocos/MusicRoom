// include
#pragma once
#include "Main.h"

// �ȏڍ׉��
class Music : public MyApp::Scene
{
private:

public:
	Music(const InitData& init);
	void update() override;
	void draw() const override;
};

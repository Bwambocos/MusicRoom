// include
#pragma once
#include "Main.h"

// ���C�ɓ�����
class Fav : public MyApp::Scene
{
private:

public:
	Fav(const InitData& init);
	void update() override;
	void draw() const override;
};

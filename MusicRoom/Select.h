// include
#pragma once
#include "Main.h"

// �A���o���I�����
class Select : public MyApp::Scene
{
private:

public:
	Select(const InitData& init);
	void update() override;
	void draw() const override;
};

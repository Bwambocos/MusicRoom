// include
#pragma once
#include "Main.h"

// �X�e�[�^�X�o�[
class Bar : public MyApp::Scene
{
private:

public:
	Bar(const InitData& init);
	void update() override;
	void draw() const override;
};

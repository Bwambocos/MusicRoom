// include
#pragma once
#include "Main.h"

// const
const int BAR_HEIGHT = 64;

// �X�e�[�^�X�o�[
class Bar : public MyApp::Scene
{
private:

public:
	Bar(const InitData& init);
	void update() override;
	void draw() const override;
};

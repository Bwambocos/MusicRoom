// include
#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>

// 全てのシーンからアクセスできるデータ
struct GameData
{

};

using MyApp = SceneManager<String, GameData>;

// ゲームの基本情報
namespace GameInfo
{
	const int32 Width = 768;
	const int32 Height = 512;
	const String Title = U"MusicRoom v3.0";
	const ColorF BackgroundColor = ColorF(0.4, 0.7, 0.5);
	const ColorF FadeInColor = ColorF(1.0, 1.0, 1.0);
}

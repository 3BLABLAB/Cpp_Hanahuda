# include "Result.h"

// コンストラクタの実装
// ClassName::FunctionName という書き方になります
Result::Result(const InitData& init)
	: IScene{ init }
	, font{ FontMethod::MSDF, 48 }
{
	Print << U"Result::Result()";
	Scene::SetBackground(ColorF{ 0.4, 0.4, 0.2 });

}

// デストラクタの実装
Result::~Result()
{
	Print << U"Result::~Result()";
}

// 更新関数の実装
void Result::update()
{
	if (SimpleGUI::Button(U"End", Vec2{ 350, 100 }, unspecified, true))
	{
		System::Exit();
	}
}

// 描画関数の実装
void Result::draw() const
{
	// ▼▼▼ 共有データから読み取る ▼▼▼
	const auto& aRolls = getData().ARolls;
	const auto& bRolls = getData().BRolls;

	//Aのスコアを表示
	font(U"A:").draw(60, 200, 100);
	for (int i = 0; i < aRolls.size(); ++i)
	{
		if (aRolls[i].isEmpty())continue;
		font(U"{}"_fmt(aRolls[i])).draw(40, Vec2{ 100, 170 + i * 50 });
	}

	//Bのスコアを表示
	font(U"B:").draw(60, 600, 100);
	for (int i = 0; i < bRolls.size(); ++i)
	{
		if (bRolls[i].isEmpty())continue;
		font(U"{}"_fmt(bRolls[i])).draw(40, Vec2{ 700, 170 + i * 50 });
	}
	
}

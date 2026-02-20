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

	int Ascore = PointCheck(aRolls);
	int Bscore = PointCheck(bRolls);
	if (Ascore > Bscore)font(U"あなたの勝ちです！").draw(60, 200, 50);
	else if (Ascore < Bscore)font(U"あなたの負けです！").draw(60, 200, 50);
	else font(U"引き分けです！").draw(60, 200, 50);
	//Aのスコアを表示
	font(U"あなた:{}"_fmt(Ascore)).draw(60, 100, 100);
	for (int i = 0; i < aRolls.size(); ++i)
	{
		if (aRolls[i].isEmpty())continue;
		font(U"{}"_fmt(aRolls[i])).draw(30, Vec2{ 100, 150 + i * 50 });
	}

	//Bのスコアを表示
	font(U"CPU:{}"_fmt(Bscore)).draw(60, 550, 100);
	for (int i = 0; i < bRolls.size(); ++i)
	{
		if (bRolls[i].isEmpty())continue;
		font(U"{}"_fmt(bRolls[i])).draw(30, Vec2{ 550, 150 + i * 50 });
	}
	
}

int Result::PointCheck(const std::vector<String>& Rolls) const{
	int sum = 0;
	for (const String& roll : Rolls) {
		if (roll.isEmpty()) continue;

		// --- 固定役の判定 ---
		if (roll == U"Goko") sum += 10;
		else if (roll == U"Shiko") sum += 8;
		else if (roll == U"AmeShiko") sum += 7;
		else if (roll == U"Sanko") sum += 5;

		if (roll == U"HanamiDeIppai") sum += 5;
		if (roll == U"TsukimiDeIppai") sum += 5;
		if (roll == U"Inoshikacho") sum += 5;
		if (roll == U"Akatan") sum += 5;
		if (roll == U"Aotan") sum += 5;

		// --- カス・タン・タネ（数字付き）の判定 ---
		if (roll.includes(U"Kasu")) // "Kasu"が含まれている場合
		{
			sum += Parse<int>(roll.removed(U"Kasu"));
		}
		else if (roll.includes(U"Tane")) // "Tane"が含まれている場合
		{
			sum += Parse<int>(roll.removed(U"Tane"));
		}
		// "Akatan"や"Aotan"に反応しないよう、"Tan"は厳密に区別するかelse ifで繋ぐ
		else if (roll.starts_with(U"Tan"))
		{
			sum += Parse<int>(roll.removed(U"Tan"));
		}
	}

	return sum;
}

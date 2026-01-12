# include "Title.h"

// コンストラクタの実装
// ClassName::FunctionName という書き方になります
Title::Title(const InitData& init)
	: IScene{ init }
	, font{ FontMethod::MSDF, 48, U"fonts/玉ねぎ楷書_激_無料版v7/玉ねぎ楷書「激」無料版v7/玉ねぎ楷書激無料版v7改.ttf" }
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	
}

// デストラクタの実装
Title::~Title()
{
	Print << U"Title::~Title()";
}

// 更新関数の実装
void Title::update()
{
	if (SimpleGUI::Button(U"Start", Vec2{ 350, 300 }, unspecified, true))
	{
		changeScene(U"Game");
	}
}

// 描画関数の実装
void Title::draw() const
{
	font(U"花札").draw(55, Vec2{ 350, 100 }, ColorF{ 0.11 });
}

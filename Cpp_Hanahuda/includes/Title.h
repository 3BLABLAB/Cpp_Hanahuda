#pragma once
# include "Common.h" // Appを使うためにインクルード

// クラスの定義（中身の関数は宣言だけにする）
class Title : public App::Scene
{
public:
	// コンストラクタ
	Title(const InitData& init);

	// デストラクタ
	~Title();

	// 更新関数
	void update() override;

	// 描画関数
	void draw() const override;

private:
	const Font font;
};

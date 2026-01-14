#pragma once
# include "Common.h" // Appを使うためにインクルード

// クラスの定義（中身の関数は宣言だけにする）
class Result : public App::Scene
{
public:
	// コンストラクタ
	Result(const InitData& init);

	// デストラクタ
	~Result();

	// 更新関数
	void update() override;

	// 描画関数
	void draw() const override;

private:
	Font font;

	int PointCheck(const std::vector <String>& Rolls) const;
};

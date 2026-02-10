#pragma once
# include "Common.h" // Appを使うためにインクルード

// クラスの定義（中身の関数は宣言だけにする）
class Game : public App::Scene
{
public:
	// コンストラクタ
	Game(const InitData& init);

	// デストラクタ
	~Game();

	// 更新関数
	void update() override;

	// 描画関数
	void draw() const override;

private:
	//変数群
	Font font;
	enum class GameState {
		Playing,
		KoiKoiCheck
	};
	GameState m_state = GameState::Playing;
	const Vec2 KoikoiButtonPos, AgariButtonPos;
	HudaFlagTable BahudaAppeared{};
	//A:Player B:Rival
	HudaFlagTable AMochihuda{};
	HudaFlagTable BMochihuda{};
	BahudaLines Bahuda{};
	TehudaLines ATehuda{}, BTehuda{};
	int turn;
	bool IsPlayerTurn = true;
	int SelectedIndex = -1;
	int DecidedIndex = -1;
	std::vector<String> ARolls, BRolls;
	Huda DecidedHuda;
	bool APreRoll, BPreRoll;

	//時間設定
	const double interval = 0.5;
	double accumlatedTime = 0.0;

	//関数群
	void DisplayBackground(const ColorF& centerColor, const ColorF& outerColor);
	void DrawTable();
	void DisplayTehuda();
	Huda GetNewHuda();
	void InitializeTable();
	int DetectSelectedTehuda(int& SelectedIndex, int& DecidedIndex);
	void HighlightCard(const Huda& card);
	void DrawYamahuda(bool turn);
	std::vector<bool> BahudaObtainable(TehudaLines Tehuda);

	//for Debugging
	void PrintMonthsWithCards();

	class DisplayMochihuda {
	public:
		static void Draw(const HudaFlagTable& AMochihuda, const HudaFlagTable& BMochihuda) {
			const double GroupWidth = Scene::Width() / 4.0;
			//Render A
			Vec2 BasePoint = AMochihudaBasePosition;
			Vec2 Position = BasePoint;
			for (int i = 0; i < 4; i++) {
				Position.x = BasePoint.x + i * GroupWidth;
				DisplayCategory(AMochihuda, i, Position);
			}
			//Render B
			BasePoint = BMochihudaBasePosition;
			Position = BasePoint;
			for (int i = 0; i < 4; i++) {
				Position.x = BasePoint.x + i * GroupWidth;
				DisplayCategory(BMochihuda, i, Position);
			}
		}
	private:
		static void DisplayCategory(const HudaFlagTable& Mochihuda, int category, Vec2& BasePoint) {
			const int gap = 20;
			switch (category) {
			case 0: {//光札
				int num = 0;
				for (auto p : HikariPairs) {
					if (Mochihuda[p.first][p.second]) {
						Huda huda(p.first, p.second);
						huda.SetPosition(BasePoint + Vec2{ gap * num, 0 });
						huda.Render();
						num++;
					}
				}
				break;
			}
			case 1: {//たね
				int num = 0;
				for (auto p : TanePairs) {
					if (Mochihuda[p.first][p.second]) {
						Huda huda(p.first, p.second);
						huda.SetPosition(BasePoint + Vec2{ gap * num, 0 });
						huda.Render();
						num++;
					}
				}
				break;
			}
			case 2: {//たん
				int num = 0;
				for (auto p : TanPairs) {
					if (Mochihuda[p.first][p.second]) {
						Huda huda(p.first, p.second);
						huda.SetPosition(BasePoint + Vec2{ gap * num, 0 });
						huda.Render();
						num++;
					}
				}
				break;
			}
			case 3: {//カス
				int num = 0;
				for (auto p : KasuPairs) {
					if (Mochihuda[p.first][p.second]) {
						Huda huda(p.first, p.second);
						huda.SetPosition(BasePoint + Vec2{ gap * num, 0 });
						huda.Render();
						num++;
					}
				}
				break;
			}
			default:break;
			}
		}
	};
};

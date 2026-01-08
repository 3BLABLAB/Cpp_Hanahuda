//"C:\Users\rk3b0\デスクトップ\github\Cpp_Hanahuda\Cpp_Hanahuda\App"
# include <RollChecks.h>
# include <memory>
# include <utility>


struct HudaTextureManager
{
	static inline std::unique_ptr<Texture> textures[12][4];
	static void Load()
	{
		for (int m = 0; m < 12; ++m)
		{
			for (int o = 0; o < 4; ++o)
			{
				const String path = U"Images/{}_{}.jpg"_fmt(m + 1, o + 1);
				// ファイル存在チェック
				if (!FileSystem::Exists(path))
				{
					throw Error{ U"[Texture Load Error] File not found : " + path };
				}
				// 読み込み
				auto tex = std::make_unique<Texture>(path);
				// Textureの生成失敗チェック
				if (!(*tex))
				{
					throw Error{ U"[Texture Load Error] Failed to load : " + path };
				}
				textures[m][o] = std::move(tex);
			}
		}
	}

	static const Texture& Get(int m, int o)
	{
		// 範囲チェック
		if (m < 0 || m >= 12 || o < 0 || o >= 4)
		{
			throw Error{
				U"[Texture Get Error] index out of range: m=" + Format(m)
				+ U", o=" + Format(o)
			};
		}
		// 未ロードチェック
		if (!textures[m][o])
		{
			throw Error{
				U"[Texture Get Error] texture not loaded : m=" + Format(m)
				+ U", o=" + Format(o)
				+ U" (call HudaTextureManager::Load() first)"
			};
		}
		return *textures[m][o];
	}
};

struct Huda {
	int width;
	int height;
	int month;
	int order;
	Vec2 pos = { 0,0 };

public: Huda() :month(0), order(0), width(40), height(60) {}
public: Huda(int m, int o) :month(m), order(o), width(40), height(60) {}
public:void SetBahuda() {
	this->width = 40;
	this->height = 60;
};
public:void SetMochihuda() {
	this->width = 40;
	this->height = 60;
};
public:void SetTehuda() {
	this->width = 80;
	this->height = 120;
};
public:void SetPosition(Vec2 position) {
	this->pos = position;
}
public:void Render() {
	HudaTextureManager::Get(month, order)
		.resized(width, height)
		.drawAt(pos);
}
};
using BahudaLines = std::array<Array<Huda>, 12>;
using TehudaLines = std::vector<Huda>;
using HudaFlagTable = std::array<std::array<bool, 4>, 12>;

constexpr Vec2 ATehudaBasePosition{ 200, 500 };
constexpr Vec2  BTehudaBasePosition{ 200, 100 };
constexpr Vec2  AMochihudaBasePosition{ 30, 400 };
constexpr Vec2  BMochihudaBasePosition{ 30, 200 };
constexpr std::array<std::pair<int, int>, 5> HikariPairs = {{
	{0,0},{2,0},{7,0},{11,0},{10,0}
}};
constexpr std::array<std::pair<int, int>, 9> TanePairs = { {
	{1,0},{3,0},{4,0},{5,0},{6,0},{7,1},{8,0},{9,0},{10,1}
} };
constexpr std::array<std::pair<int, int>, 10> TanPairs = { {
	{0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{8,1},{9,1},{10,2}
} };
constexpr std::array<std::pair<int, int>, 24> KasuPairs = { {
	{0,2},{0,3},{1,2},{1,3},{2,2},{2,3},{3,2},{3,3},{4,2},{4,3},{5,2},{5,3},
	{6,2},{6,3},{7,2},{7,3},{8,2},{8,3},{9,2},{9,3},{10,3},{11,1},{ 11,2 },{11,3}
} };


void DrawRadialGradientBackground(const ColorF& centerColor, const ColorF& outerColor)
{
	Circle{ Scene::Center(), (Scene::Size().length() * 0.5) }
	.draw(centerColor, outerColor);
}

void DrawTable(int turn, BahudaLines& Bahuda) {
	Vec2 CenterPos = Scene::Center();
	Huda huda;
	//Render yamahuda
	RectF(Arg::center(CenterPos), huda.width, huda.height)
		.drawFrame(1, Palette::Black)
		.draw(ColorF(0.26, 0.43, 0.43));
	//Render bahuda
	huda.SetBahuda();
	int gap = 10;
	float BaseDx = gap * 1.5 + gap * 2 + huda.width * 3;
	float BaseDy = gap * 0.5 + huda.height * 0.5;
	//１～3
	Vec2 BasePoint = CenterPos + Vec2{ -BaseDx , -BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		auto MonthBahuda = Bahuda[i];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	//４～６
	BasePoint = CenterPos + Vec2{ -BaseDx , +BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		auto MonthBahuda = Bahuda[i+3];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	//７～９
	BasePoint = CenterPos + Vec2{ +gap * 1.5 + huda.width , -BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		auto MonthBahuda = Bahuda[i+6];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	//１０～１２
	BasePoint = CenterPos + Vec2{ +gap * 1.5 + huda.width , +BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		auto MonthBahuda = Bahuda[i+9];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	return;
}

void DisplayTehuda(TehudaLines& ATehuda, TehudaLines& BTehuda) {
	//Render Tehuda
	//A
	Vec2 BasePoint = ATehudaBasePosition;
	int gap = -20;
	for (int i = 0; i < ATehuda.size(); i++) {
		Huda& huda = ATehuda[i];
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		huda.SetPosition(Position);
		huda.Render();
	}
	//B
	BasePoint = BTehudaBasePosition;
	gap = -20;
	for (int i = 0; i < BTehuda.size(); i++) {
		Huda& huda = BTehuda[i];
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		RectF(Arg::center(Position), huda.width, huda.height)
			.drawFrame(1, Palette::Black)
			.draw(ColorF(0.26, 0.43, 0.43));
	}
	return;
}

Huda GetNewHuda(HudaFlagTable& BahudaAppeared) {
	int NewHudaNum = Random(0, 47);
	int m, o;
	do
	{
		NewHudaNum = Random(0, 47);//0-47
		m = NewHudaNum / 4;
		o = NewHudaNum % 4;
	} while (BahudaAppeared[m][o]);
	BahudaAppeared[m][o] = true;
	Huda NewHuda = Huda(m, o);
	return NewHuda;
}

void InitializeTable(HudaFlagTable& BahudaAppeared,BahudaLines& Bahuda, TehudaLines& ATehuda, TehudaLines& BTehuda) {
	//setting bahuda
	int nonEmptyMonthCount = 0;
	while (nonEmptyMonthCount < 8)
	{
		Huda NewHuda = GetNewHuda(BahudaAppeared);
		NewHuda.SetBahuda();
		Bahuda[NewHuda.month].push_back(NewHuda);
		nonEmptyMonthCount = 0;
		for (int i = 0; i < 12; i++)
		{
			if (!Bahuda[i].isEmpty())
			{
				nonEmptyMonthCount++;
			}
		}
	}
	//setting tehuda
	for (int i = 0; i < 8; i++) {
		Huda NewHuda = GetNewHuda(BahudaAppeared);
		NewHuda.SetTehuda();
		ATehuda.push_back(NewHuda);
		NewHuda = GetNewHuda(BahudaAppeared);
		NewHuda.SetTehuda();
		BTehuda.push_back(NewHuda);
	}
}

//for Debugging
void PrintMonthsWithCards(const BahudaLines& bahuda)
{
	String line;
	bool first = true;

	for (int i = 0; i < 12; ++i)
	{
		if (!bahuda[i].isEmpty())
		{
			if (!first)
			{
				line += U", ";
			}
			line += Format(i + 1); // 月は 1〜12 で表示
			first = false;
		}
	}

	if (line.isEmpty())
	{
		line = U"(none)";
	}

	Print << U"Months with >=1 card: " << line;
}

//プレイヤーの手札クリック検知
//戻り値: クリックされた手札の index（0～7）or -1
int DetectSelectedTehuda(const TehudaLines& ATehuda,int& SelectedIndex, int& DecidedIndex)
{
	Vec2 BasePoint = Vec2{ 200, 500 };
	int gap = -20;

	int w = ATehuda[0].width;
	int h = ATehuda[0].height;

	for (int i = 0; i < ATehuda.size(); i++)
	{
		Vec2 pos = BasePoint + i * Vec2(w + gap, 0);

		// 画面上の当たり判定
		RectF hitbox(Arg::center(pos), w, h);

		if (hitbox.mouseOver())
		{
			SelectedIndex = i;
			// ダブルクリックで確定
			if (hitbox.leftClicked())
			{
				DecidedIndex = i;
			}
			break;
		}
	}
	return -1;
}

void HighlightCard(const Huda& card)
{
	RectF frame(
		Arg::center(card.pos),   // 描画位置と同一座標
		card.width + 8,
		card.height + 8
	);

	frame.drawFrame(5, Palette::Yellow);
}

void DrawYamahuda(BahudaLines& Bahuda, HudaFlagTable& BahudaAppeared, HudaFlagTable& Mochihuda) {
	Huda NewHuda = GetNewHuda(BahudaAppeared);
	if (!Bahuda[NewHuda.month].isEmpty()) {
		for (auto huda : Bahuda[NewHuda.month]) {
			Mochihuda[huda.month][huda.order] = true;
		}
		return;
	}
	Bahuda[NewHuda.month].push_back(NewHuda);
	return;
}

bool BahudaObtainable(const BahudaLines& Bahuda, const TehudaLines& Tehuda) {
	bool ans = false;
	for (int i = 0; i < Tehuda.size(); i++) {
		if (!Bahuda[Tehuda[i].month].isEmpty()) {
			ans = true;
			break;
		}
	}
	return ans;
}

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
	static void DisplayCategory(const HudaFlagTable& Mochihuda,int category, Vec2& BasePoint) {
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


void Main()
{
	//Print << FileSystem::CurrentDirectory();

	HudaFlagTable BahudaAppeared{};
	//A:Player B:Rival
	HudaFlagTable AMochihuda{};
	HudaFlagTable BMochihuda{};
	// 背景の色を設定する | Set the background color
	//Scene::SetBackground(ColorF{ 0.3, 0.9, 0.6 });
	int turn = 0;
	BahudaLines Bahuda;
	TehudaLines ATehuda, BTehuda;
	HudaTextureManager::Load();
	InitializeTable(BahudaAppeared,Bahuda, ATehuda, BTehuda);

	bool IsPlayerTurn = true;
	int SelectedIndex = -1;   
	int DecidedIndex = -1;
	std::vector<String> ARolls(8), BRolls(8);
	Huda DecidedHuda;

	//時間設定
	const double interval = 0.5;
	double accumlatedTime = 0.0;

	while (System::Update())
	{
		//accumlatedTime += Scene::DeltaTime();
		//if (interval <= accumlatedTime)
		//{
		//	accumlatedTime -= interval;
		//}
		
		//このループは無数に繰り返す
		//基本的にどちらかのターンのスコープの中に処理を書く
		//for debugging
		if (MouseR.down()) {
			AMochihuda[0][0] = true;
			AMochihuda[2][0] = true;
			AMochihuda[7][0] = true;
		}
		//for Debugging
		Print << ARolls;
		DrawRadialGradientBackground(ColorF{ 0.2, 0.8, 0.4 }, ColorF{ 0.26, 0.43, 0.35 });
		DrawTable(turn, Bahuda);

		if (ATehuda.empty() || BTehuda.empty()){
			return ;
		}
		if (IsPlayerTurn) {
			bool obtainable = BahudaObtainable(Bahuda, ATehuda);//獲得可能か
			Print << U"A: " << obtainable;
			int Clicked = DetectSelectedTehuda(ATehuda,SelectedIndex,DecidedIndex);
			//マウスオーバーされたものを強調表示
			if (SelectedIndex != -1)
			{
				HighlightCard(ATehuda[SelectedIndex]);
			}
			//ダブルクリックで決定
			if (DecidedIndex != -1)
			{
				DecidedHuda = ATehuda[DecidedIndex];
				if (!obtainable) {
					//場札に追加
					Bahuda[DecidedHuda.month].push_back( DecidedHuda);
					//手札から削除
					ATehuda.erase(ATehuda.begin() + DecidedIndex);
					SelectedIndex = -1;
					DecidedIndex = -1;
				}
				else if (!Bahuda[DecidedHuda.month].isEmpty()) {
					//場札から獲得
					for (auto huda : Bahuda[DecidedHuda.month]) {
						AMochihuda[huda.month][huda.order] = true;
					}
					//場札から削除
					Bahuda[DecidedHuda.month].clear();
					//手札から獲得
					AMochihuda[DecidedHuda.month][DecidedHuda.order] = true;
					//手札から削除
					ATehuda.erase(ATehuda.begin()+ DecidedIndex);
					SelectedIndex = -1;
					DecidedIndex = -1;
				}
				//Check Rolls
				CheckRolls(AMochihuda, ARolls);
				IsPlayerTurn = !IsPlayerTurn;
				DrawYamahuda(Bahuda, BahudaAppeared,AMochihuda);
			}
		}
		else {
			bool obtainable = BahudaObtainable(Bahuda, BTehuda);//獲得可能か
			Print << U"B: " << obtainable;
			
			for (int CPUCandidate = 0; CPUCandidate < BTehuda.size(); CPUCandidate++) {
				DecidedHuda = BTehuda[CPUCandidate];
				if (!obtainable) {
					Bahuda[DecidedHuda.month].push_back(DecidedHuda);
					IsPlayerTurn = !IsPlayerTurn;
					break;
				}
				else if (!Bahuda[DecidedHuda.month].isEmpty()) {
					//場札から獲得
					for (auto huda : Bahuda[DecidedHuda.month]) {
						BMochihuda[huda.month][huda.order] = true;
					}
					//場札から削除
					Bahuda[DecidedHuda.month].clear();
					//手札から獲得
					BMochihuda[DecidedHuda.month][DecidedHuda.order] = true;
					//手札から削除
					BTehuda.erase(BTehuda.begin() + CPUCandidate);
					break;
				}
			}
			//Check Rolls
			CheckRolls(BMochihuda, BRolls);
			IsPlayerTurn = !IsPlayerTurn;
			DrawYamahuda(Bahuda, BahudaAppeared, BMochihuda);
		}
		DisplayTehuda(ATehuda, BTehuda);
		DisplayMochihuda::Draw(AMochihuda, BMochihuda);
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//

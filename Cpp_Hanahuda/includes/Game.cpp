# include "Game.h"
# include <RollChecks.h>

// コンストラクタの実装
// ClassName::FunctionName という書き方になります
Game::Game(const InitData& init)
	: IScene{ init }
	, APreRoll{false}
	, BPreRoll{false}
	, ARolls(9)
	, BRolls(9)
	, font{ FontMethod::MSDF, 48, U"fonts/玉ねぎ楷書_激_無料版v7/玉ねぎ楷書「激」無料版v7/玉ねぎ楷書激無料版v7改.ttf" }
{
	for (auto& row : BahudaAppeared) { row.fill(false); }
	for (auto& row : AMochihuda) { row.fill(false); }
	for (auto& row : BMochihuda) { row.fill(false); }
	Print << U"Game::Game()";
	turn = 0;//偶数がプレイヤー
	InitializeTable();
	//Print << ATehuda;
	getData().ARolls.clear();
	getData().BRolls.clear();
}

// デストラクタの実装
Game::~Game()
{
	Print << U"Game::~Game()";
}

// 更新関数の実装
void Game::update()
{
	if (m_state == GameState::KoiKoiCheck)
	{
		// こいこいボタン
		if (RectF(250, 320, 120, 50).leftClicked())
		{
			// ゲーム再開
			m_state = GameState::Playing;
			IsPlayerTurn = !IsPlayerTurn;
		}

		//	上がりボタン
		if (RectF(430, 320, 120, 50).leftClicked())
		{
			getData().ARolls = this->ARolls;
			getData().BRolls = this->BRolls;
			changeScene(U"Result");
		}
		// ★ここで return することで、下のゲーム進行処理を実行させない（時を止める）
		return;
	}
	if (ATehuda.empty() || BTehuda.empty()) {
		getData().ARolls = this->ARolls;
		getData().BRolls = this->BRolls;
		changeScene(U"Result");
		return;
	}
	if (MouseR.down())
	{
		getData().ARolls = this->ARolls;
		getData().BRolls = this->BRolls;
		changeScene(U"Result");
		return;
	}
	if (IsPlayerTurn) {
		bool obtainable = BahudaObtainable(turn);//獲得可能か
		int Clicked = DetectSelectedTehuda(SelectedIndex, DecidedIndex);
		
		//ダブルクリックで決定
		if (DecidedIndex != -1)
		{
			DecidedHuda = ATehuda[DecidedIndex];
			if (!obtainable) {
				//場札に追加
				DecidedHuda.SetBahuda();
				Bahuda[DecidedHuda.month].push_back(DecidedHuda);
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
				ATehuda.erase(ATehuda.begin() + DecidedIndex);
				SelectedIndex = -1;
				DecidedIndex = -1;
			}
			//Check Rolls
			CheckRolls(AMochihuda, ARolls, APreRoll);
			IsPlayerTurn = !IsPlayerTurn;
			DrawYamahuda(turn);
		}
	}
	else {
		bool obtainable = BahudaObtainable(turn);//獲得可能か
		
		for (int CPUCandidate = 0; CPUCandidate < BTehuda.size(); CPUCandidate++) {
			DecidedHuda = BTehuda[CPUCandidate];
			if (!obtainable) {
				//場札に追加
				DecidedHuda.SetBahuda();
				Bahuda[DecidedHuda.month].push_back(DecidedHuda);
				continue;
			}
			//場札から獲得可能
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
		CheckRolls(BMochihuda, BRolls, BPreRoll);
		IsPlayerTurn = !IsPlayerTurn;
		DrawYamahuda(turn);
	}
	//こいこいチェック
	for (const String& roll : ARolls) {
		if(roll!=U"")m_state = GameState::KoiKoiCheck;
	}


	DisplayTehuda();
	DisplayMochihuda::Draw(AMochihuda, BMochihuda);
}

// 描画関数の実装
void Game::draw() const
{
	// const関数の中から非constなヘルパー関数を呼ぶためのキャスト
	Game* self = const_cast<Game*>(this);
	// 1. 背景描画
	self->DisplayBackground(ColorF{ 0.2, 0.8, 0.4 }, ColorF{ 0.26, 0.43, 0.35 });

	// 2. 場札描画
	self->DrawTable(turn);

	// 3. 手札描画
	self->DisplayTehuda();

	// 4. 獲得札描画
	DisplayMochihuda::Draw(AMochihuda, BMochihuda);

	// 5. 選択中の手札ハイライト
	if (IsPlayerTurn && SelectedIndex != -1)
	{
		// 配列外参照防止
		if (SelectedIndex >= 0 && SelectedIndex < ATehuda.size())
		{
			self->HighlightCard(ATehuda[SelectedIndex]);
		}
	}

	if (m_state == GameState::KoiKoiCheck)
	{
		// 画面全体を半透明の黒で覆う (Alpha 0.6)
		Scene::Rect().draw(ColorF{ 0.0, 0.6 });

		// ダイアログっぽい枠
		RectF(Arg::center(400, 300), 400, 200)
			.draw(Palette::White)
			.drawFrame(2, Palette::Black);

		FontAsset(U"TitleFont")(U"こいこいしますか？").drawAt(400, 250, Palette::Black);

		RectF(250, 320, 120, 50).draw(Palette::Orange).drawFrame(1, Palette::Black);
		FontAsset(U"TitleFont")(U"こいこい").drawAt(310, 345, Palette::White);

		RectF(430, 320, 120, 50).draw(Palette::Red).drawFrame(1, Palette::Black);
		FontAsset(U"TitleFont")(U"勝負").drawAt(490, 345, Palette::White);
	}
}

//表示系関数
void Game::DisplayBackground(const ColorF& centerColor, const ColorF& outerColor)
{
	Circle{ Scene::Center(), (Scene::Size().length() * 0.5) }
	.draw(centerColor, outerColor);
}

void Game::DisplayTehuda() {
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

void Game::HighlightCard(const Huda& card)
{
	RectF frame(
		Arg::center(card.pos),   // 描画位置と同一座標
		card.width + 8,
		card.height + 8
	);

	frame.drawFrame(5, Palette::Yellow);
}

//機能系関数
void Game::DrawTable(int turn) {
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
		auto MonthBahuda = Bahuda[i + 3];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	//７～９
	BasePoint = CenterPos + Vec2{ +gap * 1.5 + huda.width , -BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		auto MonthBahuda = Bahuda[i + 6];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	//１０～１２
	BasePoint = CenterPos + Vec2{ +gap * 1.5 + huda.width , +BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		auto MonthBahuda = Bahuda[i + 9];
		for (auto& huda : MonthBahuda) {
			huda.SetPosition(Position);
			huda.Render();
		}
	}
	return;
}

Huda Game::GetNewHuda() {
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

void Game::InitializeTable() {
	//setting bahuda
	int nonEmptyMonthCount = 0;
	while (nonEmptyMonthCount < 8)
	{
		Huda NewHuda = GetNewHuda();
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
		Huda NewHuda = GetNewHuda();
		NewHuda.SetTehuda();
		ATehuda.push_back(NewHuda);
		NewHuda = GetNewHuda();
		NewHuda.SetTehuda();
		BTehuda.push_back(NewHuda);
	}
}

//プレイヤーの手札クリック検知
//戻り値: クリックされた手札の index（0～7）or -1
int Game::DetectSelectedTehuda(int& SelectedIndex, int& DecidedIndex)
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

void Game::DrawYamahuda(int turn) {
	HudaFlagTable* Mochihuda;
	if (turn % 2 == 0) Mochihuda = &AMochihuda;
	else Mochihuda = &BMochihuda;

	Huda NewHuda = GetNewHuda();
	if (!Bahuda[NewHuda.month].isEmpty()) {
		for (auto huda : Bahuda[NewHuda.month]) {
			(*Mochihuda)[huda.month][huda.order] = true;
		}
		Bahuda[NewHuda.month].clear();
		(*Mochihuda)[NewHuda.month][NewHuda.order] = true;
		return;
	}
	Bahuda[NewHuda.month].push_back(NewHuda);
	return;
}

bool Game::BahudaObtainable(int turn) {
	TehudaLines Tehuda;
	if (turn % 2 == 0)Tehuda = ATehuda;
	else Tehuda = BTehuda;

	bool ans = false;
	for (int i = 0; i < Tehuda.size(); i++) {
		if (!Bahuda[Tehuda[i].month].isEmpty()) {
			ans = true;
			break;
		}
	}
	return ans;
}


//for Debugging
void Game::PrintMonthsWithCards()
{
	String line;
	bool first = true;

	for (int i = 0; i < 12; ++i)
	{
		if (!Bahuda[i].isEmpty())
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

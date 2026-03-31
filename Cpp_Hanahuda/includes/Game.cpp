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
	,KoikoiButtonPos { 300, 320 }
	,AgariButtonPos { 500, 320 }
{
	for (auto& row : BahudaAppeared) { row.fill(false); }
	for (auto& row : AMochihuda) { row.fill(false); }
	for (auto& row : BMochihuda) { row.fill(false); }
	Print << U"Game::Game()";
	InitializeTable();
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
		RectF KoikoiButton = RectF(Arg::center(KoikoiButtonPos), 120, 50);
		RectF AgariButton = RectF(Arg::center(AgariButtonPos), 120, 50);
		// こいこいボタン
		if (KoikoiButton.leftClicked())
		{
			// ゲーム再開
			m_state = GameState::Playing;
			IsPlayerTurn = !IsPlayerTurn;
		}

		//	上がりボタン
		if (AgariButton.leftClicked())
		{
			getData().ARolls = this->ARolls;
			getData().BRolls = this->BRolls;
			changeScene(U"Result");
		}
	}
	if (ATehuda.empty() || BTehuda.empty()) {
		getData().ARolls = this->ARolls;
		getData().BRolls = this->BRolls;
		changeScene(U"Result");
		return;
	}

	if (IsPlayerTurn) {
		std::vector<bool> obtainable = BahudaObtainable(ATehuda);//獲得可能か
		DetectSelectedTehuda(SelectedIndex, DecidedIndex);
		
		//ダブルクリックで決定
		if (DecidedIndex != -1)
		{
			int YakuCount = 0;
			for (auto i : ARolls) {
				if (!i.isEmpty())YakuCount++;
			}
			DecidedHuda = ATehuda[DecidedIndex];

			//場札から獲得できないなら
			if (!obtainable[DecidedIndex]) {
				Print << U"not obtainable";
				//場札に追加
				DecidedHuda.SetBahuda();
				Bahuda[DecidedHuda.month].push_back(DecidedHuda);
				//手札から削除
				ATehuda.erase(ATehuda.begin() + DecidedIndex);
			}
			else if (!Bahuda[DecidedHuda.month].isEmpty()) {
				Print << U"GetfromBahuda";
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
				
			}
			DrawYamahuda(IsPlayerTurn);
			//Check Rolls
			CheckRolls(AMochihuda, ARolls, APreRoll);

			int t_YakuCount = 0;
			for (auto i : ARolls) {
				if (!i.isEmpty())t_YakuCount++;
			}
			if (t_YakuCount > YakuCount)
			{
				m_state = GameState::KoiKoiCheck;
			}
			SelectedIndex = -1;
			DecidedIndex = -1;
			IsPlayerTurn = !IsPlayerTurn;
		}
	}
	else { // CPUのターン
		std::vector<bool> obtainable = BahudaObtainable(BTehuda);

		int targetIndex = -1; // 出す手札のインデックス

		//場札を取れるカードを探す
		for (int i = 0; i < BTehuda.size(); ++i) {
			if (obtainable[i]) {
				targetIndex = i;
				break; // 取れる札が見つかったら、それを出すことに決めて検索終了
			}
		}
		//取れる札がなければ、一番左の札を捨てる
		if (targetIndex == -1) {
			targetIndex = 0;
		}
		DecidedHuda = BTehuda[targetIndex];

		//取れる札がないので捨てる
		if (!obtainable[targetIndex]) {
			// 場札に追加
			DecidedHuda.SetBahuda();
			Bahuda[DecidedHuda.month].push_back(DecidedHuda);

			// 手札から削除
			BTehuda.erase(BTehuda.begin() + targetIndex);
		}
		//場札を獲得する
		else {
			// 場札から獲得
			for (auto huda : Bahuda[DecidedHuda.month]) {
				BMochihuda[huda.month][huda.order] = true;
			}
			// 場札から削除
			Bahuda[DecidedHuda.month].clear();
			// 手札から獲得
			BMochihuda[DecidedHuda.month][DecidedHuda.order] = true;

			// 手札から削除
			BTehuda.erase(BTehuda.begin() + targetIndex);
		}

		// Check Rolls
		int YakuCount = 0;
		for (auto i : BRolls) {
			if (!i.isEmpty())YakuCount++;
		}
		CheckRolls(BMochihuda, BRolls, BPreRoll);
		DrawYamahuda(IsPlayerTurn);

		int t_YakuCount = 0;
		for (auto i : BRolls) {
			if (!i.isEmpty())t_YakuCount++;
		}
		// CPUが新しい役を成立させたら即上がり
		if (t_YakuCount > YakuCount)
		{
			getData().ARolls = this->ARolls;
			getData().BRolls = this->BRolls;
			changeScene(U"Result");
			return;
		}
		IsPlayerTurn = !IsPlayerTurn;
	}


}

// 描画関数の実装
void Game::draw() const
{
	DisplayBackground(ColorF{ 0.2, 0.8, 0.4 }, ColorF{ 0.26, 0.43, 0.35 });

	// 2. 場札描画
	DrawTable();

	// 3. 手札描画
	DisplayTehuda();

	// 4. 獲得札描画
	DisplayMochihuda::Draw(AMochihuda, BMochihuda);

	// 5. 選択中の手札ハイライト
	if (IsPlayerTurn && SelectedIndex != -1)
	{
		if (SelectedIndex >= 0 && SelectedIndex < ATehuda.size())
		{
			const Huda& card = ATehuda[SelectedIndex];
			Vec2 pos = ATehudaBasePosition + SelectedIndex * Vec2(card.width + (-20), 0);
			HighlightCard(card, pos);
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

		//RectF KoikoiButton = RectF(Arg::center(KoikoiButtonPos), 120, 50);
		RectF(Arg::center(KoikoiButtonPos), 150, 50).draw(Palette::Orange).drawFrame(1, Palette::Black);
		font(U"こいこい").drawAt(40, KoikoiButtonPos, Palette::Black);

		RectF(Arg::center(AgariButtonPos), 150, 50).draw(Palette::Red).drawFrame(1, Palette::Black);
		font(U"勝負").drawAt(40, AgariButtonPos, Palette::Black);
	}
}

//表示系関数
void Game::DisplayBackground(const ColorF& centerColor, const ColorF& outerColor) const
{
	Circle{ Scene::Center(), (Scene::Size().length() * 0.5) }
	.draw(centerColor, outerColor);
}

void Game::DisplayTehuda() const {
	//Render Tehuda
	//A
	Vec2 BasePoint = ATehudaBasePosition;
	int gap = -20;
	for (int i = 0; i < ATehuda.size(); i++) {
		const Huda& huda = ATehuda[i];
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		huda.RenderAt(Position);
	}
	//B
	BasePoint = BTehudaBasePosition;
	gap = -20;
	for (int i = 0; i < BTehuda.size(); i++) {
		const Huda& huda = BTehuda[i];
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		RectF(Arg::center(Position), huda.width, huda.height)
			.drawFrame(1, Palette::Black)
			.draw(ColorF(0.26, 0.43, 0.43));
	}
}

void Game::HighlightCard(const Huda& card, Vec2 position) const
{
	RectF frame(
		Arg::center(position),
		card.width + 8,
		card.height + 8
	);

	frame.drawFrame(5, Palette::Yellow);
}

//機能系関数
void Game::DrawTable() const {
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
		const auto& MonthBahuda = Bahuda[i];
		for (const auto& h : MonthBahuda) {
			h.RenderAt(Position);
		}
	}
	//４～６
	BasePoint = CenterPos + Vec2{ -BaseDx , +BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		const auto& MonthBahuda = Bahuda[i + 3];
		for (const auto& h : MonthBahuda) {
			h.RenderAt(Position);
		}
	}
	//７～９
	BasePoint = CenterPos + Vec2{ +gap * 1.5 + huda.width , -BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		const auto& MonthBahuda = Bahuda[i + 6];
		for (const auto& h : MonthBahuda) {
			h.RenderAt(Position);
		}
	}
	//１０～１２
	BasePoint = CenterPos + Vec2{ +gap * 1.5 + huda.width , +BaseDy };
	for (int i = 0; i < 3; i++) {
		Vec2 Position = BasePoint + i * Vec2(huda.width + gap, 0);
		const auto& MonthBahuda = Bahuda[i + 9];
		for (const auto& h : MonthBahuda) {
			h.RenderAt(Position);
		}
	}
}

Huda Game::DrawFromDeck() {
	int idx = deck[deckPos++];
	return Huda(idx / 4, idx % 4);
}

void Game::InitializeTable() {
	// デッキ作成＆シャッフル
	deck.resize(48);
	for (int i = 0; i < 48; i++) deck[i] = i;
	deck.shuffle();
	deckPos = 0;

	// 場札8枚
	for (int i = 0; i < 8; i++) {
		Huda NewHuda = DrawFromDeck();
		NewHuda.SetBahuda();
		Bahuda[NewHuda.month].push_back(NewHuda);
	}
	// 手札8枚ずつ
	for (int i = 0; i < 8; i++) {
		Huda NewHuda = DrawFromDeck();
		NewHuda.SetTehuda();
		ATehuda.push_back(NewHuda);
		NewHuda = DrawFromDeck();
		NewHuda.SetTehuda();
		BTehuda.push_back(NewHuda);
	}
}

void Game::DetectSelectedTehuda(int& SelectedIndex, int& DecidedIndex)
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
			// クリックで確定
			if (hitbox.leftClicked())
			{
				DecidedIndex = i;
			}
			break;
		}
	}
}

void Game::DrawYamahuda(bool IsPlayerTurn) {
	HudaFlagTable* Mochihuda;
	if (IsPlayerTurn) Mochihuda = &AMochihuda;
	else Mochihuda = &BMochihuda;

	Huda NewHuda = DrawFromDeck();
	if (!Bahuda[NewHuda.month].isEmpty()) {
		for (auto huda : Bahuda[NewHuda.month]) {
			(*Mochihuda)[huda.month][huda.order] = true;
		}
		Bahuda[NewHuda.month].clear();
		(*Mochihuda)[NewHuda.month][NewHuda.order] = true;
		return;
	}
	NewHuda.SetBahuda();
	Bahuda[NewHuda.month].push_back(NewHuda);
}

std::vector<bool> Game::BahudaObtainable(TehudaLines Tehuda) {
	std::vector<bool> ans(Tehuda.size(), false);
	for (int i = 0; i < Tehuda.size(); i++) {
		if (!Bahuda[Tehuda[i].month].isEmpty()) {
			ans[i] = true;
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

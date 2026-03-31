#pragma once
# include <Siv3D.hpp>

//テクスチャ管理
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

//札クラス
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
public:void RenderAt(Vec2 position) const {
	HudaTextureManager::Get(month, order)
		.resized(width, height)
		.drawAt(position);
}
};

// シーンの名前（キー）の型
using State = String;

//データ型
using HudaFlagTable = std::array<std::array<bool, 4>, 12>;
using BahudaLines = std::array<Array<Huda>, 12>;
using TehudaLines = std::vector<Huda>;

constexpr Vec2 ATehudaBasePosition{ 200, 500 };
constexpr Vec2  BTehudaBasePosition{ 200, 100 };
constexpr Vec2  AMochihudaBasePosition{ 30, 400 };
constexpr Vec2  BMochihudaBasePosition{ 30, 200 };
constexpr std::array<std::pair<int, int>, 5> HikariPairs = { {
	{0,0},{2,0},{7,0},{11,0},{10,0}
} };
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

// 共有データの型（今回は空の構造体とします。必要なら追加）
struct GameData
{
	// int score = 0; など
	std::vector<String> ARolls, BRolls;
};

// シーンマネージャーの型定義
using App = SceneManager<State,GameData>;

//"C:\Users\rk3b0\デスクトップ\github\Cpp_Hanahuda\Cpp_Hanahuda\App"
# include <Result.h>
# include <Game.h>
# include <Common.h>
# include <memory>
# include <utility>


void Main()
{
	HudaTextureManager::Load();
	App manager;
	manager.add<Game>(U"Game");
	manager.add<Result>(U"Result");

	//Print << FileSystem::CurrentDirectory();
	manager.init(U"Game");
	//manager.changeScene(U"Result");
	
	while (System::Update())
	{
		//このループは無数に繰り返す
		//基本的にどちらかのターンのスコープの中に処理を書く
		
		if (not manager.update())
		{
			break;
		}
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

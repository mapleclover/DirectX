#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//関数プロト?イプ
void mainLoop();

//グロ?バル変数
State* gState = 0;

//ユ?ザ実装関数。中身はmainLoop()に丸投げ
namespace GameLib{
	void Framework::update(){
		mainLoop();
	}
}

void mainLoop(){
	//???ン押されてる？
	if ( Framework::instance().isEndRequested() ){
		if ( gState ){
			delete gState;
			gState = 0;
		}
		return;
	}
	//最初のフレ??は初期化。最初の状態を?画して終わり。
	if ( !gState ){ 
		File file( "stageData.txt" );
		if ( !( file.data() ) ){ //デ??ない！
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State( file.data(), file.size() );
		//初回?画
		gState->draw();
		return; //そのまま終わる
	}
	bool cleared = false;
	//メインル?プ
	//クリア?ェック
	if ( gState->hasCleared() ){
		cleared = true;
	}
	//入力取得
	cout << "a:left s:right w:up z:down. command?" << endl; //?作説明
	char input;
	cin >> input;
	//終了判定
	if ( input == 'q' ){
		delete gState;
		gState = 0;
		Framework::instance().requestEnd();
		return;
	}
	//更新
	gState->update( input );
	//?画
	gState->draw();

	if ( cleared ){
		//祝いのメッセ?ジ
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
}



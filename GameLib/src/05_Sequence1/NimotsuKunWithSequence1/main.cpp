#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"
#include "Image.h"

//関数プロト?イプ
void mainLoop();
void game(); //ゲ??ル?プ
void title(); //?イトル画面ル?プ

//シ?ケンス列挙
enum Sequence{
	SEQUENCE_GAME,
	SEQUENCE_TITLE,
};

//グロ?バル変数
State* gState = 0; //ゲ??状態
Image* gTitleImage = 0; //?イトル画面画像
Sequence gSequence = SEQUENCE_TITLE; //シ?ケンス番号
int gCounter = 0;

//ユ?ザ実装関数。中身はmainLoop()に丸投げ
namespace GameLib{
	void Framework::update(){
		mainLoop();
	}
}

void mainLoop(){
	//フレ??レ?ト調整
	Framework f = Framework::instance();
	f.setFrameRate( 60 ); //一回呼べばいいのだが面?なので呼んでしまう。

	if ( gCounter % 60 == 0 ){ //60フレ??に一回フレ??レ?ト?示
		cout << " FrameRate:" << f.frameRate() << endl;
	}
	++gCounter;

	//シ?ケンス分岐
	switch ( gSequence ){
		case SEQUENCE_GAME: 
			game();
			break;
		case SEQUENCE_TITLE:
			title();
			break;
	}
	//終了判定(qが押されたか、?ウスで???ンが?かれたか)
	if ( f.isKeyOn( 'q' ) ){
		f.requestEnd();
	}
	if ( f.isEndRequested() ){
		SAFE_DELETE( gState );
		SAFE_DELETE( gTitleImage );
	}
	f.sleep(10);
}

//ゲ??ル?プ。前のサンプルまでのGameLoop()の大半をこっちに移植する。
void game(){
	//最初のフレ??は初期化。
	if ( !gState ){ 
		File file( "stageData.txt" );
		if ( !( file.data() ) ){ //デ??ない！
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State( file.data(), file.size() );
	}
	bool cleared = false;
	//メインル?プ
	//クリア?ェック
	if ( gState->hasCleared() ){
		cleared = true;
	}
	//入力取得
	Framework f = Framework::instance();
	int dx = 0;
	int dy = 0;
	if ( f.isKeyOn( 'a' ) ){
		dx -= 1;
	}else if ( f.isKeyOn( 's' ) ){
		dx += 1;
	}else if ( f.isKeyOn( 'w' ) ){
		dy -= 1;
	}else if ( f.isKeyOn( 'z' ) ){
		dy += 1;
	}
	//更新
	gState->update( dx, dy );
	//?画
	gState->draw();

	//クリアしたなら後始末して?イトル画面へ
	if ( cleared ){
		gSequence = SEQUENCE_TITLE;
		SAFE_DELETE( gState );
	}
}

void title(){
	//最初のフレ??は初期化。
	if ( !gTitleImage ){
		gTitleImage = new Image( "title.dds" );
	}
	//入力取得
	if ( Framework::instance().isKeyOn( ' ' ) ){
		gSequence = SEQUENCE_GAME;
		SAFE_DELETE( gTitleImage );
		return;
	}
	gTitleImage->draw( 0, 0, 0, 0, gTitleImage->width(), gTitleImage->height() );
}


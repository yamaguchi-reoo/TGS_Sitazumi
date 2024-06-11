#pragma once

//画面サイズ
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 100

#define OBJECT_NUM	10000		//１ステージに存在できるプレイヤー、ブロック、敵の合計数
#define MAX_STAGE_HEIGHT 500	//ステージのブロックの縦の最大個数
#define MAX_STAGE_WIDTH  750	//ステージのブロックの横の最大個数
#define BOX_HEIGHT	40			//ステージのブロック一つ当たりの高さ
#define BOX_WIDTH	40			//ステージのブロック一つ当たりの幅

#define PLAYER_SPAWN_NUM 9      //プレイヤーの初期スポーン設定ブロックがUIの何番目か(EditScene専用)

#define SWAP_EFFECT_TIMER 30			//交換のアニメーション時間
#define SWAP_EFFECT_SIZE 10				//交換のアニメーションサイズ
#define SWAP_EFFECT_SPEED (SWAP_EFFECT_TIMER/3)			//交換のアニメーション速度(数字が小さいほど速い)
#define SWAP_EFFECT_STOP_TIME (SWAP_EFFECT_TIMER/3)		//交換のアニメーション後の硬直

#define DEFAULT_FREQ 40000			//音声再生速度用 通常速度

#define FADEIN_TIME 120				//ゲームメインに遷移して来た時のフェードイン演出の時間



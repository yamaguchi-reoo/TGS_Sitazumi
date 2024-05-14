#pragma once

//画面サイズ
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PLAYER_WIDTH 75
#define PLAYER_HEIGHT 150

#define OBJECT_NUM	4000		//１ステージに存在できるプレイヤー、ブロック、敵の合計数
#define MAX_STAGE_HEIGHT 500	//ステージのブロックの縦の最大個数
#define MAX_STAGE_WIDTH  750	//ステージのブロックの横の最大個数
#define BOX_HEIGHT	40			//ステージのブロック一つ当たりの高さ
#define BOX_WIDTH	40			//ステージのブロック一つ当たりの幅

#define PLAYER_SPAWN_NUM 9      //プレイヤーの初期スポーン設定ブロックがUIの何番目か(EditScene専用)

#define SWAP_EFFECT_TIMER 30			//交換のアニメーション時間
#define SWAP_EFFECT_SPEED 13			//交換のアニメーション速度(数字が小さいほど速い)
#define SWAP_EFFECT_STOP_TIME 10		//交換のアニメーション後の硬直


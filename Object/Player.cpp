#include "Player.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include "../Scene/GameMain.h"
#include <math.h>

// Playerのhpが変わるのはメモリの破損のせいかな？
int Player::hp = 5;

Player::Player()
{
	frame = 0;
	type = PLAYER;
	can_swap = TRUE;	//プレイヤーのcan_swapは真でも偽でも大丈夫
	can_hit = TRUE;

	vector = { 0,0 };

	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
	}

	searchedLen = 1000.f;
	searchedObj = nullptr;
	oldSearchedObj = nullptr;
	searchFlg = false;
	swapTimer = -1;
	oldSearchFlg = false;

	for (int i = 0; i < OBJECT_NUM; i++)
	{
		searchedObjAll[i] = nullptr;
		oldSearchedObjAll[i] = nullptr;
	}
	objNum = 0;
	objSelectNum[0] = 0;
	objSelectNum[1] = 0;
	oldStick[0] = 0.f;
	oldStick[1] = 0.f;
	oldStick[2] = 0.f;
	oldStick[3] = 0.f;
	for (int i = 0; i < 720 / 40; i++){
		for (int j = 0; j < 1280 / 40; j++){
			posRelation[i][j] = -1;
		}
	}
	posRelNum[0] = 0;
	posRelNum[1] = 0;

	objSelectNumTmp = 0;
	searchedObjFlg = false;
	effect_once = false;
	swap_once = false;
	damageFlg = false;
	damageOldFlg = false;
	hp = 5;
	state = 0;
	stateFlg = false;

	direction = -1;
	oldDirection = -1;
	oldObjNum = 0;

	for (int i = 0; i < 4; i++)
	{
		angle[i] = 0.f;
	}
	pState = idle;
	moveFrontFlg = true;
	animFlg = false;


	walk_se[0] = ResourceManager::SetSound("Resource/Sounds/Player/walk_normal.wav");
	walk_se[1] = ResourceManager::SetSound("Resource/Sounds/Player/walk_fire.wav");
	walk_se[2] = ResourceManager::SetSound("Resource/Sounds/Player/walk_grass.wav");
	walk_se[3] = ResourceManager::SetSound("Resource/Sounds/Player/walk_water.wav");
	jump_se = ResourceManager::SetSound("Resource/Sounds/Player/player_jump.wav");
	old_jump_se = jump_se;
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Player/damage_fire.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Player/damage_grass.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Player/damage_water.wav");
	now_riding = 0;
	draw_color = 0;
}

Player::~Player()
{

}

void Player::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;
	draw_color = color;

	object_pos = _object_pos;

	damageFlg = false;
}

void Player::Update(GameMain* _g)
{
	
	//意図しない変更が発生したか測定
	if (old_jump_se != jump_se)
	{
		printfDx("ジャンプSE変更");
		old_jump_se = jump_se;
	}
	fps = 0;
	//放置で大きくなりすぎるのを防止
	if (++frame > 6000)
	{
		frame = 0;
	}
	//移動エフェクト
	if (vector.x != 0 || vector.y != 0)
	{
		_g->SpawnEffect(location, erea, ShineEffect, 20, color);
	}
	//ステージ遷移時に座標だけ移動させる用（体力や色情報などはそのまま）
	if (_g->GetPlayerRespawnFlg())
	{
		location = _g->GetPlayerRespawnLocation();
		_g->SetPlayerRespawnFlg(false);
	}
	if (stageHitFlg[1][bottom] != true/* && !searchFlg*/) { //重力
		switch (state)
		{
		case 0:
			vector.y += 1.f;
			if (vector.y > 16.f) {
				vector.y = 16.f;
			}
			break;

		case 1:
			vector.y += 0.5f;
			if (vector.y > 8.f) {
				vector.y = 8.f;
			}
			break;

		default:
			break;
		}
		
		effect_once = false;
	}
	else {
		//一回だけエフェクトを出す
		if (effect_once == false)
		{
			_g->SpawnEffect(location, erea, LandingEffect, 15, color);
			effect_once = true;
		}
		vector.y = 0.f;
	}

	if (objNum <= 0) {
		searchedObjFlg = false;
	}
	else {
		searchedObjFlg = true;
	}

	oldSearchFlg = searchFlg;

	//交換後エフェクト用の硬直
	if (swapTimer >= 0)
	{
		if (swapTimer <= SWAP_EFFECT_STOP_TIME)
		{
			if (swap_once == false)
			{
				ChangePlayerColor();
				swap_once = true;
			}

		}
		//硬直が終わったら色を交換
		if (--swapTimer < 0)
		{
			searchFlg = false;
			swapTimer = -1;
			searchedObj = nullptr;
			swap_once = false;
		}

	}
	else
	{
		//Bボタンで色の交換ができるモードと切り替え
		if (PadInput::OnPressed(XINPUT_BUTTON_B)/* && searchedObjFlg*/) {
			SelectObject();
			searchFlg = true;
		}
		else if (PadInput::OnRelease(XINPUT_BUTTON_B) && searchFlg && searchedObj != nullptr && swapTimer < 0) {
			//交換エフェクトにかかる時間を受け取る
			swapTimer = _g->Swap(this, searchedObj);
			objSelectNumTmp = 0;
			for (int i = 0; i < OBJECT_NUM; i++)
			{
				oldSearchedObjAll[i] = nullptr;
			}
			//描画する色を白に
			draw_color = WHITE;
		}
		else if (PadInput::OnRelease(XINPUT_BUTTON_B) /*&& !searchedObjFlg*/ /*&& searchedObj == nullptr*/) {//交換できるオブジェクトが画面内になかった時
			searchFlg = false;
		}
		/*else if (PadInput::OnButton(XINPUT_BUTTON_B) && searchFlg) {
			searchFlg = false;
		}*/
		//Yボタンで色の交換
		/*if (searchedObj != nullptr) {
			objNum = 0;
		}*/
		MoveActor();
		if (swapTimer == -1)
		{
			MoveAim();
		}
	}

	if (searchFlg) {
		location.x += vector.x * 0.02f;
		location.y += vector.y * 0.02f;
	}
	else {
		location.x += vector.x;
		location.y += vector.y;
	}

	//交換中だけ対象のオブジェクトの見た目を白くする
	if (swapTimer > SWAP_EFFECT_STOP_TIME)
	{
		//選択中のオブジェクトを更新
		_g->SetNowCurrentObject(searchedObj);
	}
	else
	{
		_g->SetNowCurrentObject(nullptr);
	}
	//音声の周波数設定
	if (searchFlg == TRUE)
	{
		ResourceManager::SetSoundFreq(8000);
	}
	else
	{
		ResourceManager::SetSoundFreq(DEFAULT_FREQ);
	}

	PlayerSound();		//音声再生関連処理

	//damage
	if (damageFlg == true && !damageOldFlg) {
		hp--;
		damageEffectFlg = true;
	}
	if (damageEffectFlg) {
		damageEffectTime--;
		if (damageEffectTime < 0) {
			damageEffectFlg = false;
			damageEffectTime = 90;
			damageFlg = false;
			//エフェクトが終わった後に体力が0ならプレイヤーを削除
			if (hp <= 0)
			{
				_g->SetStage(_g->GetNowStage(), TRUE);
			}
		}
	}


	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	searchedLen = 1000.f;
	//searchedObj = nullptr;
	objNum = 0;
	for (int i = 0; i < 720 / 40; i++) {
		for (int j = 0; j < 1280 / 40; j++) {
			posRelation[i][j] = -1;
		}
	}
	posRelNum[0] = 0;
	posRelNum[1] = 0;

	damageOldFlg = damageFlg;
	damageFlg = false;

	stateFlg = false;

	mo++;
	now_riding = 0;

	ang += 1.f;

	
}

void Player::Draw()const
{
	//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE, 2.f);

	if (damageEffectFlg) {
		if (damageEffectTime % 10 == 0) {
			float ang = 0.f;
			//DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);
			DrawPlayer();
		
		}
	}
	else {//見た目
		//DrawBox(local_location.x, local_location.y, local_location.x + erea.width, local_location.y + erea.height, color, FALSE);
		
		DrawPlayer();
	
	}

	//DrawFormatString(local_location.x, local_location.y, 0xffff00, "hp : %d", hp);

	if (searchedObj != nullptr && searchFlg) {
		DrawCircleAA(searchedObj->GetLocalLocation().x + searchedObj->GetErea().width / 2,
			searchedObj->GetLocalLocation().y + searchedObj->GetErea().height / 2, 40, 40, 0xffff00, FALSE, 5);
	}
}

void Player::Finalize()
{

}

void Player::Hit(Object* _object)
{
	__super::Hit(_object);

	//ブロックと当たった時の処理
	if (
			(_object->GetObjectType() == BLOCK && _object->GetCanHit() == TRUE)||
			(_object->GetObjectType() == FIRE && _object->GetCanSwap() == TRUE && this->color == RED) ||
			(_object->GetObjectType() == WOOD && _object->GetCanSwap() == TRUE && this->color == GREEN)||
			(_object->GetObjectType() == WATER && _object->GetCanSwap() == TRUE && this->color == BLUE)
		)
	{
		Location tmpl = location;
		Erea tmpe = erea;
		move[0] = 0;
		move[1] = 0;
		move[2] = 0;
		move[3] = 0;

		//上下判定用に座標とエリアの調整
		location.x += 10.f;
		erea.height = 1.f;
		erea.width = tmpe.width - 20.f;

		//プレイヤー上方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//プレイヤー下方向の判定
		location.y += tmpe.height + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][bottom]) {
			stageHitFlg[0][bottom] = true;
			stageHitFlg[1][bottom] = true;
		}
		else {
			stageHitFlg[0][bottom] = false;
		}

		//戻す
		location.x = tmpl.x;
		location.y = tmpl.y;
		erea.height = tmpe.height;
		erea.width = tmpe.width;

		//上方向に埋まらないようにする
		if (stageHitFlg[0][top]) {//上方向に埋まっていたら
			float t = (_object->GetLocation().y + _object->GetErea().height) - location.y;
			if (t != 0) {
				vector.y = 0.f;
				move[top] = t;
			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			float t = _object->GetLocation().y - (location.y + erea.height);
			if (t != 0) {
				move[bottom] = t;
			}
		}


		location.y += move[top];
		location.y += move[bottom];


		//左右判定用に座標とエリアの調整
		//location.y += 20.f;
		erea.height = tmpe.height - 20.f;
		erea.width = 1;

		//プレイヤー左方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_object->GetLocation(), _object->GetErea());
		}
		else {
			stageHitFlg[0][left] = false;
		}


		//プレイヤー右方向の判定
		location.x = tmpl.x + tmpe.width + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right]) {
			stageHitFlg[0][right] = true;
			stageHitFlg[1][right] = true;
		}
		else {
			stageHitFlg[0][right] = false;
		}



		//最初の値に戻す

		location.x = tmpl.x;
		//location.y += -20.f;
		erea.height = tmpe.height;
		erea.width = tmpe.width;



		//左方向に埋まらないようにする
		if (stageHitFlg[0][left]) {//左方向に埋まっていたら
			float t = (_object->GetLocation().x + _object->GetErea().width) - location.x;
			if (t != 0) {
				vector.x = 0.f;
				move[left] = t;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.width);
			if (t != 0) {
				vector.x = 0.f;
				move[right] = t;
			}
		}


		//上下左右の移動量から移動後も埋まってるか調べる
		//左右移動させてみてまだ埋まってたら戻す
		location.x += move[left];
		location.x += move[right];
		if (location.x + erea.width < _object->GetLocation().x || location.x > _object->GetLocation().x + _object->GetErea().width) {
			if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
				location.x -= move[left];
				location.x -= move[right];
			}
		}

		erea.height = tmpe.height;
		erea.width = tmpe.width;

	}

	if (!damageEffectFlg && CheckCollision(_object->GetLocation(), _object->GetErea())) {
		//色ごとの判定
		switch (color)
		{
		case RED:
			if (_object->GetObjectType() == WATER || _object->GetColorData() == BLUE) {
				damageFlg = true;
			}
			
			break;

		case BLUE:
			if (_object->GetObjectType() == WOOD || _object->GetColorData() == GREEN) {
				damageFlg = true;
			}
			
			break;

		case GREEN:
			if (_object->GetObjectType() == FIRE || _object->GetColorData() == FIRE) {
				damageFlg = true;
			}
			
			break;

		default:
			break;
		}
	}

	//ダメージゾーンを上書きする
	if ((_object->GetObjectType() == WATER && _object->GetCanSwap() == FALSE && this->color == GREEN) ||
		(_object->GetObjectType() == FIRE && _object->GetCanSwap() == FALSE && this->color == BLUE) ||
		(_object->GetObjectType() == WOOD && _object->GetCanSwap() == FALSE && this->color == RED))
	{
		_object->SetColorData(color);
	}

	if ((_object->GetObjectType() == WATER && _object->GetCanSwap() == FALSE && this->color == BLUE && !stateFlg) ||
		(_object->GetObjectType() == FIRE && _object->GetCanSwap() == FALSE && this->color == RED && !stateFlg) ||
		(_object->GetObjectType() == WOOD && _object->GetCanSwap() == FALSE && this->color == GREEN && !stateFlg)){
		state = 1;
		stateFlg = true;
	}
	else {
		if (!stateFlg) {
			state = 0;
		}
	}

	//自分が乗っている(触れている)ブロックに応じてSEを変える
	if ((_object->GetObjectType() == FIRE && this->color != RED) || _object->GetObjectType() == WOOD || _object->GetObjectType() == WATER)
	{
		now_riding = _object->GetObjectType() - 2;
	}
	else if(_object->GetObjectType() == FIRE && this->color == RED)
	{
		now_riding = 0;
	}
}

void Player::MoveActor()
{
	//ジャンプ
	if ((PadInput::OnButton(XINPUT_BUTTON_A)/*|| PadInput::OnPressed(XINPUT_BUTTON_A)*/)) {
		switch (state)
		{
		case 0:
			if (stageHitFlg[1][bottom]) {
				vector.y = -23.f;
			}
			break;

		case 1:
			vector.y = -15.f;

			break;

		default:
			break;
		}
	
	}

	//左右移動
	float stick;
	if (!searchFlg) {
		//サーチ状態の時のベクトルを代入
		if (oldSearchFlg) {
			vector = saveVec;
		}

		if (PadInput::TipLeftLStick(STICKL_X) > 0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			//vector.x = stick * 7.5f; //こっちの移動方法を使うかも
			if (vector.x > 7.5f) {
				vector.x = 7.5f;
			}
			pState = moving;
		}
		else if (PadInput::TipLeftLStick(STICKL_X) < -0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			//vector.x = stick * 7.5f; //こっちの移動方法を使うかも
			if (vector.x < -7.5f) {
				vector.x = -7.5f;
			}
			pState = moving;
		}
		else {
			if (vector.x > 0.1f) {
				vector.x += -0.2f;
			}
			else if (vector.x < 0.1f) {
				vector.x += 0.2f;
			}

			pState = moving;

			if (vector.x > -0.4f && vector.x < 0.4f) {
				vector.x = 0.f;
				pState = idle;
			}

			//vector.x = 0.f;
		}
	}
	else {
		if (!oldSearchFlg) {
			saveVec = vector;
		}
		/*vector.x = 0.f;
		vector.y = 0.f;*/
	}

	if (stageHitFlg[1][bottom] != true) {
		pState = jump;
	}
}

void Player::MoveAim()
{
	
}

bool Player::SearchColor(Object* ob)
{
	if (ob != nullptr && ob->GetColorData() > 0 ){
		if(ob->GetLocalLocation().x >= 0 && ob->GetLocalLocation().x <= 1280 && ob->GetLocalLocation().y >= 0 && ob->GetLocalLocation().y <= 720) {
			searchedObjAll[objNum] = ob;
			if (objNum > 0 && searchedObjAll[objNum - 1]->GetLocalLocation().y + 40 <= ob->GetLocalLocation().y) {
				posRelNum[0]++;
				posRelNum[1] = 0;
			}
			int x = (int)ob->GetLocalLocation().x / 40;
			int y = (int)ob->GetLocalLocation().y / 40;
			posRelation[y][x] = objNum;
			objNum++;
		}
	}
	return false;
}

bool Player::ChangePlayerColor()
{
	ChangeColor(searchedObj);
	return false;
}

void Player::SelectObject()
{
	bool flg = false;//選択したかどうか
	if (swapTimer == -1 && searchedObjFlg && searchedObj != nullptr) {
		
		//X軸
		if ((PadInput::TipLeftLStick(STICKL_X) > 0.8f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT)) && oldStick[0]) {
			oldStick[0] = false;
			flg = true;
			float nearLen[4] = { 1000.f,1000.f,1000.f,1000.f };
			int snum[4] = { -1,-1,-1,-1 };
			oldDirection = direction;
			direction = right;
			/*
			* 優先順位（右）
			* 同じｘかつ上
			* 違うｘかつ上
			* 同じｘかつ下
			* 違うｘかつ下
			* 
			* このやり方無理かも
			* 配列に位置関係を保存してそこから求める
			* 今までのやり方じゃなくて座標を40で割った値を配列の添え字にして場所の関係事保存
			*/
			for (int i = 0;  i < objNum; i++)
			{
				if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) != 0) {

					//if (oldSearchedObj->GetLocalLocation().y >= searchedObj->GetLocalLocation().y) {//前回の選択オブジェクトより上にあったら

						if (searchedObj->GetLocalLocation().x <= searchedObjAll[i]->GetLocalLocation().x && //同じｘかつうえ
							searchedObj->GetLocalLocation().y >= searchedObjAll[i]->GetLocalLocation().y)
						{
							if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[0])
							{
								nearLen[0] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
								//セレクトオブジェクトここに入れる
								snum[0] = i;
							}
						}


						if (searchedObj->GetLocalLocation().x < searchedObjAll[i]->GetLocalLocation().x &&
							searchedObj->GetLocalLocation().y > searchedObjAll[i]->GetLocalLocation().y)//ちがうｘかつ上
						{
							if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[1])
							{
								nearLen[1] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
								//セレクトオブジェクトここに入れる
								//objSelectNumTmp = i;
								snum[1] = i;
							}
						}


						if (searchedObj->GetLocalLocation().x < searchedObjAll[i]->GetLocalLocation().x && //おなじｘかつした
							searchedObj->GetLocalLocation().y < searchedObjAll[i]->GetLocalLocation().y)
						{
							if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[2])
							{
								nearLen[2] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
								//セレクトオブジェクトここに入れる
								snum[2] = i;
							}
						}

						if (searchedObj->GetLocalLocation().x <= searchedObjAll[i]->GetLocalLocation().x && //ちがうｘかつした
							searchedObj->GetLocalLocation().y < searchedObjAll[i]->GetLocalLocation().y)
						{
							if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[3])
							{
								nearLen[3] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
								//セレクトオブジェクトここに入れる
								snum[3] = i;
							}
						}
					//}
				}
			}

			/*if (snum[0] > -1 && GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[1]]->GetLocalLocation()) >
				GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[0]]->GetLocalLocation())) {
				objSelectNumTmp = snum[0];
			}
			else if (snum[1] > -1 && GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[2]]->GetLocalLocation()) >
				GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[1]]->GetLocalLocation())) {
				objSelectNumTmp = snum[1];
			}*/

			if (oldDirection == direction) {
				for (int i = 0; i < OBJECT_NUM; i++)
				{
					if (oldSearchedObjAll[i] != nullptr) {
						if (snum[0] > -1) {
							if (GetLength(searchedObjAll[snum[0]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[0] = -1;
							}
						}
						if (snum[1] > -1) {
							if (GetLength(searchedObjAll[snum[1]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[1] = -1;
							}
						}
						if (snum[2] > -1) {
							if (GetLength(searchedObjAll[snum[2]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[2] = -1;
							}
						}
						if (snum[3] > -1) {
							if (GetLength(searchedObjAll[snum[3]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[3] = -1;
							}
						}
					}
				}
			}

			if (snum[0] > -1) {
				objSelectNumTmp = snum[0];
			}
			else if (snum[1] > -1) {
				objSelectNumTmp = snum[1];
			}
			else if (snum[3] > -1) {
				objSelectNumTmp = snum[3];
			}
			else if (snum[2] > -1) {
				objSelectNumTmp = snum[2];
			}


		}
		else if ((PadInput::TipLeftLStick(STICKL_X) < -0.8f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT)) && oldStick[1]) {
			oldStick[1] = false;
			flg = true;

			oldDirection = direction;
			direction = left;

			float nearLen[4] = { 1000.f,1000.f,1000.f,1000.f };
			int snum[4] = { -1,-1,-1,-1 };
			/*
			* 優先順位（左）
			* 同じｘかつ下
			* 違うｘかつ下
			* 同じｘかつ上
			* 違うｘかつ上
			*
			* このやり方無理かも
			* 配列に位置関係を保存してそこから求める
			* 今までのやり方じゃなくて座標を40で割った値を配列の添え字にして場所の関係事保存
			*/
			for (int i = 0; i < objNum; i++)
			{
				if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) != 0) {

					if (searchedObj->GetLocalLocation().x >= searchedObjAll[i]->GetLocalLocation().x && //同じｘかつうえ
						searchedObj->GetLocalLocation().y >= searchedObjAll[i]->GetLocalLocation().y)
					{
						if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[0])
						//if (fabs(searchedObj->GetLocalLocation().x - searchedObjAll[i]->GetLocalLocation().x) < nearLen[0])
						{
							nearLen[0] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
							//セレクトオブジェクトここに入れる
							snum[0] = i;
						}
					}


					if (searchedObj->GetLocalLocation().x > searchedObjAll[i]->GetLocalLocation().x &&
						searchedObj->GetLocalLocation().y > searchedObjAll[i]->GetLocalLocation().y)//ちがうｘかつ上
					{
						if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[1])
						//if (fabs(searchedObj->GetLocalLocation().x - searchedObjAll[i]->GetLocalLocation().x) < nearLen[1])
						{
							nearLen[1] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
							//nearLen[1] = fabs(searchedObj->GetLocalLocation().x - searchedObjAll[i]->GetLocalLocation().x);
							//セレクトオブジェクトここに入れる
							//objSelectNumTmp = i;
							snum[1] = i;
						}
					}


					if (searchedObj->GetLocalLocation().x > searchedObjAll[i]->GetLocalLocation().x && //おなじｘかつした
						searchedObj->GetLocalLocation().y < searchedObjAll[i]->GetLocalLocation().y)
					{
						if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[2])
						//if (fabs(searchedObj->GetLocalLocation().x - searchedObjAll[i]->GetLocalLocation().x) < nearLen[2])
						{
							nearLen[2] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
							//セレクトオブジェクトここに入れる
							snum[2] = i;
						}
					}

					if (searchedObj->GetLocalLocation().x >= searchedObjAll[i]->GetLocalLocation().x && //ちがうｘかつした
						searchedObj->GetLocalLocation().y <= searchedObjAll[i]->GetLocalLocation().y)
					{
						if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) <= nearLen[3])
						//if (fabs(searchedObj->GetLocalLocation().x - searchedObjAll[i]->GetLocalLocation().x) < nearLen[3])
						{
							nearLen[3] = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
							//セレクトオブジェクトここに入れる
							snum[3] = i;
						}
					}
				}
			}

			/*if (snum[0] > -1 && GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[1]]->GetLocalLocation()) >
				GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[0]]->GetLocalLocation())) {
				objSelectNumTmp = snum[0];
			}
			else if (snum[1] > -1 && GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[2]]->GetLocalLocation()) >
				GetLength(searchedObj->GetLocalLocation(), searchedObjAll[snum[1]]->GetLocalLocation())) {
				objSelectNumTmp = snum[1];
			}*/

			if (oldDirection == direction) {
				for (int i = 0; i < OBJECT_NUM; i++)
				{
					if (oldSearchedObjAll[i] != nullptr) {
						if (snum[0] > -1) {
							if (GetLength(searchedObjAll[snum[0]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[0] = -1;
							}
						}
						if (snum[1] > -1) {
							if (GetLength(searchedObjAll[snum[1]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[1] = -1;
							}
						}
						if (snum[2] > -1) {
							if (GetLength(searchedObjAll[snum[2]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[2] = -1;
							}
						}
						if (snum[3] > -1) {
							if (GetLength(searchedObjAll[snum[3]]->GetLocalLocation(), oldSearchedObjAll[i]->GetLocalLocation()) == 0) {
								snum[3] = -1;
							}
						}
					}
				}
			}
			

			if (snum[0] > -1) {
				objSelectNumTmp = snum[0];
			}
			else if (snum[1] > -1) {
				objSelectNumTmp = snum[1];
			}
			else if (snum[3] > -1) {
				objSelectNumTmp = snum[3];
			}
			else if (snum[2] > -1) {
				objSelectNumTmp = snum[2];
			}

			//if (snum[3] > -1) {
			//	objSelectNumTmp = snum[3];
			//}
			//else if (snum[2] > -1) {
			//	objSelectNumTmp = snum[2];
			//}
			//else if (snum[0] > -1) {
			//	objSelectNumTmp = snum[0];
			//}
			//else if (snum[1] > -1) {
			//	objSelectNumTmp = snum[1];
			//}

		}
		else if (PadInput::TipLeftLStick(STICKL_X) < 0.1f && PadInput::TipLeftLStick(STICKL_X) > -0.1f) {
			oldStick[0] = true;
			oldStick[1] = true;
		}
		//Y軸
		if ((PadInput::TipLeftLStick(STICKL_Y) > 0.8f || PadInput::OnButton(XINPUT_BUTTON_DPAD_UP)) && oldStick[2]) {
			oldStick[2] = false;
			flg = true;

			oldDirection = direction;
			direction = top;

			float nearLen = 1000.f;
			for (int i = 0; i < objNum; i++)
			{
				if (searchedObj->GetLocalLocation().y > searchedObjAll[i]->GetLocalLocation().y)
				{
					if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) < nearLen)
					{
						nearLen = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
						//セレクトオブジェクトここに入れる
						objSelectNumTmp = i;
					}
				}
			}
		}
		else if ((PadInput::TipLeftLStick(STICKL_Y) < -0.8f || PadInput::OnButton(XINPUT_BUTTON_DPAD_DOWN)) && oldStick[3]) {
			oldStick[3] = false;
			flg = true;

			oldDirection = direction;
			direction = bottom;


			float nearLen = 1000.f;
			for (int i = 0; i < objNum; i++)
			{
				if (searchedObj->GetLocalLocation().y < searchedObjAll[i]->GetLocalLocation().y)
				{
					if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) < nearLen)
					{
						nearLen = GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation());
						//セレクトオブジェクトここに入れる
						objSelectNumTmp = i;
					}
				}
			}
		}
		else if (PadInput::TipLeftLStick(STICKL_Y) < 0.1f && PadInput::TipLeftLStick(STICKL_Y) > -0.1f) {
			oldStick[2] = true;
			oldStick[3] = true;
		}



		if (oldDirection != direction) {//前回の移動方向と違うならリセ
			for (int i = 0; i < OBJECT_NUM; i++)
			{
				oldSearchedObjAll[i] = nullptr;
			}
			oldObjNum = 0;
			oldSearchedObjAll[oldObjNum++] = oldSearchedObj;
		}

		if (flg) {
			oldSearchedObj = searchedObj;
			oldSearchedObjAll[oldObjNum++] = oldSearchedObj;
		}

		searchedObj = searchedObjAll[objSelectNumTmp];

		
	}
	else if (searchedObjFlg) {//最初
		float len = 1000;
		int n = 0;
		for (int i = 0; i < objNum; i++)
		{
			if (GetLength(local_location, searchedObjAll[i]->GetLocalLocation()) < len) {
				len = GetLength(local_location, searchedObjAll[i]->GetLocalLocation());
				n = i;
			}
		}
		objSelectNumTmp = n;
		searchedObj = searchedObjAll[objSelectNumTmp];
		oldSearchedObj = searchedObj;
		oldSearchedObjAll[0] = oldSearchedObj;
		direction = -1;
		oldObjNum = 0;
	}
	else {
		searchedObj = nullptr;
	}
}

//bool Player::CheckCollision(Location l, Erea e)
//{
//	bool ret = false;
//
//	//自分の左上座標
//	float my_x = location.x;
//	float my_y = location.y;
//	//自分の中央座標
//	float my_cx = my_x + (erea.width / 2);
//	float my_cy = my_y + (erea.height / 2);
//	//自分の幅と高さの半分
//	float my_harf_width = erea.width / 2;
//	float my_harf_height = erea.height / 2;
//
//	//相手の左上座標
//	float sub_x = l.x;
//	float sub_y = l.y;
//	//相手の中央座標
//	float sub_cx = sub_x + (e.width / 2);
//	float sub_cy = sub_y + (e.height / 2);
//	//相手の幅と高さの半分
//	float sub_harf_width = e.width / 2;
//	float sub_harf_height = e.height / 2;
//
//	//自分と相手の中心座標の差
//	float diff_x = my_cx - sub_cx;
//	float diff_y = my_cy - sub_cy;
//
//	//当たり判定の演算
//	if (fabsf(diff_x) < my_harf_width + sub_harf_width &&
//		fabsf(diff_y) < my_harf_height + sub_harf_height)
//	{
//		ret = true;
//	}
//	return ret;
//}

float Player::ThreePointAngle(Location l1, Location l2, Location referenceP)const
{
	if (l1.x == l2.x && l1.y == l2.y)return 0.f;

	Location v1, v2;
	v1.x = l1.x - referenceP.x;
	v1.y = l1.y - referenceP.y;
	v2.x = l2.x - referenceP.x;
	v2.y = l2.y - referenceP.y;
	//DrawLine(referenceP.x, referenceP.y, referenceP.x + v1.x, referenceP.y + v1.y, 0x00ff00);
	DrawLineAA(referenceP.x, referenceP.y, referenceP.x + v2.x, referenceP.y + v2.y, 0x00ffff);

	float len1, len2;
	//len1 = sqrtf(powf(v1.x, 2) + powf(v1.y, 2));
	//len2 = sqrtf(powf(v2.x, 2) + powf(v2.y, 2));
	len1 = powf((v1.x * v1.x) + (v1.y + v1.y), 0.5);
	len2 = powf((v2.x * v2.x) + (v2.y + v2.y), 0.5);
	/*len1 = sqrtf((v1.x * v1.x) + (v1.y * v1.y));
	len2 = sqrtf((v2.x * v2.x) + (v2.y * v2.y));*/

	/*Location uv1, uv2 = { 0,0 };
	uv1.x = v1.x / len1;
	uv1.y = v1.y / len1;
	uv2.x = v2.x / len2;
	uv2.y = v2.y / len2;*/

	/*float dot;
	dot = (uv1.x * uv2.x) + (uv1.y + uv2.y);*/

	float dot;
	dot = (v1.x * v2.x) + (v1.y + v2.y);

	/*float len1, len2;
	len1 = sqrtf(powf(v1.x, 2) + powf(v1.y, 2));
	len2 = sqrtf(powf(v2.x, 2) + powf(v2.y, 2));*/

	float cos;
	cos = dot / (len1 * len2);

	float sita;
	sita = acosf(cos) * (180.f / 3.14f);

	return sita;
}

float Player::GetLength(Location l1, Location l2) 
{
	float len;
	float x, y;

	x = l2.x - l1.x;
	y = l2.y - l1.y;

	len = sqrtf(powf(x, 2) + powf(y, 2));

	return len;
}

void Player::PlayerSound()
{
	//スローモーション中は10フレームに一回だけ音を鳴らす
	if (searchFlg == FALSE || (searchFlg == TRUE && frame % 10 == 0))
	{
		//プレイヤー歩行
		if (stageHitFlg[1][bottom] == true && vector.x != 0 && frame % 15 == 0)
		{
			ResourceManager::StartSound(walk_se[now_riding]);
		}
		//ジャンプ
		if (PadInput::OnButton(XINPUT_BUTTON_A) == true && ((state == 0 && stageHitFlg[1][bottom]) || state == 1))
		{
			ResourceManager::StartSound(jump_se);
		}

		//ダメージ
		if (damageFlg && !damageOldFlg && now_riding > 0) {
			ResourceManager::StartSound(damage_se[now_riding-1]);
		}
	}
}

void Player::PlayerAnim()
{
	switch (pState)
	{
	case idle:
		angle[0] = 20.f;
		angle[1] = -20.f;
		angle[2] = 20.f;
		angle[3] = -20.f;
		break;

	case moving:
		float speed;
		speed = abs(vector.x) * 0.1f;
		if (animFlg) {
			angle[0] += speed;
			angle[1] -= speed;
			angle[2] += speed;
			angle[3] -= speed;

			if (angle[0] > 20.f) {
				animFlg = false;
			}
		}
		else {
			angle[0] -= speed;
			angle[1] += speed;
			angle[2] -= speed;
			angle[3] += speed;

			if (angle[0] < -20.f) {
				animFlg = true;
			}
		}
		break;

	case jump:
		angle[0] = 20.f;
		angle[1] = -20.f;
		angle[2] = 0.f;
		angle[3] = 0.f;
		break;

	default:
		break;
	}
}

void Player::DrawPlayer() const
{
	if (moveFrontFlg) {
		if (hp > 0) {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 76, 23, 15, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) - 6, local_location.y - (erea.height) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 62, 10, 5, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, false);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 37, 21, 37, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, false);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 5, 23, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, false);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
		}
		else {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, true);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) - 6, local_location.y - (erea.height) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, true);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, true);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
		}
		//腕
		if (hp > 4) {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], 0x000000, true);


		}
		if (hp > 3) {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], 0x000000, true);
		}


		//足 前から
		if (hp > 2) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, false);

		}
		else {

			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, true);

		}
		if (hp > 1) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, true);
		}

		if (hp > 1) {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, false);

		}
		else {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0xffffff, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0xffffff, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0xffffff, false);

		}
	}
	else {
		if (hp > 0) {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 76, 23, 15, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 6, local_location.y - (erea.height) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 62, 10, 5, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, false);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 37, 21, 37, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, false);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) - 15, local_location.y - (erea.height) + 40, 5, 23, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) - 15, local_location.y - (erea.height) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, false);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) - 15, local_location.y - (erea.height) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
		}
		else {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, true);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) - 6, local_location.y - (erea.height) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, true);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2), local_location.y - (erea.height) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, true);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.width / 2) + 15, local_location.y - (erea.height) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
		}
		//腕
		if (hp > 4) {
			ResourceManager::DrawRotaBox(local_location.x + 15 , local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 15 , local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, 0x000000, false);
			DrawCircle(local_location.x + 25, local_location.y + 50, 3, TRUE);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, 0x000000, true);


		}
		if (hp > 3) {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, 0x000000, true);
		}


		//足 前から
		if (hp > 2) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, false);

		}
		else {

			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, true);

		}
		if (hp > 1) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, true);
		}

		if (hp > 1) {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, false);

		}
		else {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0xffffff, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0xffffff, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.width / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0xffffff, false);

		}

	}
}


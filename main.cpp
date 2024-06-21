#include "DxLib.h"
#include "Scene/SceneManager.h"
#include "Utility/FpsController.h"
#include "Utility/PadInput.h"
#include "Utility/KeyInput.h"
#include "Utility/ResourceManager.h"
#include "Utility/common.h"
#include "Scene/Title.h"
#include "Scene/Help.h"

#include "Scene/GameMain.h"

#define FRAMERATE 60.0 //フレームレート

//メインプログラム 開始

int WINAPI WinMain(_In_ HINSTANCE  hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow) {

    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    SetMainWindowText("下積み");         // ウィンドウタイトルを設定

    ChangeWindowMode(TRUE);                        // ウインドウモードで起動

    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // ウインドウのサイズ

    if (DxLib_Init() == -1) return -1;             // DXライブラリの初期化処理

    SetDrawScreen(DX_SCREEN_BACK);                 // 描画先画面を裏にする（ダブルバッファリング）

    //シーンオブジェクト作成
    SceneManager* sceneMng = new SceneManager((AbstractScene*) new GameMain(0));

    //シーンオブジェクト初期化
    sceneMng->Initialize();

    //fps制御
    FpsController* FPSC= new FpsController(FRAMERATE, 800);

    //ステージアニメーション初期化
    ResourceManager::StageAnimInitialize();

    //フォント変更 候補:"Franklin Gothic" "HGS創英角ｺﾞｼｯｸUB" "HGｺﾞｼｯｸE" ステージ後半で変更"Wingdings"
    ChangeFont("Franklin Gothic");

    try
    {
        // ゲームループし、シーンマネジャーでシーンの更新
        while ((ProcessMessage() == 0) && (sceneMng->Update() != nullptr))
        {

            ClearDrawScreen();      // 画面の初期化

            PadInput::UpdateKey();  //パッドの入力処理
            KeyInput::UpdateKey();  //キーボードの入力処理
            ResourceManager::StageAnimUpdate();     //ステージアニメーションの更新

            // シーンマネジャーでシーンの描画開始
            sceneMng->Draw();

            FPSC->All();
#ifdef _DEBUG
            FPSC->Disp();
#endif

            //強制終了
            //Escapeキーまたはバックボタンを押したら強制終了
            if (PadInput::OnButton(XINPUT_BUTTON_BACK) || (KeyInput::OnKey(KEY_INPUT_ESCAPE))) {
                break;
            }
            ScreenFlip(); // 裏画面の内容を表画面に反映する

            // 画面の文字達を消す
            clsDx();
        };
    }
    catch (const char* err_log)
    {
        OutputDebugString(err_log);

        return -1;
    }

    //シーンオブジェクト終了処理
    sceneMng->Finalize();

    ResourceManager::DeleteResource(); //音声データの削除
    DxLib_End(); // DXライブラリ使用の終了処理
    return 0;    // プログラムの終了
};
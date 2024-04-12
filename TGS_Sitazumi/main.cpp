#include "DxLib.h"
#include "Scene/SceneManager.h"
#include "Utility/FpsController.h"
#include "Utility/PadInput.h"
#include "Utility/KeyInput.h"
#include "Utility/ResourceManager.h"
#include "Utility/common.h"
#include "Scene/Title.h"

#include "Scene/GameMain.h"

#define FRAMERATE 60.0 //�t���[�����[�g
//���C���v���O���� �J�n

int WINAPI WinMain(_In_ HINSTANCE  hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow) {
    SetMainWindowText("�Q�[���^�C�g��");         // �E�B���h�E�^�C�g����ݒ�

    ChangeWindowMode(TRUE);                        // �E�C���h�E���[�h�ŋN��

    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // �E�C���h�E�̃T�C�Y

    if (DxLib_Init() == -1) return -1;             // DX���C�u�����̏���������

    SetDrawScreen(DX_SCREEN_BACK);                 // �`����ʂ𗠂ɂ���i�_�u���o�b�t�@�����O�j

    //�f�[�^�ǂݍ���
    ResourceManager::LoadResource();

    // �^�C�g�� �V�[���I�u�W�F�N�g�쐬
    SceneManager* sceneMng = new SceneManager((AbstractScene*) new GameMain(0));

    //fps����
    FpsController* FPSC= new FpsController(FRAMERATE, 800);

    // �Q�[�����[�v���A�V�[���}�l�W���[�ŃV�[���̍X�V
    while ((ProcessMessage() == 0) && (sceneMng->Update() != nullptr))
    {
        ClearDrawScreen(); // ��ʂ̏�����

        PadInput::UpdateKey();//�p�b�h�̓��͏���
        KeyInput::UpdateKey();//�L�[�{�[�h�̓��͏���

        // �V�[���}�l�W���[�ŃV�[���̕`��J�n
        sceneMng->Draw();

        FPSC->All();
#ifdef _DEBUG
        FPSC->Disp();
#endif

        //�����I��
        //Escape�L�[�܂��̓o�b�N�{�^����������狭���I��
        if(PadInput::OnButton(XINPUT_BUTTON_BACK) || (KeyInput::OnKey(KEY_INPUT_ESCAPE))) {  
            break;
        }
        ScreenFlip(); // ����ʂ̓�e��\��ʂɔ��f����

    };

    ResourceManager::DeleteResource(); //�����f�[�^�̍폜
    DxLib_End(); // DX���C�u�����g�p�̏I������
    return 0;    // �v���O�����̏I��
};
#pragma once
#include "./Interface/ctabdialog.h"

#define MAX_FIGHT_CLASS_BTN	19
#define MAX_ROGUE_CLASS_BTN	18
#define MAX_MAGE_CLASS_BTN	18
#define MAX_DEVIL_CLASS_BTN 5


// ���� ���̵� Ʃ�丮�� ��ȣ
#define GLADECLASS_TUTORIAL_NO	20

class cPushupButton;
class cStatic;
class cButton;

// ���� Ʈ�� ���̵� Ŭ����
// 100302 ONS ���� Ʈ�� ���̵� ���̾�α׸� ������ ����, ���� ���� �߰�
class CGradeClassDlg : public cTabDialog
{
	// ���� ������ư
	cButton*		m_pStaticGradeBtn[6];

	// ������ ��ư�׷�
    cPushupButton*		m_pFighterClassBtn[MAX_FIGHT_CLASS_BTN];
	cPushupButton*		m_pRogueClassBtn[MAX_ROGUE_CLASS_BTN];
	cPushupButton*		m_pMageClassBtn[MAX_MAGE_CLASS_BTN];
	cPushupButton*		m_pDevilClassBtn[MAX_DEVIL_CLASS_BTN];

public:
	CGradeClassDlg();
	virtual ~CGradeClassDlg();

	void Linking();
	void Init();

	virtual void Add( cWindow* );
	virtual void Render();
	virtual void SetActive( BOOL val );
private:
	void SetClassTree(cPushupButton** pPushBtn, ENUM_CLASS eClass);
};
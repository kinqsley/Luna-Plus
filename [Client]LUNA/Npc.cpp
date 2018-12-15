// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Npc.h"
#include "GameResourceManager.h"
#include "MOTIONDESC.h"		// 080321 NYJ --- NPC��� enum�� ���� include
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{
	m_pSInfo = NULL;
	m_dwLastMotionTime = 0;	// 080321 NYJ --- IDLE��Ǻ���
	m_nNextMotionTime = 5000;

	m_bReadyToMove = FALSE;		// 090227 ShinJS --- �̵��� �غ� �ʱ�ȭ

	m_bUseNoticeDlg = FALSE;	// 091006 ShinJS --- NPC Notice Dialog ��뿩�� �ʱ�ȭ
}

CNpc::~CNpc()
{

}


void CNpc::InitNpc(NPC_TOTALINFO* pTotalInfo)
{
	memcpy(&m_NpcInfo,pTotalInfo,sizeof(NPC_TOTALINFO));
	memset(m_QuestList, 0, 4*MAX_NPCQUEST_LIST);
	m_QuestCount = 0;
	m_pSInfo = GAMERESRCMNGR->GetNpcInfo(GetNpcKind());

	m_nNextMotionTime = 20000 + rand()%30000;
	m_dwLastMotionTime = gCurTime;	// 080321 NYJ --- IDLE��Ǻ���

	m_bReadyToMove = FALSE;		// 090227 ShinJS --- �̵��� �غ� �ʱ�ȭ

	m_bUseNoticeDlg = FALSE;	// 091006 ShinJS --- NPC Notice Dialog ��뿩�� �ʱ�ȭ
}



void CNpc::Process()
{
	if(IsDied())
	{
		DWORD time;
		time = REMOVEFADE_START_TIME;

		if( gCurTime - m_DiedTime > time )
		{
			ShowObjectName( FALSE );
			ShowChatBalloon( FALSE, NULL );
			//�׸��ڵ� ��������.
			if(m_ShadowObj.IsInited())
			{
				m_ShadowObj.Release();
			}

			DWORD ToTime = time;
			DWORD ElTime = gCurTime - m_DiedTime - time;
			float alp = 1 - (ElTime / (float)ToTime);
			if(alp <= 0)
				alp = 0;
			GetEngineObject()->SetAlpha(alp);
		}
#ifdef _TESTCLIENT_
		if(gCurTime - m_DiedTime > MONSTERREMOVE_TIME)
		{
			OBJECTMGR->AddGarbageObject(this);
		}
#endif
	}

	CObject::Process();

	const DWORD dwLatencyTime = 60000;	// 60sec
	const DWORD dwTimeRange = 15000;	// 15sec
	
	// 080321 NYJ --- IDLE��Ǻ����� �����ð�(dwLatencyTime+dwTimeRange)�� �ѹ��� �ٲ۴�.
	if(DWORD(m_nNextMotionTime) < gCurTime && m_dwLastMotionTime <= gCurTime - DWORD(m_nNextMotionTime) && GetEngineObject()->IsEndMotion())
	{
		SetMotionInState(eObjectState_None);
		m_dwLastMotionTime = gCurTime;
		m_nNextMotionTime = dwLatencyTime + rand()%dwTimeRange;
	}
}

void CNpc::SetMotionInState(EObjectState State)
{
	switch(State)
	{
	case eObjectState_Move:
		{
			ASSERT(0);
		}
		break;
	case eObjectState_None:
		{
			const int nStand2Motion = 2;	// 080321 NYJ --- IDLE2 ����� �ϵ��ڵ����� �ִ´�.
			m_ObjectState.State_Start_Motion = 1;
			m_ObjectState.State_Ing_Motion = 1;
			m_ObjectState.State_End_Motion = 1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;

			ChangeMotion(nStand2Motion,FALSE);
		}
		break;
	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;
		}
		break;
	}
}

void CNpc::OnClicked()
{
	ChangeBaseMotion(1);
	ChangeMotion(5,FALSE);
}


void CNpc::AddQuestIndex(int Idx)
{
	if(Idx <= 0)		return;

	m_QuestList[m_QuestCount] = Idx;
	++m_QuestCount;
}

void CNpc::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	if(GetNpcJob() == BOMUL_ROLE)
	{
		ChangeBaseMotion(1);
		ChangeMotion(2,FALSE);
	}
}
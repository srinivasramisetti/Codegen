#if !defined(AFX_MODELPROCESSING_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_)
#define AFX_MODELPROCESSING_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_

struct GetNestedDefaultStateReturnStatus_t
{
	int NextStateNumber;
	CString Status;
};

CString ProcessModelFile(void);
int ConvertStateTextToNumber(CString StateTextToConvert);
CString ConvertStateNumberToText(int StateNumberToConvert);
int ConvertCondCStringToBit(CString CondCString);
CString ProcessTransitions(int CurrentState, int SuperStateFlag);

int StoreSuperStateTrans(int SuperStateExtendedListStateNumber, CString StateName, int AdditionalCondMask, int AdditionalDontCareMask);
void ProcessSuperStateTrans(void);
CString ProcessStateSuperStatesTargets(void);
CString TransMutualExclusionCheck(void);
CString ProcessFunctionBlock(CString EndCompareCString1, CString EndCompareCString2,
                             CString EndCompareCString3, CString EndCompareCString4);
CString CheckForTransitionErrors(void);
void UpdateTransWithAddlCondDontCareMasks(void);
void AddTransFromSuperStates(void);

CString CheckPowerupTransitionTarget(CString PassedStateSuperStatesTargetsStatus);
GetNestedDefaultStateReturnStatus_t GetNestedDefaultState(int SuperStateNumber);

#define TRANS_ERROR_MASK 0xFFFFFF00

#define CONVERT_ERROR 0xFFFFFFFF

#define INIT                    0
#define GETTING_DONT_CARE_BIT   1
#define GETTING_COND_BIT        2
#define WAITING_FOR_END_BRACKET	3

#define UNKNOWN_DONT_CARE_MASK      0x00000001
#define UNKNOWN_COND_MASK           0x00000002
#define UNKNOWN_TARGET_STATE_MASK   0x00000004
#define ILLEGAL_LOGIC_MASK          0x00000008
#define TRANS_ACTION_PRESENT_MASK   0x00000010

#define SUPER_STATE_OFFSET 65536

#endif /* AFX_MODELPROCESSING_H__A85A5564_DCDD_11D3_8625_D268D3FDEB93__INCLUDED_ */

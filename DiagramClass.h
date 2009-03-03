// Diagram class header file

#if !defined(AFX_MODEL_H__A85A5564_DCDD_4567_8625_D268D3FDEB93__INCLUDED_)
#define AFX_MODEL_H__A85A5564_DCDD_4567_8625_D268D3FDEB93__INCLUDED_

#include "CommonStateClass.h"
#include "StateClass.h"
#include "SuperStateClass.h"

//*********************************************************
struct CondBit_t
{
    CString CondBitText;
    int CondBit;
    CondBit_t *NextElementPtr;
};

struct DontCareBit_t
{
    CString DontCareBitText;
    int DontCareBit;
    DontCareBit_t *NextElementPtr;
};

struct StateNumberToText_t
{
    CString Text;
    StateNumberToText_t *NextElementPtr;
};

struct SuperStateAdditionalMask_t
{
    int Mask;
    SuperStateAdditionalMask_t *NextElementPtr;
};

//*********************************************************
class DiagramClass
{
public:
// BEG NEW FUNCTIONS
    // Diagram class functions
    DiagramClass(void); // Constructor
    void AddPowerupTransAction(CString NewPowerupTransAction);
    CString GetPowerupTransAction(void);

    void AddInitialStateName(CString NewInitialStateName);
    CString GetInitialStateName(void);

    void AddCondBit(CondBit_t *CondBitPtr);
    int GetNumCondBits(void);
    int GetCondBit(int CondBitNumber);
    CString GetCondBitText(int CondBitNumber);

    void AddDontCareBit(DontCareBit_t *DontCareBitPtr);
    int GetNumDontCareBits(void);
    int GetDontCareBit(int DontCareBitNumber);
    CString GetDontCareBitText(int DontCareBitNumber);

    // State class functions
    void AddState(State_t *StatePtr);
    CString GetEntryAction(int StateNumber);
    CString GetActivityFunction(int StateNumber);
    CString GetExitAction(int StateNumber);

    // Super state class functions
    void AddSuperState(SuperState_t *SuperStatePtr);
    void AddDefaultTransTargetState(int SuperStateNumber, CString NewDefaultTransTargetState);
    CString GetDefaultTransTargetState(int SuperStateNumber);

    // State and super state class functions
    int GetNumStates(int SuperStateFlag);
    CString GetStateName(int SuperStateFlag, int StateNumber);
    void UpdateTransCondDontCareMasks(int SuperStateFlag, int StateNumber, int TransNumber, int NewCondMask, int NewDontCareMask);

    void AddTrans(int SuperStateFlag, int StateNumber, Trans_t *TransPtr);
    int GetNumTrans(int SuperStateFlag, int StateNumber);
    CString GetTransActionFunctionName(int SuperStateFlag, int StateNumber, int TransNumber);
    CString GetTransActionFunctionBody(int SuperStateFlag, int StateNumber, int TransNumber);
    int GetTransTargetState(int SuperStateFlag, int StateNumber, int TransNumber);
    int GetTransCondMask(int SuperStateFlag, int StateNumber, int TransNumber);
    int GetTransDontCareMask(int SuperStateFlag, int StateNumber, int TransNumber);
    CString GetTransAddedBySuperState(int StateNumber, int TransNumber);
    CString GetTransStatus(int SuperStateFlag, int StateNumber, int TransNumber);

    void UpdateTargetState(int SuperStateFlag, int StateNumber, int TransNumber, int UpdatedNextState);

    void AddSuperStateListSubState(int SuperStateNumber, ListSubState_t *NewListSubStateData);
    int GetSuperStateListNumSubStates(int SuperStateNumber);
    CString GetSuperStateListSubStateName(int SuperStateNumber, int SubStateNumber);
    void RemoveLastSuperStateListSubStateName(int SuperStateNumber);
    
    void AddSuperStateExtendedListSubState(int SuperStateNumber, ListSubState_t *NewListSubStateData);
    int GetSuperStateExtendedListNumSubStates(int SuperStateNumber);
    CString GetSuperStateExtendedListSubStateName(int SuperStateNumber, int SubStateNumber);
    
    void AddStateNumberToTextElement(CString ElementText);
    int GetNumStatesInConfigFile(void);
    CString GetStateNumberToTextElement(int ElementNumber);

    void AddSuperStateNumberToTextElement(CString ElementText);
    int GetNumSuperStatesInConfigFile(void);
    CString GetSuperStateNumberToTextElement(int ElementNumber);
    
    void AddSuperStateAdditionalCondMask(int Mask);
    int GetNumSuperStateAdditionalCondMasks(void);
    int GetSuperStateAdditionalCondMask(int ElementNumber);
    
    void AddSuperStateAdditionalDontCareMask(int Mask);
    int GetNumSuperStateAdditionalDontCareMasks(void);
    int GetSuperStateAdditionalDontCareMask(int ElementNumber);

    void IncNumTransActions(void);
    int GetNumTransActions(void);
private:
    Trans_t *FindTransElementPtr(int TransNumber);

    int NumStatesInConfigFile;
    int NumStateNumberToTextElements;
    StateNumberToText_t *HeadStateNumberToTextArrayPtr;
    StateNumberToText_t *TailStateNumberToTextArrayPtr;

    int NumSuperStatesInConfigFile;
    int NumSuperStateNumberToTextElements;
    StateNumberToText_t *HeadSuperStateNumberToTextArrayPtr;
    StateNumberToText_t *TailSuperStateNumberToTextArrayPtr;
    
    int NumSuperStateAdditionalCondMasks;
    SuperStateAdditionalMask_t *HeadSuperStateAdditionalCondMaskArrayPtr;
    SuperStateAdditionalMask_t *TailSuperStateAdditionalCondMaskArrayPtr;

    int NumSuperStateAdditionalDontCareMasks;
    SuperStateAdditionalMask_t *HeadSuperStateAdditionalDontCareMaskArrayPtr;
    SuperStateAdditionalMask_t *TailSuperStateAdditionalDontCareMaskArrayPtr;
    
    CString PowerupTransAction;
    CString InitialStateName;
    // The following item supports making sure the transition
    // action functions have unique names and support the transition
    // mutual exclusion checking
    int NumTransActions;

    int NumCondBits;
    CondBit_t *HeadCondBitPtr;
    CondBit_t *TailCondBitPtr;
    CondBit_t *FindCondBitElementPtr(int CondBitNumber);

    int NumDontCareBits;
    DontCareBit_t *HeadDontCareBitPtr;
    DontCareBit_t *TailDontCareBitPtr;
    DontCareBit_t *FindDontCareBitElementPtr(int DontCareBitNumber);

    int NumStates;
    StateClass *HeadStatePtr;
    StateClass *TailStatePtr;
    StateClass *FindStateElementPtr(int StateNumber);

    int NumSuperStates;
    SuperStateClass *HeadSuperStatePtr;
    SuperStateClass *TailSuperStatePtr;
    SuperStateClass *FindSuperStateElementPtr(int SuperStateNumber);
};

#endif //AFX_MODEL_H__A85A5564_DCDD_4567_8625_D268D3FDEB93__INCLUDED_

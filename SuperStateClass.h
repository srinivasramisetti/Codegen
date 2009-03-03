// Super state class header file

#if !defined(AFX_MODEL_H__A85A5564_DCDD_3456_8625_D268D3FDEB93__INCLUDED_)
#define AFX_MODEL_H__A85A5564_DCDD_3456_8625_D268D3FDEB93__INCLUDED_


//*********************************************************
struct SuperState_t
{
    int SuperStateNumber;
    CString SuperStateName;
    CString DefaultTransTargetState;
};

struct ListSubState_t
{
    CString SubStateName;
    ListSubState_t *LastElementPtr;
    ListSubState_t *NextElementPtr;
};

//*********************************************************
class SuperStateClass: public CommonStateClass
{
public:
    SuperStateClass(SuperState_t *SuperStateData); // Constructor

    void AddDefaultTransTargetState(CString NewDefaultTransTargetState);
    CString GetDefaultTransTargetState(void);
    int GetSuperStateNumber(void);
    CString GetSuperStateName(void);
    void SetNextElementPtr(SuperStateClass *NewNextElementPtr);
    SuperStateClass *GetNextElementPtr(void);

    void AddSuperStateListSubState(ListSubState_t *NewListSubStateData);
    int GetNumSuperStateListSubStates(void);
    CString GetSuperStateListSubStateName(int SubStateNumber);
    void RemoveLastSuperStateListSubStateName(void);
    void SetHeadSubStateListPtr(ListSubState_t *NewHeadSubStateListPtr);
    ListSubState_t *GetHeadSubStateListPtr(void);
    void SetTailSubStateListPtr(ListSubState_t *NewTailSubStateListPtr);
    ListSubState_t *GetTailSubStateListPtr(void);

    void AddSuperStateExtendedListSubState(ListSubState_t *NewListSubStateData);
    int GetSuperStateExtendedListNumSubStates(void);
    CString GetSuperStateExtendedListSubStateName(int SubStateNumber);
    void RemoveLastSuperStateExtendedListSubStateName(void);
    void SetHeadSubStateExtendedListPtr(ListSubState_t *NewHeadSubStateExtendedListPtr);
    ListSubState_t *GetHeadSubStateExtendedListPtr(void);
    void SetTailSubStateExtendedListPtr(ListSubState_t *NewTailSubStateExtendedListPtr);
    ListSubState_t *GetTailSubStateExtendedListPtr(void);

private:
    SuperState_t SuperState;
    SuperStateClass *NextElementPtr;

    int NumListSubStates;
    ListSubState_t *HeadSubStateListPtr;
    ListSubState_t *TailSubStateListPtr;
    ListSubState_t *FindSubStateListElementPtr(int SubStateListNumber);

    int NumExtendedListSubStates;
    ListSubState_t *HeadSubStateExtendedListPtr;
    ListSubState_t *TailSubStateExtendedListPtr;
    ListSubState_t *FindSubStateExtendedListElementPtr(int SubStateExtendedListNumber);
};

#endif //AFX_MODEL_H__A85A5564_DCDD_3456_8625_D268D3FDEB93__INCLUDED_

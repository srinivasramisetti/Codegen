// State class (non-super state) header file

#if !defined(AFX_MODEL_H__A85A5564_DCDD_2345_8625_D268D3FDEB93__INCLUDED_)
#define AFX_MODEL_H__A85A5564_DCDD_2345_8625_D268D3FDEB93__INCLUDED_


//*********************************************************
struct State_t
{
    int StateNumber;
    CString StateName;
    CString EntryAction;
    CString ActivityFunction;
    CString ExitAction;
};

//*********************************************************
class StateClass: public CommonStateClass
{
public:
    StateClass(State_t *StateData); // Constructor

    int GetStateNumber(void);
    CString GetStateName(void);
    CString GetEntryAction(void);
    CString GetActivityFunction(void);
    CString GetExitAction(void);
    void SetNextElementPtr(StateClass *NewNextElementPtr);
    StateClass *GetNextElementPtr(void);
protected:
    State_t State;
    StateClass *NextElementPtr;
};

#endif //AFX_MODEL_H__A85A5564_DCDD_2345_8625_D268D3FDEB93__INCLUDED_
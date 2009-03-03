// Common state class header file

#if !defined(AFX_MODEL_H__A85A5564_DCDD_1234_8625_D268D3FDEB93__INCLUDED_)
#define AFX_MODEL_H__A85A5564_DCDD_1234_8625_D268D3FDEB93__INCLUDED_

//*********************************************************
struct Trans_t
{
   CString TransStatus;
   int TargetState;
   int TransCondMask;
   int TransDontCareMask;
   CString TransActionFunctionName;
   CString TransActionFunctionBody;
   CString AddedBySuperState;
   Trans_t *NextElementPtr;
};

//*********************************************************
class CommonStateClass
{
public:
    CommonStateClass(void); // Constructor
    void UpdateTargetState(int TransNumber, int UpdatedNextState);
    void UpdateTransCondDontCareMasks(int TransNumber, int NewCondMask, int NewDontCareMask);
    int GetNumTrans(void);

    void AddTrans(Trans_t *TransPtr);
    CString GetTransActionFunctionName(int TransNumber);
    CString GetTransActionFunctionBody(int TransNumber);
    int GetTransTargetState(int TransNumber);
    int GetTransCondMask(int TransNumber);
    int GetTransDontCareMask(int TransNumber);
    CString GetTransAddedBySuperState(int TransNumber);
    CString GetTransStatus(int TransNumber);

    CString GetEntryAction(void);
    CString GetActivityFunction(void);
    CString GetExitAction(void);

protected:
    int NumTrans;
    Trans_t *HeadTransPtr;
    Trans_t *TailTransPtr;
    Trans_t *FindTransElementPtr(int TransNumber);
};

#endif //AFX_MODEL_H__A85A5564_DCDD_1234_8625_D268D3FDEB93__INCLUDED_
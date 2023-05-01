class IInputSystem
{
public:
    
    void EnableInput(bool bEnable)
    {
        typedef void (* oEnableInput)(void*, bool);
        return getvfunc<oEnableInput>(this, 11)(this, bEnable);
    }
    
    bool IsButtonDown(ButtonCode_t code) {
        typedef bool (* oIsButtonDown)(void*, ButtonCode_t);
        return getvfunc<oIsButtonDown>(this, 15)(this, code);
    }
    
    void ResetInputState()
    {
        typedef void (* oResetInputState)(void*);
        return getvfunc<oResetInputState>(this, 39)(this);
    }
    
    
};

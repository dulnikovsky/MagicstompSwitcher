
const int numOfPatches = 99;
const int PatchNameLength = 12;

enum MagistompPatchDesc
{
    PatchType,
    Control1 = 2,
    Control2 = 4,
    Control3 = 6,
    PatchName = 16,
    PatchNameLast = PatchName + PatchNameLength,
    PatchCommonLength = 0x20,
    PatchEffectLength = 0x7F,
    PatchTotalLength = PatchCommonLength + PatchEffectLength
};

enum class SysExDumpState
{
    Idle,
    ExpectingStart,
    ExpectingCommonData,
    ExpectingEffectData,
    ExpectingEnd
};



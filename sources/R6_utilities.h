
/*// Name-Value Pair type - an item in enumeration type
typedef struct NVP
{
    int32 value;
    std::string name;
}
NVP;
// Name-Value Pair Container type - an enumeration type
typedef std::vector<NVP> NVPC;

// Each camera has one or more ports, this structure holds information with port
// descriptions. Each camera port has one or more speeds (readout frequencies).
// On most EM cameras there are two ports - one EM and one non-EM port with one
// or two speeds per port.
// On non-EM camera there is usually one port only with multiple speeds.
typedef struct READOUT_OPTION
{
    NVP port;
    int16 speedIndex;
    float readoutFrequency;
    int16 bitDepth;
    std::vector<int16> gains;
}
READOUT_OPTION;

extern std::vector<READOUT_OPTION> g_SpeedTable;
bool ReadEnumeration(NVPC *nvpc, uns32 paramID, const char *paramName);
*/
int IsParamAvailable(int16 hCam, uns32 paramID);
int GetParamValue(int16 hCam, uns32 paramID, void *currentValue);
int GetEnumParamValue(int16 hCam, uns32 paramID, char *descriptionTxt);






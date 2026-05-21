/* #########################   INCLUDE	 ################################## */

//# include <Windows.h>

# include <stdio.h>
# include <time.h>
//#include <QCamApi.h>  
#include <nivision.h>
//#include <niimaq.h>
//# include <Sencam.h> 

// F2026 addition 
# include "master.h"
# include "pvcam.h"



/* #########################   DEFINE ################################## */
#define MESSAGE_ABOUT			"Copyright: POLITECNICO DI MILANO\nVersion: 1.2\nDate: Settembre 2005"

#define FILENAME_LEN				260L
#define LENCOMMENT					700
#define LENPATH 					1000
#define STRLEN						1000
#define LENCOMMAND					512
#define ERROR_MSG_LEN				1000
#define SETTING_DIR 				"Settings"
#define SETTING_EXT 				"*.SET"
#define SETTING_DEFAULT 			"Settings\\DEFAULT.SET"
#define NUM_INTENSITIES 			16384
#define HEADER_LEN					1280
#define EMPTY_HEADER				30
#define LENGAIN						3
//#define NUM_LEVEL_12BIT				4096
//#define NUM_LEVEL_8BIT				256
#define NUM_COLORS_8_BIT			256
#define GRAY_LOW					0
#define GRAY_HIGH					16382

#define MAX_LABEL_DELAY				99999
#define LENDELAY					5
#define LENSUFFIX					3
#define LENCODE 					5
#define SLASH						"\\"
#define UNDER						"_"
#define EXTLIST 					".lst"
#define EXTLISTINPUT				".lstIn"
#define EXTLISTOUTPUT				".lstOut"
#define EXTIMG						".imh"		

#define DEL150						0
#define STANFORD					1
#define ZERO_DELAY					0

#define GPIB0						0
#define ADDRDELAY					15
#define NSECTOSEC					1E-9
#define PSECTOSEC					1E-12
#define MUSECTOSEC					1E-6


#define SECTONSEC					1E+9

#define BAUDRATE					38400
#define COMNUM						5
#define COMNAME 					"COM5"
#define CR							13
#define WAIT_TIME					300L  // 300 msec
#define OVERLENGTH_GATE 			40	  // 40 ns
#define TIMEOUT			 			1	  // 1 s
#define WAIT_TIME_MS				1



#define SENSI						0
#define WIDTH_SENSI 				1280
#define HEIGHT_SENSI				1024
#define SEQ_MODE					0
#define SIM_MODE					65536
#define BINX_MAX					8
#define BINY_MAX					8

#define QIMAGING					1
#define WIDTH_QIMAGING 				1600
#define HEIGHT_QIMAGING				1200

#define R6							2
#define WIDTH_R6 					2688
#define HEIGHT_R6					2200
#define MINUS_TEN_DEGREE			-10


#define MAX_GAIN					999
#define ZERO_GAIN					600
#define THR_DIFF_GAIN				5
#define MAXDELAYS					1000
#define NUMDELAYS_SUBSET			12
#define SINGLE_SHOT 				4
#define CONTINUOS					0

#define MANUAL						0
#define AUTO_GAIN					1
#define AUTO_GAIN_FAST				2
#define AUTO_GAIN_TEXP				3


#define IMAGE_DIR					"C:\\"
#define IMAGE_EXT					"*.imh"
#define INPUT_EXT					"*.lstIn"

#define SLOW						1
#define FAST						3

#define ZERO_EXPOSURE				100
#define MAX_EXPOSURE				1000000
#define GRAY_MIN					5

#define NUM_GAIN_VALUES				21

#define IMAGING 					1
#define SPECTROSCOPY				0

#define T_INT16						1 
#define T_UNS16						2
#define T_UNS32						3
#define T_ENUM						4
#define T_BOOL						5
#define T_ULONG64					6



// F2026 Additions
# define MAXTABLEMEAS	   1000
# define MAXNAMELENGTH  	100
# define MAXLINELENGTH 		500
# define MAXCSVDELAYS		 50

# define MAXDELAYSFORGAINCALC		 12
# define MAXDELAYSFORMEASUREMENT	100


/****************************************************************/

struct sHardware{
	int Ccd;		// Ccd initialized
	int Intensifier;// Intensifier initialized
	int Del;		// Del Initialized
	int Buffer;		// Buffer allocated
	int Laser;		// Laser on
	int CcdType;	// Ccd type (Sensi=0)
	int DelayType; // Del type (Del150=0 or Stanford=1)
};

struct sCcd {	
	uns16 Roix1;
	uns16 Roix2;
	uns16 Roiy1;
	uns16 Roiy2;	
	uns16 Binningx;
	uns16 Binningy;	
	int RowLength;
	int ColLength;
	int Exposure;
	unsigned int grayL;
	unsigned int grayH;
	uns32 Numbytes;
	int displayGrayLevelAuto;
};


struct sQImaging {
	/*
	QCam_Handle handle;
	QCam_Err error;
	bufferunsigned long listLength;
	QCam_CamListItem lista[1];	
	QCam_Settings settings;
	unsigned long currentFrame;
	*/
};

struct sR6{  
	int16 handle;
	char name[STRLEN];
	uns16 sensorResX;
	uns16 sensorResY;
	//int16 Temperature;
	//int16 Temperature_SetPoint;
};

struct sDel150{
	short hndl;
	float range;
	short triggerSlope;
	float triggerLevel;
	short triggered;
	short invertedOutput;
};

struct sStanford{
	int hndl;
	int triggerExternal;
	int triggerSlope;
	float triggerLevel;
	short triggered;
	short invertedOutput;
};

struct sCom{
	int Num;
	int error;
};


struct sGate{
	int Width;
	int McpGateOn;
	int On;
};


struct sGain{
	int Init; //initial gain during search
	int Set;	// gain to be set
	int Type;			// auto or manual search for best gain
	int ThrLevel;	// thr in trms of pixel intensity
	float ThrNumPix; // thr in terms of pixel number
	float Sat;				// percentage of saturated pixels
	double Slope;
};

struct sDelay{
	int Set;
	int ID;
	int First[3];
	int Step[3];
	int Last[3];
	int BkgFirst;
	int BkgLast;
	int BkgStep;
	int AutoValueFile; 
	int NumDelaysAuto;
	int Nom[MAXDELAYS];
	int Gain[MAXDELAYS];
	int TimeExp[MAXDELAYS];
	char Label[MAXDELAYS][LENDELAY+1+LENSUFFIX+1];
	int Subset[NUMDELAYS_SUBSET];
	int GainSubset[NUMDELAYS_SUBSET];
	unsigned short int timescale; //1 nanosecond, 2 picosecond, 3 microsecond
	//int delayZero;
};

struct sLaser {
	int Freq;
};

struct sActual{
	int Delay;
	int Gate;
	int Gain;
	};

struct sAcquisition{
	int NumAverage;
	int DisplayMode;
	int OtherScanTst;
	int BkgScan;
	int Mode;
};

struct sInfo{
	char Prefix[32];
	int Code;
	char Suffix[5];
	char Comment[LENCOMMENT];
	char Dir[LENPATH];
};

struct sFileList{
	char Name[LENPATH];
	char Path[LENPATH];
	FILE *File;
	char PathIn[LENPATH];
	FILE *FileIn;
	char PathOut[LENPATH];
	FILE *FileOut;
};

struct sFileImage{
	char Name[MAXDELAYS][LENPATH];
	char Path[MAXDELAYS][LENPATH];
	FILE *File;
};

struct sFile{
	char Dir[LENPATH];
	struct sFileList List;
	struct sFileImage Image;
};

struct sHist{
	long Vals[NUM_INTENSITIES];
	double Mean;
	long Limit;
	long Peak;
	long Max;
	long Somma;
	float SatPercentage;
	long NumSat;
	long Intensity;
};


struct sBkg{
	int On;
	char Name[LENPATH];
	char Path[LENPATH];
	FILE *File;
	int Width;
	int Height;

};


//F2026 additions
# define X(type, fmt, ctype) type,
typedef enum  
{
# include "types.def"
	NUMTYPES
} DATATYPE;
# undef X


// stMeas type definition
typedef struct{
	int Ctrl[MAXTABLEMEAS];
	int Pnl[MAXTABLEMEAS];
	void *Addr[MAXTABLEMEAS];
    // F2026 addition
	char CtrlName[MAXTABLEMEAS][MAXNAMELENGTH]; 
    DATATYPE Type[MAXTABLEMEAS];
} stMeas;


typedef struct {
	int panel;
	int control;
} UIException;


const char *TYPENAMES[NUMTYPES] = {
# define X(type, fmt, ctype) #type,
# include "types.def"
# undef X
};

const char *TYPEFORMATS[NUMTYPES] = {
# define X(type, fmt, ctype) fmt,
# include "types.def"
# undef X
};




/************** Global Variable Declarations **************/ 
struct sHardware Hardware;
struct sCcd Ccd;
struct sQImaging QImaging;
struct sR6 ccdR6;
struct sDel150 del150;
struct sStanford stanford;
struct sCom Com;
struct sGate Gate;
struct sGain Gain;
struct sDelay Delays;
struct sLaser Laser;
struct sActual Actual;
struct sAcquisition Acquisition;
struct sInfo Info;
struct sFile File;
struct sHist Hist;
struct sBkg Bkg;

//F2026 addition
stMeas tMeas;


//short *buffer13;
uns16 *buffer12;
uns16 *buffer12Bkg;
uns16 *displayBuffer12;
double *bufferSpectrum;  
rgn_type ccdRoi;
//FRAME_INFO *ccd_pFrameInfo = NULL;	  // F2026
FRAME_INFO *ccd_pFrameInfo;

RGBValue userPalette[NUM_COLORS_8_BIT];
int voltageValues[NUM_GAIN_VALUES];
double gainValues[NUM_GAIN_VALUES];

char Message[STRLEN];
//HWND hWindCamera,hWindBW;
int d150panel, stanfordPnl, console, keypress;
int smallpanel, measure, graph, taupanel;

// F2026 addition
int SwitchGainMeas;
int SwitchGainCalc;

int NumMeas, NumSmallPnl, NumD150Pnl, NumStanfordPnl, NumCtrl, NumConsoleProfile, NumDelaysGainCalcMeasurement;


int DelaysForGainCalc[MAXDELAYSFORGAINCALC];
int DelaysForMeasurement[MAXDELAYSFORMEASUREMENT];





/******************************Function prototypes*************************************************************/
void checkR6Error(void);

int InitCCD(int ccdType);
int QuitCCD(int ccdType);
void GetCcdSettings(int ccdType);
void SetCcdSettings(int ccdType);
int SetCcdDefaultSettings(int ccdType);
int SetTimeExpM(int ccdType);
int GetCcdStatus(int ccdType);
void SetSeqMode (int ccdType);
void ResetSimMode(int ccdType);
void GetImageSize(int ccdType,int *w,int *h);
void StartCcd(int ccdType, uns16 *b);
void ReadCcd(int ccdType, uns16 *b);
void GrabCcdImage(int ccdType, int mode, uns16 *b);
//void ConvertTo8(int ccdType, uns32 *b12, byte *b8);
void StopCcd(int ccdType, int mode);
//void SubBkg(void *frame);
//void QCAMAPI cbk_DisplayFrame(void* userPtr,unsigned long userData,QCam_Err errcode,unsigned long flags);

int InitIntensifier(void);
void QuitIntensifier(void);
void UpdateIntensifierM(void);
int SetGainM (int CurrentGain);
void WriteIntensifier(char *StringCommand);
void ReadIntensifier(char *StringResponse, char *StringCommand, int echo);
void VerifyGain(int valueSet, int *valueRead);
int GetRealGain(void);
void Wait(clock_t pause);


int InitDelayGenerator(int delayType);
int InitDel150(void);
int InitStanford(void);
short UpdateD150M(void);
void UpdateStanfordM(void); 
void QuitDelayGenerator(int delayType); 
int SetDelayM (int delayType, int d, unsigned short int timescale);

void AllocBuffer(int ccdType);
void QuitBuffer(void);
void FreeBuf(void);
void MFreeBuf(void);
void RotateBuffer(uns16 *b12,int w, int h);

void ActivateIntensifierButtons(void);
void ActivateDELButtons(void);
void ActivateCCDButtons(int ccdType);
void ActivateLaserButtons(void);
void DeactivateButtons(void);

int LoadBkgM(void);
void LoadImgM(void);
int SingleGrabM(void);
int ImageGrabM(void);
int AcquireM(void);
void SelectGainM(int *g);
void FindCcdExposure(int value_msec,int *roundValue_msec);
void FindOptimal_ExpTime_Gain (double ev, double *Gain, double *T_Exp);
void FindOptimal_Gain (double ev, double *g);
int FindVoltage(double gain);


void GetImage(uns16 *b12);

void OpenDisplay(int *window);
void CloseDisplay(int *window);
void CreateImage(uns16 *buffer, int w, int h, Image *image);
void DisplayImage(Image *image, int *window);
//void FreeImage(Image *image);

void CreateSpectrum(uns16 *buffer12, int w, int h, double *bufferSpectrum);
void DisplaySpectrum(double *bufferSpectrum,int w);


int SaveImage(uns16 *buffer);
void SaveAuto(uns16 *buffer);


void ShowConsole(void);
void StatsCalc(uns16 *buffer,int rowL, int colL);
void CreatePath(void);
void CreatePath1(void);
void AutoIncrementFile(void);
int OverwritingFile(void);

void CreateUserPalette(void);
void InitUir(void);
void *GetAddrMeas(int Control, int Panel);
void CreateGainValues(void);


//F2026 addition 
void CreateTableMeas(stMeas *s);
void PrintSettingsTxt(FILE *fp, stMeas *s);
void ReadAllControls(stMeas *s);
void String2Value(char *s, DATATYPE type, void *out);
void UpdateControl(int panelHandle, int controlID, DATATYPE type, void *addr);
void GetSettingsPath(char *s);
int isException(int panel, int control, UIException ex[], int nExceptions);
void ReadFromFile( FILE * fp, stMeas *s);
void InitDelayArray(int DelayArr[], int length);
//void ReadCsvDelays( FILE * fp, int *DelArray, int textBoxPanel, int textBoxControl);
void WriteArrayToTextBox(int panel, int textBoxControl, int arr[], int numel);
void ReadTextBoxToArray(int panel, int textBoxControl, int arr[], int numel);
int IsValidInt(char * str);
void DimDelayNumBoxes(int yn);
void DimDelayMeasScans(int yn);
void ReorderArray(int arr[], int numel);
int comp(const void *a, const void *b);


	



#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include <ansi_c.h> 
#include <stddef.h>
//#include <Windows.h>
#include <stdlib.h>

//'include' per intensificatore
#include <utility.h>
#include <formatio.h>
#include <rs232.h>
#include <gpib.h>

# include <math.h>


//#include "del_def.h" 

//'include' for Qimaging camera
//#include <QCamApi.h>
//#include <QImgTypes.h>  

// include statements for Retiga R2 camera
#include "master.h"
#include "pvcam.h"
#include "R6_utilities.h"


#include "Measure_nsF2026.h"
FRAME_INFO *ccd_pFrameInfo = NULL;	  // initialization 

#include "Panel_ns.h"
//#include "Table_ns.h"
//#include "utils.h"


static int QuitProg;
static Image* image;

/***************** Global Variable definitions ***************/

/*
// table 
stMeas tMeas;

// arrays containing the delays to use for Gain calculation and measurements
int DelaysForGainCalc[MAXDELAYSFORGAINCALC];
int DelaysForMeasurement[MAXDELAYSFORMEASUREMENT];


int NumMeas, NumSmallPnl, NumD150Pnl, NumStanfordPnl, NumCtrl, NumConsoleProfile, NumDelaysGainCalcMeasurement;

// contain the values of the binary old-new switches
int SwitchGainMeas = 0;
int SwitchGainCalc = 0;
*/

// small function declarations  <=========TEMPORARY!!!
ColorMapEntry *CreateColorMap(double minVal, double maxVal);   
void getMinMax(double *data, int size, double *min, double *max);
void DrawColorBar(int panel, int colorBarCtrl, ColorMapEntry *cMap, double minVal, double maxVal);
void InitTauPanel();


	int Nx = 100;
	int Ny = 100;
	double tau[10000];

	double maxTau ;
	double minTau ;
ColorMapEntry *cMap;


/**************************************************************/


int main (int argc, char *argv[])
{
	
	// placeholder
	// prototype drawing tau 
	
	for(int i = 0; i<Nx*Ny; i++){
		tau[i] = (double) i;
	}

	getMinMax(tau, Nx*Ny, &minTau, &maxTau); // saves the minTau and maxTau
	
	 //
	 
	 
	// initialize the delay arrays to -1
	InitDelayArray(DelaysForGainCalc, MAXDELAYSFORGAINCALC);
	InitDelayArray(DelaysForMeasurement, MAXDELAYSFORMEASUREMENT);
	//
	
	QuitProg=0;
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((d150panel = LoadPanel (0, "sources/Panel_ns.uir", D150PANEL)) < 0)
		return -1;
	if ((stanfordPnl = LoadPanel (0, "sources/Panel_ns.uir", STFORD_PNL)) < 0)
		return -1;
	if ((console = LoadPanel (0, "sources/Panel_ns.uir", CONSOLE)) < 0)
		return -1;
	if ((keypress = LoadPanel (0, "sources/Panel_ns.uir", KEYPRESS)) < 0)
		return -1;
	if ((smallpanel = LoadPanel (0,"sources/Panel_ns.uir", SMALLPANEL)) < 0)
		return -1;
	if ((measure = LoadPanel (0, "sources/Panel_ns.uir", MEASURE)) < 0)
		return -1;
	if ((graph = LoadPanel (0, "sources/Panel_ns.uir", SPECTR_PNL)) < 0)
		return -1;
	if ((taupanel = LoadPanel (0, "sources/Panel_ns.uir", TAU_PNL)) < 0)
		return -1;
	
	
	DisplayPanel (measure);
	DisplayPanel (console);
	CreateUserPalette();
	//CreateTableMeas();  //OLD
	CreateTableMeas(&tMeas);
	InitUir();	
	image = imaqCreateImage (IMAQ_IMAGE_U16, 1);  
	
	Hardware.Ccd=0;
	Hardware.Del=0;
	Hardware.Intensifier=0;	
	Hardware.Buffer=0;
	do 
	RunUserInterface (); 
	while(!QuitProg);
	
	DiscardPanel (d150panel);
	DiscardPanel (stanfordPnl);
	DiscardPanel (console);
	DiscardPanel (keypress);
	DiscardPanel (smallpanel);						   
	DiscardPanel (measure);
	return 0;
}
/***************************Funzioni di supporto per DISPLAY************************************/
 void CreateUserPalette(void){

	unsigned char i;
	
	for (i=0;i<(NUM_COLORS_8_BIT-1);i++){
		userPalette[i].R = i;
		userPalette[i].G = i;
		userPalette[i].B = i; 
		userPalette[i].alpha = 0; 
	}
	userPalette[NUM_COLORS_8_BIT-1].R = NUM_COLORS_8_BIT - 1;  
	userPalette[NUM_COLORS_8_BIT-1].G = 0;
	userPalette[NUM_COLORS_8_BIT-1].B = 0; 
	userPalette[NUM_COLORS_8_BIT-1].alpha = 0; 
 }


/***************************Funzioni di supporto per Uir************************************/

/* INITIALIZE USER INTERFACE */
void InitUir(void){
// New version below
/*	
	int idx;
	
	
	RecallPanelState(measure,SETTING_DEFAULT,MEASURE);
	for(idx=0;idx<NumMeas;idx++){
		GetCtrlVal(measure,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	}
	Ccd.Binningy=Ccd.Binningx;
	
	RecallPanelState(smallpanel,SETTING_DEFAULT,SMALLPANEL);
	for(idx=NumMeas;idx<NumSmallPnl;idx++){
		GetCtrlVal(smallpanel,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	}
	
	RecallPanelState(d150panel,SETTING_DEFAULT,D150PANEL);
	for(idx=NumSmallPnl;idx<NumD150Pnl;idx++){
		GetCtrlVal(d150panel,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	}
	
	RecallPanelState(stanfordPnl,SETTING_DEFAULT,STFORD_PNL);
	for(idx=NumD150Pnl;idx<NumStanfordPnl;idx++){
		GetCtrlVal(stanfordPnl,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	}   
	RecallPanelState(graph,SETTING_DEFAULT,SPECTR_PNL);		 
	RecallPanelState(console,SETTING_DEFAULT,CONSOLE);
*/	

// F2026
	FILE *fp = fopen("Settings/DEFAULT_SETTINGS_F2026.txt", "r");
	ReadFromFile( fp, &tMeas);
	fclose(fp);
	
	// write delay arrays to text boxes
	WriteArrayToTextBox(measure, MEASURE_TEXTBOX_DELGAINCALC, DelaysForGainCalc,  MAXDELAYSFORGAINCALC);
	WriteArrayToTextBox(measure, MEASURE_TEXTBOX_DELMEAS, DelaysForMeasurement,  MAXDELAYSFORMEASUREMENT);
	
	// miscellaneous
	SetCtrlVal(measure, MEASURE_OVERWRITE_SETTINGS, 0); // set the overwrite button to off
	//SwitchGain (measure, MEASURE_SWITCH_GAIN_CALC, EVENT_COMMIT, NULL, 0, 0); // execute the binary switches callbacks
	//SwitchGain (measure, MEASURE_SWITCH_GAIN_MEAS, EVENT_COMMIT, NULL, 0, 0); 
	Ccd.Binningy = Ccd.Binningx;

	
}

/***************************Funzioni di supporto per Uir************************************/

void *GetAddrMeas(int Control, int Panel)
{
	int idx;
	for(idx=0;idx<NumCtrl;idx++){
		if((tMeas.Ctrl[idx]==Control)&(tMeas.Pnl[idx]==Panel))
			return(tMeas.Addr[idx]);
	}
	return(NULL);
}

/***************************Funzioni di supporto per Uir************************************/


// callback for updating values of controls in MEASURE panel
int CVICALLBACK UpdateMeasure (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	void *address;
	
	if(event!=EVENT_VAL_CHANGED)
		return 0;
	address=GetAddrMeas(control,panel);
	GetCtrlVal(panel,control,address);
	
	//printf("%d\n", *(int*)address); // should show the user inserted value

	if (control==MEASURE_BINNING) 
		Ccd.Binningy=Ccd.Binningx;   
	
	if (control==MEASURE_GAIN_TYPE){
		if (Gain.Type==AUTO_GAIN_FAST)
		{
			 // If AUTO_GAIN_FAST, look at the SwitchGainCalc value
			 SetCtrlAttribute(measure, MEASURE_SWITCH_GAIN_CALC, ATTR_DIMMED, 0); 
			 SwitchGain(measure, MEASURE_SWITCH_GAIN_CALC, EVENT_COMMIT, NULL,0,0);
		}
		else
		{
			// Dim UI elements
			SetCtrlAttribute(measure, MEASURE_SWITCH_GAIN_CALC, ATTR_DIMMED, 1); 
			DimDelayNumBoxes(1);
			SetCtrlAttribute(measure, MEASURE_TEXTBOX_DELGAINCALC, ATTR_DIMMED, 1);  
			SetCtrlAttribute(measure, MEASURE_ENTER_TO_ORDER, ATTR_DIMMED,1);
			
		}
	}
	// update fields that share the same address
	if (panel == measure && control==MEASURE_DELAY){
		UpdateControl(smallpanel, SMALLPANEL_DELAY_SMALL, INT, address);
	}
	if (panel == smallpanel && control==SMALLPANEL_DELAY_SMALL){
		UpdateControl(measure, MEASURE_DELAY, INT, address);
	}
	if (panel == measure && control==MEASURE_GAIN){
		UpdateControl(smallpanel, SMALLPANEL_GAIN_SMALL, INT, address);
	}
	if (panel == smallpanel && control==SMALLPANEL_GAIN_SMALL){
		UpdateControl(measure, MEASURE_GAIN, INT, address);
	}
	
	
	
	return 1;
}

/***************************Funzioni di supporto per Uir************************************/

void CVICALLBACK Quit (int menuBar, int menuItem, void *callbackData,
					   int panel)
{
	int flag;
	
	if (Hardware.Ccd==1){
		flag=QuitCCD(Hardware.CcdType); 
		if (flag<0) {
			MessagePopup ("Warning", "CCD camera was not properly closed !");  
		}
			
		if (Hardware.Buffer==1) {	
			QuitBuffer();
			Hardware.Buffer=0;
		}
	}	
	
	if (Hardware.Del==1){	
		QuitDelayGenerator(Hardware.DelayType);
		
	}	
	
	if (Hardware.Intensifier==1){
		QuitIntensifier();			
	}
	imaqDispose(image);  
	QuitUserInterface(0);
	QuitProg=1;
}
/*******************************************************************************************/

int CVICALLBACK InitHw (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int statusCcd;
	int statusDel;
	int statusInt;
	
	switch (event)
	{
	case EVENT_COMMIT:
		
		DeactivateButtons();
		
		if (Hardware.Ccd==1){
			statusCcd=InitCCD(Hardware.CcdType);
			if (statusCcd>0) {
				ActivateCCDButtons(Hardware.CcdType);
				sprintf(Message,"\nCcd correctly initialized\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			}
			else {
				Hardware.Ccd=0;
				SetCtrlVal(measure,MEASURE_CCD_ON,0);
				sprintf(Message,"\nError in initializing Ccd\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				Beep();
			}
		}
		if ((Hardware.Ccd==1)&(Hardware.Buffer==0)){
			AllocBuffer(Hardware.CcdType);
			Hardware.Buffer=1;
			sprintf(Message,"\nImaging buffers allocated\n");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message); 
		}
		if (Hardware.Del==1){
			statusDel=InitDelayGenerator(Hardware.DelayType);			
			if (statusDel>0) {
				ActivateDELButtons();
				sprintf(Message,"\nDelay Generator correctly initialized\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			}
			else {
				Hardware.Del=0;
				SetCtrlVal(measure,MEASURE_DEL_ON,0);
				sprintf(Message,"\nError in initializing Delay Generator\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				Beep();
			}									
		}	
		
		if (Hardware.Intensifier==1){
			statusInt=InitIntensifier(); 
			if (statusInt>=0){
				ActivateIntensifierButtons();
				sprintf(Message,"\nIntensifier correctly initialized\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				CreateGainValues();
			}
			else{
				Hardware.Intensifier=0;
				SetCtrlVal(measure,MEASURE_INT_ON,0);
				sprintf(Message,"\nError initializing intensifier\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				Beep();
			}
		}

		if (Hardware.Laser==1){
			ActivateLaserButtons();				
		}
		
		
		break;
	}
	return 0;
}
//**************************************************************************************//

int CVICALLBACK UpdateIntensifier (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		UpdateIntensifierM();
		break;
	}
	return 0;
}

//*************************************Update CCD parameters************************************//

int CVICALLBACK UpdateCcd (int panel, int control, int event,			//callback of UPDATE CCD button
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:						
			SetCcdSettings(Hardware.CcdType);						
			break;
		}
	return 0;
}

//*************************************Update CCD parameters************************************//

int CVICALLBACK UpdateRoiValues (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal(panel,MEASURE_BINNING,&Ccd.Binningx);
			Ccd.Binningy=Ccd.Binningx;
			/*   ONLY FOR SENSI or QIMAGING
			SetCtrlVal(panel,MEASURE_ROI_X1,0);
			SetCtrlVal(panel,MEASURE_ROI_X2,WIDTH_QIMAGING/Ccd.Binningx);
			SetCtrlVal(panel,MEASURE_ROI_Y1,0);
			SetCtrlVal(panel,MEASURE_ROI_Y2,HEIGHT_QIMAGING/Ccd.Binningx);
			GetCtrlVal(panel,MEASURE_ROI_X1,&Ccd.Roix1);
			GetCtrlVal(panel,MEASURE_ROI_X2,&Ccd.Roix2);
			GetCtrlVal(panel,MEASURE_ROI_Y1,&Ccd.Roiy1);
			GetCtrlVal(panel,MEASURE_ROI_Y2,&Ccd.Roiy2);
			*/
			
			break;
		}
	return 0;
}

//*************************************Set Delay and gain************************************//

int CVICALLBACK SetTimeExp (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int status, value;
	switch (event)
		{
		case EVENT_COMMIT:
			status=GetCtrlVal(panel,control,&value);

			if (Hardware.Laser)
				FindCcdExposure(value,&Ccd.Exposure);
			else
				Ccd.Exposure=value;

			status=SetCtrlVal(measure,MEASURE_CCD_EXPOSURE,Ccd.Exposure);
			status=SetCtrlVal(smallpanel,SMALLPANEL_CCD_EXPOSURE,Ccd.Exposure);
			SetTimeExpM(Hardware.CcdType);
			break;
		}
	return 0;
}

//*************************************Set Delay and gain************************************//
int CVICALLBACK SetDelay (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	char TimescaleString[STRLEN];
	switch (event)
	{
	case EVENT_COMMIT:
		
		Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Set,Delays.timescale);   
		switch (Delays.timescale)  {  
				case (1): //NANOSECOND  
					sprintf(TimescaleString,"nanosecond"); break;
				case (2): //PICOSECOND   
					sprintf(TimescaleString,"picosecond"); break;  
				case (3): //MICROSECOND	
					sprintf(TimescaleString,"microsecond"); break; 
		}	 
		sprintf(Message,"\n Delay set to: %d %s",Actual.Delay,TimescaleString);
		SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
		break;
		
	}
	return 0;
}
//*************************************Set Delay and gain************************************//

int CVICALLBACK SetGain (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		
		Actual.Gain=SetGainM(Gain.Set);
		
		break;
	}
	return 0;
}
//**************************************************************************************//
/* DA ELIMINARE */
int CVICALLBACK BkgOnOff (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int ledOn;
	int status;
	
	switch (event)
	{
	case EVENT_COMMIT:
		GetCtrlVal(measure,MEASURE_BKGSWITCH,&ledOn);
		if (ledOn){
			status=LoadBkgM();
			if (status>0){
				SetCtrlVal(measure,MEASURE_BKG_LED,1);
				Bkg.On=1;
			}
			else{
				SetCtrlVal(measure,MEASURE_BKG_LED,0);				
				Bkg.On=0;
			}
		}
		else{
			SetCtrlVal(measure,MEASURE_BKG_LED,0);
			Bkg.On=0;
		}			
		break;
	}
	return 0;
}

//**************************************************************************************//

int CVICALLBACK SingleGrab (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		SingleGrabM();
		break;
	}
	return 0;
}
//*************************************Set Delay and gain************************************//

int CVICALLBACK ImageGrab (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		ImageGrabM();
		break;
	}
	return 0;
}
//*************************************Set Delay and gain************************************//

void CVICALLBACK Acquire (int menuBar, int menuItem, void *callbackData,
						  int panel)
{
	AcquireM(); 
}


//**************************************************************************************//

void CVICALLBACK LoadSetting (int menuBar, int menuItem, void *callbackData,
							  int panel)
{
	int idx;
	char fpath[LENPATH];
	int status;
	
	FileSelectPopup (SETTING_DIR, SETTING_EXT, "*.*", "File Settings", VAL_LOAD_BUTTON, 0,
		0, 1, 1, fpath);
	status=RecallPanelState(measure,fpath,MEASURE);
	status=RecallPanelState(smallpanel,fpath,SMALLPANEL);
	status=RecallPanelState(d150panel,fpath,D150PANEL);
	status=RecallPanelState(stanfordPnl,fpath,STFORD_PNL);
	status=RecallPanelState(graph,fpath,SPECTR_PNL);		 
	status=RecallPanelState(console,fpath,CONSOLE);
	
	
    for(idx=0;idx<NumMeas;idx++)
		status=GetCtrlVal(measure,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	for(idx=NumMeas;idx<NumSmallPnl;idx++)
		status=GetCtrlVal(smallpanel,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	for(idx=NumSmallPnl;idx<NumD150Pnl;idx++)
		status=GetCtrlVal(d150panel,tMeas.Ctrl[idx],tMeas.Addr[idx]);
	for(idx=NumD150Pnl;idx<NumStanfordPnl;idx++)
		status=GetCtrlVal(stanfordPnl,tMeas.Ctrl[idx],tMeas.Addr[idx]);	
}
//*************************************Set Delay and gain************************************//

void CVICALLBACK SaveSetting (int menuBar, int menuItem, void *callbackData,
							  int panel)
{
	char fpath[MAX_PATHNAME_LEN];   
	int status;

	FileSelectPopup (SETTING_DIR, SETTING_EXT, "*.*", "File Settings",
		VAL_SAVE_BUTTON, 0, 0, 1, 1, fpath);
	status=SavePanelState(measure,fpath,MEASURE);
	status=SavePanelState(smallpanel,fpath,SMALLPANEL);
	status=SavePanelState(d150panel,fpath,D150PANEL);
	status=SavePanelState(stanfordPnl,fpath,STFORD_PNL);
	status=SavePanelState(graph,fpath,SPECTR_PNL);
	status=SavePanelState(console,fpath,CONSOLE);
	
}

//*************************************Set Delay and gain************************************//

void CVICALLBACK LoadImg (int menuBar, int menuItem, void *callbackData,
						  int panel)
{
	LoadImgM();
}

//**************************************************************************************//

void CVICALLBACK ccdpanel (int menuBar, int menuItem, void *callbackData,
						   int panel)
{
	/*
	OPEN_DIALOG_CAM(hWindCamera,1,"Parametri telecamera");
	*/
}

//**************************************************************************************//

void CVICALLBACK GrayPanel (int menuBar, int menuItem, void *callbackData,
							int panel)
{
	/*
	OPEN_DIALOG_BW(hWindBW,1,"");
	*/
	
}

//**************************************************************************************//

void CVICALLBACK SmallPanel (int menuBar, int menuItem, void *callbackData,
							 int panel)
{
	DisplayPanel(smallpanel);
	
}

//**************************************************************************************//

void CVICALLBACK Del150Panel (int menuBar, int menuItem, void *callbackData,
							  int panel)
{
	DisplayPanel(d150panel);
	
}
//*************************************Set Delay and gain************************************//


int CVICALLBACK UpdateD150 (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int triggerOn;
	switch (event)
	{
	case EVENT_COMMIT:
		triggerOn=UpdateD150M();
		SetCtrlVal(panel,D150PANEL_TRIGGERLED,triggerOn);
		break;
	}
	return 0;
}
//*************************************Set Delay and gain************************************//

int CVICALLBACK CloseD150Panel (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		HidePanel(d150panel);		
		break;
	}
	return 0;
}




//**************************************************************************************//

void CVICALLBACK StanfordPanel (int menuBar, int menuItem, void *callbackData,
								int panel)
{
	DisplayPanel(stanfordPnl);
}

//*************************************Set Delay and gain************************************//

int CVICALLBACK UpdateStanford (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		UpdateStanfordM();	
		break;
	}
	return 0;
}
//*************************************Set Delay and gain************************************//

int CVICALLBACK CloseStanfordPanel (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	case EVENT_COMMIT:
		HidePanel(stanfordPnl);
		break;
	}
	return 0;
}

//**************************************************************************************//


void CVICALLBACK About (int menuBar, int menuItem, void *callbackData,
						int panel)
{
	MessagePopup ("ABOUT CAMERA", MESSAGE_ABOUT);
}
//**************************************************************************************//
//**************************************************************************************//
/* DISPLAY IMAGE */

void OpenDisplay(int *window){
	
	imaqGetWindowHandle(window);
	imaqShowScrollbars(*window, 1);
	imaqSetWindowPalette(*window,  IMAQ_PALETTE_USER, userPalette, sizeof(userPalette)/sizeof(*userPalette)); 
	imaqSetWindowZoomToFit(*window,TRUE);
	
}

void CloseDisplay(int *window){  
	imaqCloseWindow(*window);  
}


void CreateImage(uns16 *buffer, int w, int h, Image *image){
	imaqSetImageSize(image,w,h); 
	imaqArrayToImage(image,buffer,w,h); 
}

void DisplayImage(Image *image, int *window){
	DisplayMapping mapping;  
	switch (Ccd.displayGrayLevelAuto){
		case 1:
			mapping.method=IMAQ_FULL_DYNAMIC_ALWAYS; 
			break;
		case 0:
			mapping.method=IMAQ_RANGE_ALWAYS;  
			mapping.minimumValue=Ccd.grayL;
			mapping.maximumValue=Ccd.grayH;
			break;
	}
	imaqSetWindowDisplayMapping(*window,&mapping);
	imaqDisplayImage(image, *window, TRUE);		 
	
}
/*
void FreeImage(Image *image){
	imaqDispose(image); 
}
*/

//**************************************************************************************//
//**************************************************************************************//
/* DISPLAY SPECTRUM */

void CreateSpectrum(uns16 *buffer12, int w, int h, double *bufferSpectrum){
	uns16 *pbuffer1, *pbuffer2;
	double *pbufferSpectrum;
	int j,i;
	
	for (i=1,pbufferSpectrum=bufferSpectrum;i<=w;i++,pbufferSpectrum++){
				*pbufferSpectrum=0;
	}
	for (i=1,pbuffer1=buffer12,pbufferSpectrum=bufferSpectrum;i<=w;i++,pbuffer1++,pbufferSpectrum++){  
		 for (j=1,pbuffer2=pbuffer1;j<=h;j++,pbuffer2=pbuffer2+w){	  
			 *pbufferSpectrum=(*pbufferSpectrum)+(double)(*pbuffer2)/h; 
		 }
	 }
}

void DisplaySpectrum(double *bufferSpectrum, int w){
	DeleteGraphPlot (graph, SPECTR_PNL_GRAPH, -1, VAL_DELAYED_DRAW);   
	PlotY (graph, SPECTR_PNL_GRAPH, bufferSpectrum, w, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_RED);
}

//***********************************************************************************//
/* ######################################################### */
/* ###													 ### */
/* ###				   GESTIONE CCD 					 ### */ 
/* ###													 ### */
/* ######################################################### */
//INITIALIZE CCD  - HO ELIMINATO FUNZIONALITA' di SENSICAM
		
void checkR6Error(void){
	char msg[ERROR_MSG_LEN];   
	
	pl_error_message(pl_error_code(), msg);
	sprintf(Message,"\nError in R6 camera: %s\n", msg);
	SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
}

int CVICALLBACK DisplayMeasurePanel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT: 
			DisplayPanel (measure);
			break;
	}
	return 0;
}

int CVICALLBACK GetCcdParamValue (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	unsigned short int parmNumber;
	uns32 paramID = 0;  
	int CommandType = 0;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(measure, MEASURE_R6_PARM,&parmNumber);
			switch (parmNumber){
				case 1:
					paramID=PARAM_BIT_DEPTH;
					CommandType=T_INT16;
					sprintf(Message,"\nNumber of bits for the current port\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 2:
					paramID=PARAM_CLEAR_CYCLES;   
					CommandType=T_UNS16;
					sprintf(Message,"\nNumber of times for clearing the CCD\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break; 
				case 3:
					paramID=PARAM_CLEAR_MODE;
					CommandType=T_ENUM;
					sprintf(Message,"\nClear mode\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 4:
					paramID=PARAM_COOLING_MODE;				   
					CommandType=T_ENUM;  
					sprintf(Message,"\nCooling mode\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;	  
				case 5:
					paramID=PARAM_EXPOSURE_MODE;
					CommandType=T_ENUM;    
					sprintf(Message,"\nExposure mode\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;  
				case 6:
					paramID=PARAM_FRAME_CAPABLE;   
					CommandType=T_BOOL;    
					sprintf(Message,"\nTrue if the camera can run in frame transfer mode\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 7:
					paramID=PARAM_FWELL_CAPACITY;
					CommandType=T_UNS32; 
					sprintf(Message,"\nFull well capacity\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;  
				case 8:
					paramID=PARAM_PAR_SIZE;  
					CommandType=T_UNS16;
					sprintf(Message,"\nPixel number\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;  
				case 9:
					paramID=PARAM_PIX_PAR_DIST;
					CommandType=T_UNS16;
					sprintf(Message,"\nPixel c-to-c distance (in nm)\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;					  
				case 10:				 
					paramID=PARAM_PIX_PAR_SIZE;  
					CommandType=T_UNS16; 
					sprintf(Message,"\nPixel dimension (in nm)\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;  
				case 11:
					paramID=PARAM_SER_SIZE;
					CommandType=T_UNS16;
					sprintf(Message,"\nPixel number\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;  
				case 12:
					paramID=PARAM_PIX_SER_DIST;
					CommandType=T_UNS16;
					sprintf(Message,"\nPixel c-to-c distance (in nm)\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;					  
				case 13:				 
					paramID=PARAM_PIX_SER_SIZE;
					CommandType=T_UNS16;
					sprintf(Message,"\nPixel dimension (in nm)\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 14:				 
					paramID=PARAM_READOUT_PORT;
					CommandType=T_ENUM;
					sprintf(Message,"\nReadout port\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 15:				  
					paramID=PARAM_READOUT_TIME;
					CommandType=T_UNS32;
					sprintf(Message,"\nReading time (in micros)\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 16:				  
					paramID=PARAM_PREAMP_DELAY;
					CommandType=T_UNS16;
					sprintf(Message,"\nPreamp stabilization time (in ms)\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break; 
				case 17:				  
					paramID=PARAM_SHTR_OPEN_MODE;
					CommandType=T_ENUM;
					sprintf(Message,"\nShutter opening condition\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 18:				  
					paramID=PARAM_SHTR_STATUS;
					CommandType=T_ENUM;  
					sprintf(Message,"\nShutter status\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;	  
				case 19:				  
					paramID=PARAM_TEMP;
					CommandType=T_INT16;   
					sprintf(Message,"\nTemperature in degree x 100\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				/*case 20:
					paramID=PARAM_TEMP_SETPOINT;
					CommandType=T_INT16; 
					break; 
				*/
				case 21:
					paramID=PARAM_READ_NOISE;
					CommandType=T_UNS16;	   
					sprintf(Message,"\nRead noise for the current speed\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
				case 22:
					paramID=PARAM_CIRC_BUFFER;
					CommandType=T_BOOL;	   
					sprintf(Message,"\nTests to see if the hardware/software can perform circular buffer\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;	   
				case 23:
					paramID=PARAM_EXP_RES;
					CommandType=T_ENUM;	   
					sprintf(Message,"\nGets the resolution for the current resolution index\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break; 
				/*
				case 24:
					paramID=PARAM_EXPOSURE_TIME;
					CommandType=T_ULONG64;	   
					sprintf(Message,"\nThis is used to examine current exposure time\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
					*/
				case 25:
					paramID=PARAM_PMODE;
					CommandType=T_ENUM;	   
					sprintf(Message,"\nThis allows the user to select the parallel clocking method.\n");
					SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					break;
					
					
			}
			switch (CommandType){
				case T_INT16:
					int16 int16currentValue;
					GetParamValue(ccdR6.handle, paramID, &int16currentValue);		
					SetCtrlVal(measure, MEASURE_CCD_PARM_VALUE,(int)(int16currentValue)); 
					break;  
				case T_UNS16:
					uns16 uns16currentValue; 
					GetParamValue(ccdR6.handle, paramID, &uns16currentValue);  
					SetCtrlVal(measure, MEASURE_CCD_PARM_VALUE,(int)(uns16currentValue));   
					break;  
				case T_UNS32:
					uns32 uns32currentValue;
					GetParamValue(ccdR6.handle, paramID, (uns32*)&uns32currentValue);   
					SetCtrlVal(measure, MEASURE_CCD_PARM_VALUE,(int)(uns32currentValue));
					break; 
				case T_ULONG64:
					ulong64 ulong64currentValue;
					GetParamValue(ccdR6.handle, paramID, (ulong64*)&ulong64currentValue);   
					SetCtrlVal(measure, MEASURE_CCD_PARM_VALUE,(int)(ulong64currentValue));
					break; 
				case T_ENUM:
					char StrMessage[STRLEN];  
					GetEnumParamValue(ccdR6.handle, paramID,StrMessage);  
					SetCtrlVal(measure, MEASURE_CCD_PARM_STRING,StrMessage);   
					break;
				case T_BOOL:
					rs_bool boolcurrentValue;
					GetParamValue(ccdR6.handle, paramID, &boolcurrentValue);  
					SetCtrlVal(measure, MEASURE_CCD_PARM_VALUE,(int)(boolcurrentValue));   
					break;
			
			}
			checkR6Error();
			break;
			
	}
	return 0;
}


int InitCCD(int ccdType){   
	
	switch (ccdType) {
		case SENSI: 
			/*
			SET_BOARD(0);				// Define existing board as '0' board 
			returnCode=SET_INIT(2); 	// Initialize board with default values  
			if (returnCode!=0){ 	
			return -1;
			}		
			OPEN_DIALOG_CAM(hWindCamera,1,"Ccd Settings");
			OPEN_DIALOG_BW(hWindBW,1,"Gray Level");
			*/
			sprintf(Message,"\nCcd SENSICAM is no longer supported in the present software version\n");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			return -1;
			
		case QIMAGING:
			/*
			QImaging.error = QCam_LoadDriver();
			if (QImaging.error != qerrSuccess)
			return -1;
			
			QImaging.listLength = sizeof(QImaging.lista)/sizeof(QImaging.lista[0]);
			QCam_ListCameras(QImaging.lista, &QImaging.listLength);
			
			if ((QImaging.listLength>0) && (QImaging.lista[0].isOpen == 0)){			
			QImaging.error = QCam_OpenCamera(QImaging.lista[0].cameraId, &QImaging.handle);
			if ((QImaging.error != qerrSuccess) | (QImaging.handle==NULL)){
			QCam_ReleaseDriver();
			return -1;
			}		
			}
			else if (QImaging.listLength==0){
			QCam_ReleaseDriver();
			return -1;
			}
			SetCcdDefaultSettings(ccdType);   
			*/	   
			sprintf(Message,"\nCcd RETIGA 2000 is no longer supported in the present software version\n");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			return -1;
			
		case R6: 
			if (pl_pvcam_init() == FALSE){	  
				checkR6Error();
				return -1;
			}
			if (pl_cam_get_name(0, ccdR6.name) == FALSE) {
				checkR6Error();
				return -1;
			}
			//sprintf(Message,"\nCamera is called %s\n", ccdR6.name);
			//SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			if (pl_cam_open(ccdR6.name, &ccdR6.handle, OPEN_EXCLUSIVE) == FALSE) {
				checkR6Error();		
				return -1;
			}  
		
			if (pl_create_frame_info_struct(&ccd_pFrameInfo) == FALSE) {
				checkR6Error();	   
				// Release frame info
				QuitCCD(ccdType);
				return -1;
			}
			
			GetParamValue(ccdR6.handle,PARAM_SER_SIZE,&ccdR6.sensorResX);
			GetParamValue(ccdR6.handle,PARAM_PAR_SIZE,&ccdR6.sensorResY);

			ccdRoi.p1=(uns16)(0);  
			ccdRoi.p2=(uns16)(ccdR6.sensorResX-1);
			ccdRoi.pbin=(uns16)1;    
			ccdRoi.s1=(uns16)(0);  
			ccdRoi.s2=(uns16)(ccdR6.sensorResY-1);				   
			ccdRoi.sbin=(uns16)1;
			Ccd.Exposure=ZERO_EXPOSURE;
			uns32 exposureBytes;
			if (pl_exp_setup_seq(ccdR6.handle, 1, 1, &ccdRoi, TIMED_MODE, Ccd.Exposure, &exposureBytes)!= PV_OK) {
				//sprintf(Message,"\nError in sending parameter to camera - DON'T WORRY ABOUT IT\n");
				//SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				//checkR6Error();
			} 
			return 1;
	}
	return -1;
}
	

  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//
// Close CCD
int QuitCCD(int ccdType){
	
	
	if (ccdType==SENSI){
		/*
		STOP_COC(0);
		CLOSE_DIALOG_CAM();
		CLOSE_DIALOG_BW();						
		SET_INIT(0);
		*/
	}
	else if(ccdType==QIMAGING){
		/*
		QImaging.settings.size = sizeof(QImaging.settings);
		QCam_ReadDefaultSettings( QImaging.handle, &QImaging.settings);	
		QCam_SendSettingsToCam(QImaging.handle,&QImaging.settings);

		QCam_CloseCamera(QImaging.handle);
		QCam_ReleaseDriver();
		*/

	}
	else if (ccdType == R6){
		if (ccd_pFrameInfo != NULL)   
			pl_release_frame_info_struct(ccd_pFrameInfo);
		if (pl_pvcam_uninit()== FALSE){
			checkR6Error();
			return -1;
		} 
	}
	return 1;
}


  //**************************************************************************************//

// Get all settings for Ccd; aggiorna la struttura Ccd
void GetCcdSettings(int ccdType){

	//char exptime[20];
	//char delay[20];
	//int mode;
	//int trigger;
	//char table[200];  


	switch (ccdType){
		case SENSI:
			/*
			GET_SETTINGS(&mode,&trigger,&Ccd.Roix1,&Ccd.Roix2,&Ccd.Roiy1,&Ccd.Roiy2,&Ccd.Binningx,&Ccd.Binningy,(char**) table);		
			sscanf(table,"%[^,],%[^,]",delay,exptime);
			Ccd.Exposure=atoi(exptime); 
			GET_IMAGE_SIZE(&Ccd.RowLength,&Ccd.ColLength);	
			*/
		break;
		case R6:
			/*char valueDescription[STRLEN]; 
			sprintf(paramName,"Horizontal binning");
			GetEnumParamValue(PARAM_BINNING_SER,paramName,ccdR6.handle,valueDescription);
			sprintf(Message,"\nCamera %s = %s\n", paramName,valueDescription);
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			
			sprintf(paramName,"Vertical binning");
			GetEnumParamValue(PARAM_BINNING_PAR,paramName,ccdR6.handle,valueDescription);
			sprintf(Message,"\nCamera %s = %s\n", paramName,valueDescription);
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			*/
			break;
	}
}


//**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//
// Set all Ccd settings
void SetCcdSettings(int ccdType){

	//int mode;
	//int trigger;
	//char table[200];
	//int roiW, roiH;
	//UNSIGNED64 exposure_ns;


	switch (ccdType){
		case SENSI:
		/*
		GET_SETTINGS(&mode,&trigger,&Ccd.Roix1,&Ccd.Roix2,&Ccd.Roiy1,&Ccd.Roiy2,&Ccd.Binningx,&Ccd.Binningy,(char**) table);		
		sprintf(table,"0,%d,-1,-1",Ccd.Exposure);
		SET_COC(mode,trigger,Ccd.Roix1,Ccd.Roix2,Ccd.Roiy1,Ccd.Roiy2,Ccd.Binningx,Ccd.Binningy,table);
		*/		
		break;
		case QIMAGING:
			/*
		QImaging.settings.size = sizeof(QImaging.settings);	

		QCam_SetParam(&QImaging.settings,qprmBinning,Ccd.Binningx);	
		roiW=Ccd.Roix2-Ccd.Roix1;
		QCam_SetParam(&QImaging.settings,qprmRoiX,Ccd.Roix1);
		QCam_SetParam(&QImaging.settings,qprmRoiWidth,roiW);
		roiH=Ccd.Roiy2-Ccd.Roiy1;
		QCam_SetParam(&QImaging.settings,qprmRoiY,Ccd.Roiy1);
		QCam_SetParam(&QImaging.settings,qprmRoiHeight,roiH);		
		exposure_ns=(UNSIGNED64)(Ccd.Exposure)*1000000;
		QCam_SetParam64(&QImaging.settings,qprm64Exposure,exposure_ns);			
		QCam_SendSettingsToCam(QImaging.handle,&QImaging.settings);	
		*/
		break; 
		case R6:   
			ccdRoi.p1=(uns16)(Ccd.Roiy1-1);  
			ccdRoi.p2=(uns16)(Ccd.Roiy2-1);
			ccdRoi.pbin=(uns16)(Ccd.Binningy);
			ccdRoi.s1=(uns16)(Ccd.Roix1-1);  
			ccdRoi.s2=(uns16)(Ccd.Roix2-1);				   
			ccdRoi.sbin=(uns16)(Ccd.Binningx);
			break;
	}
}

 //**************************************************************************************//

int SetCcdDefaultSettings(int ccdType){
	

	if (ccdType==QIMAGING){	
		/*
		QImaging.settings.size = sizeof(QImaging.settings);
		QCam_ReadDefaultSettings( QImaging.handle, &QImaging.settings);

		QCam_SetParam(&QImaging.settings,qprmReadoutSpeed, qcReadout20M);
		QCam_SetParam(&QImaging.settings,qprmCoolerActive, 1);
		QCam_SetParam(&QImaging.settings,qprmImageFormat, qfmtMono16);		
		QCam_SendSettingsToCam(QImaging.handle,&QImaging.settings);
		*/
	}
	return 1;

}
  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//

// Set TimeExposure
int SetTimeExpM(int ccdType){

	//char table[200];
	//int mode, trigger;
	//UNSIGNED64 exposure_ns;

	switch (ccdType) {
		case SENSI:
		/*
		GET_SETTINGS(&mode,&trigger,&Ccd.Roix1,&Ccd.Roix2,&Ccd.Roiy1,&Ccd.Roiy2,&Ccd.Binningx,&Ccd.Binningy,(char**) table);		
		sprintf(table,"0,%d,-1,-1",Ccd.Exposure);
		SET_COC(mode,trigger,Ccd.Roix1,Ccd.Roix2,Ccd.Roiy1,Ccd.Roiy2,Ccd.Binningx,Ccd.Binningy,table);	
		*/
		break;
	
		case QIMAGING:
		/*
		QImaging.settings.size = sizeof(QImaging.settings);
		exposure_ns=(UNSIGNED64)(Ccd.Exposure)*1000000;
		QCam_SetParam64(&QImaging.settings,qprm64Exposure,exposure_ns);
		QCam_SendSettingsToCam(QImaging.handle,&QImaging.settings);
		*/
		break;
		case R6:
			int16 status=0;
			uns32 exposureBytes;
			pl_exp_check_status(ccdR6.handle, &status, &exposureBytes);
			if (status!=READOUT_NOT_ACTIVE)	{
				pl_exp_stop_cont(ccdR6.handle, CCS_CLEAR);
				if (pl_exp_setup_cont(ccdR6.handle, 1, &ccdRoi, TIMED_MODE, Ccd.Exposure, &exposureBytes, CIRC_OVERWRITE)!= PV_OK) {
						sprintf(Message,"\nError in sending parameter to camera\n");
						SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
						checkR6Error();
				} 
				if (PV_OK != pl_exp_start_cont(ccdR6.handle, buffer12, exposureBytes))  {
					checkR6Error();
					break;
				}
			}
			
			break;
	}
	return 1;
}
  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//


// Get Ccd status: ritorna il valore di status
int GetCcdStatus(int ccdType){

	//int status;
	int done = 1;

	switch (ccdType){	
		case SENSI:
			/*	
			GET_IMAGE_STATUS(&status);
			if(((status&1)|(status&2))!=0)
			done=0;
			else 
			done=1;
			*/
			break;
		case QIMAGING:
			done=1;
			
			break; 
		case R6:	  
			done=1;
			break;
	}
	return done;
}
//**************************************************************************************//
//**************************************************************************************//
// Set Sequential mode for SensiCam Ccd
void SetSeqMode (int ccdType){
	//int mode;
	//int trigger;
	//char table[200];

	if (ccdType==SENSI){
		/*
		GET_SETTINGS(&mode,&trigger,&Ccd.Roix1,&Ccd.Roix2,&Ccd.Roiy1,&Ccd.Roiy2,&Ccd.Binningx,&Ccd.Binningy,(char**) table);			
		mode=SEQ_MODE;
		SET_COC(mode,trigger,Ccd.Roix1,Ccd.Roix2,Ccd.Roiy1,Ccd.Roiy2,Ccd.Binningx,Ccd.Binningy,table);
		*/
	}
}

  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//
// Set Simultaneous mode for SensiCam Ccd
void ResetSimMode(int ccdType){

	//int mode;
	//int trigger;
	//char table[200];

	if (ccdType==SENSI){
		/*
		GET_SETTINGS(&mode,&trigger,&Ccd.Roix1,&Ccd.Roix2,&Ccd.Roiy1,&Ccd.Roiy2,&Ccd.Binningx,&Ccd.Binningy,(char**)table); 		
		mode=SIM_MODE;
		SET_COC(mode,trigger,Ccd.Roix1,Ccd.Roix2,Ccd.Roiy1,Ccd.Roiy2,Ccd.Binningx,Ccd.Binningy,table);
		*/
	}
}

//**************************************************************************************//

void GetImageSize(int ccdType,int *w,int *h){
	
	//unsigned long width;
	//unsigned long height;

	switch (ccdType){
		case SENSI:
		/*
		GET_IMAGE_SIZE(w,h); 
		*/
			break;
		case QIMAGING:
		/*
		QCam_GetInfo(QImaging.handle,qinfImageWidth,&width);
		QCam_GetInfo(QImaging.handle,qinfImageHeight,&height);
		*w=(int)width;
		*h=(int)height;*/
			break;
		case R6:
			*w=(int)((Ccd.Roix2-Ccd.Roix1+1)/Ccd.Binningx);
			*h=(int)((Ccd.Roiy2-Ccd.Roiy1+1)/Ccd.Binningy);
			break;
			
	}
}

//**************************************************************************************//
// get data from Ccd
void ReadCcd(int ccdType, uns16 *b){

	//QCam_Frame frame;
	//int width, height;

	switch (ccdType){
		case SENSI:
		/*
		GET_IMAGE_SIZE(&width,&height); 
		READ_IMAGE_12BIT(0,width,height,b);
		*/
		break;
		case QIMAGING:
			
		/*
		frame.pBuffer=b;
		frame.bufferSize=Ccd.RowLength*Ccd.ColLength*sizeof(uns32);	
		QImaging.error=QCam_GrabFrame(QImaging.handle,&frame);
		*/
		break;
		case R6:
			uns32 exposureBytes;
			int16 status = 0;
			uns32 buffer_cnt = 0;
			do{  
				pl_exp_check_cont_status(ccdR6.handle, &status, &exposureBytes,&buffer_cnt);
			}
			while (status!=FRAME_AVAILABLE);
			pl_exp_get_latest_frame(ccdR6.handle, (void **)&b);
			break;
	}
}


  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//

void GrabCcdImage(int ccdType, int mode, uns16 *b){

	//int status;
	//int width, height;
	//QCam_Frame frame;
	//QCam_Frame frame_bis;		

	
	switch (ccdType){
		case SENSI:
		/*
		RUN_COC(mode);
		do{
			GET_IMAGE_STATUS(&status);
		}
		while (((status&1)|(status&2))!=0);
		GET_IMAGE_SIZE(&width,&height); 		
		READ_IMAGE_12BIT(0,width,height,b);
		*/
		break;
		case QIMAGING:
		/*
		frame.pBuffer=b;
		frame.bufferSize=Ccd.RowLength*Ccd.ColLength*sizeof(uns32);	
		QImaging.error=QCam_GrabFrame(QImaging.handle,&frame);				
		*/
		break;
		case R6:					 
			uns32 exposureBytes;
			int16 status = 0;
			switch (mode){
				case SINGLE_SHOT:   
				
					if (pl_exp_setup_seq(ccdR6.handle, 1, 1, &ccdRoi, TIMED_MODE, Ccd.Exposure, &exposureBytes)!= PV_OK) {
						sprintf(Message,"\nError in sending parameter to camera\n");
						SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
						checkR6Error();
					} 
					
					if (PV_OK != pl_exp_start_seq(ccdR6.handle, b))  {
						checkR6Error();
						break;
					} 
					//sprintf(Message,"\nAcquisition has started successfully\n");
					//SetCtrlVal(console,CONSOLE_TEXTBOX,Message);   
					do{  
						pl_exp_check_status(ccdR6.handle, &status, &exposureBytes);
					}
					while (status!=READOUT_COMPLETE);
					pl_exp_finish_seq( ccdR6.handle, b, 0);
					break;
				case CONTINUOS: 
					if (pl_exp_setup_cont(ccdR6.handle, 1, &ccdRoi, TIMED_MODE, Ccd.Exposure, &exposureBytes, CIRC_OVERWRITE)!= PV_OK) {
						sprintf(Message,"\nError in sending parameter to camera\n");
						SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
						checkR6Error();
					} 
					if (PV_OK != pl_exp_start_cont(ccdR6.handle, b, exposureBytes))  {
						checkR6Error();
						break;
					}
					//sprintf(Message,"\nAcquisition has started successful\n");
					//SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
					
					uns32 buffer_cnt = 0;
					do{  
						pl_exp_check_cont_status(ccdR6.handle, &status, &exposureBytes,&buffer_cnt);
					}
					while (status!=FRAME_AVAILABLE);
					pl_exp_get_latest_frame(ccdR6.handle, (void **)&b);
					break;
			}
			break;								 
	}
}


//**************************************************************************************//
//**************************************************************************************//
//**************************************************************************************//

// stop ccd exposition
void StopCcd(int ccdType, int mode){
	/*
	if (ccdType==SENSI)		
		STOP_COC(0);		
	else if (ccdType==QIMAGING){
		//QCam_Abort(QImaging.handle);
		//QImaging.error= QCam_SetStreaming(QImaging.handle,0);
	}
	*/
	switch (mode){
		case CONTINUOS:
			pl_exp_stop_cont(ccdR6.handle, CCS_CLEAR);
			break;
		case SINGLE_SHOT:
			break;
	}
}
//**************************************************************************************//
//**************************************************************************************//
//**************************************************************************************//


  

/* ######################################################### */
/* ###													 ### */
/* ###				   GESTIONE IMAGE INTENSIFIER		 ### */ 
/* ###													 ### */
/* ######################################################### */
// Initialize HRI
int InitIntensifier(void){	

	int status;	
	char command[LENCOMMAND];
	char response[LENCOMMAND];

	
	Com.Num=COMNUM;
	// apre comunicazione con usb
	status=OpenComConfig(Com.Num,COMNAME,BAUDRATE, 0,8,1,LENCOMMAND,LENCOMMAND);		
	if (status>=0){	
		
		status=SetCTSMode (Com.Num, LWRS_HWHANDSHAKE_CTS_RTS_DTR);
		status=SetComTime (Com.Num, (double)TIMEOUT);

		sprintf(command,"[ST%d]\r",4);
		WriteIntensifier(command);						 
		ReadIntensifier(response,command,0);		

		// Gate On
		Gate.On=1;
		sprintf(command,"[GO%d]\r",Gate.On);
		WriteIntensifier(command);
		ReadIntensifier(response,command,1);		


		// gain to ZERO_GAIN
		sprintf(command,"[MG%d]\r",ZERO_GAIN);
		WriteIntensifier(command);
		ReadIntensifier(response,command,1);		
		
	}
	return (status);
}

void QuitIntensifier(void){ 
	int result;
	Actual.Gain=SetGainM(ZERO_GAIN);
	result=CloseCom(Com.Num);

}


/*'WriteHri' function receives a string and sends it to HRI*/
void WriteIntensifier(char *StringCommand) {
	
	int nbytes;

	nbytes=ComWrt(Com.Num,StringCommand,strlen(StringCommand));
	Wait(WAIT_TIME_MS);
	return;
}

void ReadIntensifier(char *StringResponse, char *StringCommand, int echo) {
	
	int nbytes, status;

	nbytes = ComRdTerm (Com.Num, StringResponse, LENCOMMAND, CR);
	Wait(WAIT_TIME_MS);
	if (echo==1){	
		status=strncmp(StringResponse,StringCommand,nbytes);
		if (status!=0)
			MessagePopup ("Error", "Communication error with Hamamatsu image intensifier");
	}
	return;
}

void UpdateIntensifierM(void){	

	char command[LENCOMMAND];
	char response[LENCOMMAND];
	char cmd[LENCOMMAND];
	int triggerWidth = 0;


	if (Gate.On){
		// CW or normal gate or MCP gated 
		//CW mode
		if (Gate.McpGateOn==0) {
			sprintf(command,"[GM%d]\r",0);
			WriteIntensifier(command);
			ReadIntensifier(response,command,1);		
		}
		else {
			//normal mode
			if (Gate.McpGateOn==1) {			
				triggerWidth=Gate.Width;			
			}
			//MCP gated mode
			else if (Gate.McpGateOn==2) {				
				triggerWidth=Gate.Width+OVERLENGTH_GATE;
			}

			// GateWidth	
			if (Hardware.DelayType==DEL150){
				/*
				DEL_set_output_pulse_width(del150.hndl,(float)triggerWidth);
				Actual.Gate=Gate.Width;
				*/
			}
			else if (Hardware.DelayType==STANFORD){
				sprintf(cmd,"DT 3,2,%E",triggerWidth*NSECTOSEC);
				ibwrt(stanford.hndl,cmd,strlen(cmd));
				Actual.Gate=Gate.Width;
			}

			//normal mode
			if (Gate.McpGateOn==1) {
				sprintf(command,"[GM%d]\r",1);
				WriteIntensifier(command);
				ReadIntensifier(response,command,1);		
				sprintf(command,"[DA%d]\r",0);
				WriteIntensifier(command);
				ReadIntensifier(response,command,1);		
			}
			//MCP gated mode
			else if (Gate.McpGateOn==2) {
				sprintf(command,"[GM%d]\r",1);
				WriteIntensifier(command);
				ReadIntensifier(response,command,1);		
				
				sprintf(command,"[DA%d]\r",1);
				WriteIntensifier(command);
				ReadIntensifier(response,command,1);		
			}			
		}
	}
}

int CVICALLBACK GetIntensifierStatus (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char command[LENCOMMAND];
	char response[LENCOMMAND];
	
	switch (event)
	{
		case EVENT_COMMIT:		 
			sprintf(command,"[ST%d]\r",1);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);
			
			sprintf(command,"[ST%d]\r",2);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);
			
			sprintf(command,"[ST%d]\r",3);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);
			
			sprintf(command,"[ST%d]\r",4);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);
			
			sprintf(command,"[ST%d]\r",5);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);
			
			sprintf(command,"[ST%d]\r",6);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);
			
			sprintf(command,"[ST%d]\r",7);
			WriteIntensifier(command);						
			ReadIntensifier(response,command,0);

			break;
	}
	return 0;
}



  //**************************************************************************************//

int SetGainM (int CurrentGain)
 {
	char command[LENCOMMAND];
	char response[LENCOMMAND];	
	int realGain;

	/*Control on maximum value for gain*/
	if(CurrentGain>MAX_GAIN){ 
		CurrentGain=ZERO_GAIN;
		sprintf(Message,"\nError: gain exceed maximum value\nGain set to %d\n",ZERO_GAIN);
		SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
	}
	
	sprintf(command,"[MG%d]\r",CurrentGain);
	WriteIntensifier(command);
	ReadIntensifier(response,command,1);		

	VerifyGain(CurrentGain,&realGain);


	sprintf(Message,"\nGain set to %d... real value %d",CurrentGain,realGain);	
	SetCtrlVal(console,CONSOLE_TEXTBOX,Message);

	SetCtrlVal(measure,MEASURE_GAIN,CurrentGain);
	SetCtrlVal(smallpanel,SMALLPANEL_GAIN_SMALL,CurrentGain);

	return (CurrentGain);
}


void VerifyGain(int valueSet, int *valueRead){

	char command[LENCOMMAND];
	char response[LENCOMMAND];	
	char s_read[LENGAIN+1];
	int diff;

	do{
		sprintf(command,"[ST2]\r");
		WriteIntensifier(command);
		ReadIntensifier(response,command,0);		
			
		CopyString (s_read,0,response,7,3);
		Scan (s_read, "%s>%i", valueRead);
		if (valueSet>=*valueRead)
			diff=(valueSet-*valueRead);
		else
			diff=(*valueRead-valueSet);
	}
	while (diff>=THR_DIFF_GAIN);
}


int GetRealGain(void){

	int valueRead;
	char command[LENCOMMAND];
	char response[LENCOMMAND];	
	char s_read[LENGAIN];

	sprintf(command,"[ST2]\r");
	WriteIntensifier(command);
	ReadIntensifier(response,command,0);		
	
	CopyString (s_read,0,response,7,3);
	Scan (s_read, "%s>%i", &valueRead);

	sprintf(Message,"\nActual.Gain: %d\n",valueRead);	
	SetCtrlVal(console,CONSOLE_TEXTBOX,Message);

	return valueRead;
	
}



	/* WAIT */
void Wait(clock_t pause){
	clock_t cstart;
	clock_t numClocks_pause;
	
	
	numClocks_pause=(clock_t)(pause*CLOCKS_PER_SEC/1000); 
	// Wait for the gain to be settled
	cstart = clock();  /* Use clock for timing to hundredths of seconds */
	while ((clock()-cstart)<numClocks_pause);
	return;
}


  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//

/* ######################################################### */
/* ###													 ### */
/* ###				   GESTIONE DELAY GENERATOR 		 ### */ 
/* ###													 ### */
/* ######################################################### */


int InitDelayGenerator(int delayType){

	int status = -1;
	if (delayType==DEL150){
		/*
		status=InitDel150();		
		*/
		status = -1;
		sprintf(Message,"\nDelay generator DEL150 is no longer supported in the present software version\n");
		SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
	}
	else if (delayType==STANFORD){
		status=InitStanford();
	}
	return status;
}



int InitDel150(void) {
	/*
	short error;
	
	error=DEL_init("c:\\DelDll32\\del150.ini",&del150.hndl);	

	if (error<0)
		return -1;
	else{
		// output disabled;
		DEL_set_slope(del150.hndl,0);
		return 1;	
	}
	*/
	return -1;
}

short UpdateD150M(void){
	/*
	short t;

	DEL_set_slope(del150.hndl,del150.triggerSlope);
	DEL_set_threshold(del150.hndl,fdel150.triggerLevel); 	
	DEL_test_if_triggered(del150.hndl, &t); 
	SetCtrlVal(d150panel,D150PANEL_TRIGGERLED,0);	
	DEL_set_delay_range(del150.hndl,del150.range);
	DEL_invert_output_pulse(del150.hndl,del150.invertedOutput); 

	// Set output al valore di range/10;
	DEL_set_output_pulse_width(del150.hndl,del150.range/10);
	return t;
	*/
	return -1;
} 

int InitStanford(void) {

	char cmd[STRLEN];
	//char name[STRLEN];

	// Open Gpib Device for address ADDRDELAY
	//stanford.hndl = ibfind (name);
	stanford.hndl = ibdev (GPIB0, ADDRDELAY, NO_SAD, T100ms, 1, 0);
	if (stanford.hndl<0)
		return -1;
	
	// Initialize delay generator D535
	 ibwrt(stanford.hndl,"CL",2L);
	sprintf(cmd,"DS INITIALIZE_OK");
	ibwrt(stanford.hndl,cmd,strlen(cmd));
	
	UpdateStanfordM();
	
	return 1;

	
}

void UpdateStanfordM(void){
		char cmd[STRLEN];

		if (stanford.triggerExternal==1){
			// Set trigger external
			ibwrt(stanford.hndl,"TM 1",4L);
			
			// Set trigger level
			sprintf(cmd,"TL %f",stanford.triggerLevel);
			ibwrt(stanford.hndl,cmd,strlen(cmd));
			// Set trigger slope
			if (stanford.triggerSlope==1)
				ibwrt(stanford.hndl,"TS 1",4L);
			else
				ibwrt(stanford.hndl,"TS 0",4L); 		
			// Set trigger input impedance Low Z
			ibwrt(stanford.hndl,"TZ 0,0",6L);
			
		}
		else {
			// Set trigger internal
			ibwrt(stanford.hndl,"TM 0",4L);
			// Set trigger rep. rate to 50 Hz
			sprintf(cmd,"TR 0,%f",50.0);
			ibwrt(stanford.hndl,cmd,strlen(cmd));
		}		
		// Set delay AB Low Imped.
		ibwrt(stanford.hndl,"TZ 4,0",6L);
		
		// Set output level to 3 Volts
		ibwrt(stanford.hndl,"OM 4,3; OA 4,3; OO 4,0",22L);
		
		// Set output a 100 ns;
		sprintf(cmd,"DT 3,2,%E",100*NSECTOSEC);
		ibwrt(stanford.hndl,cmd,strlen(cmd));
		sprintf(cmd,"DS SETTINGS_OK");
		ibwrt(stanford.hndl,cmd,strlen(cmd));
}

int SetDelayM (int delayType, int d, unsigned short int timescale){

	char cmd[STRLEN];
	char codeTimescale[3];
	double dn=0;

	switch (delayType){
		/*
		case DEL150:
			DEL_set_delay(del150.hndl,(float)d); // è già in ns per Del150			
			break;
			*/

		case STANFORD:
			switch (timescale){
				case (1): //NANOSECOND  
					dn=d*NSECTOSEC;
					sprintf(codeTimescale,"NS");
					break;
				case (2): //PICOSECOND  
					dn=d*PSECTOSEC; 
					sprintf(codeTimescale,"PS");
					
					break;
				case (3): //MICROSECOND  
					dn=d*MUSECTOSEC; 
					sprintf(codeTimescale,"US");
					break;
			}
			
			sprintf(cmd,"DT 2,1,%E",dn);
			ibwrt(stanford.hndl,cmd,strlen(cmd));
			sprintf(cmd,"DS DELAY_%d_%s",d,codeTimescale);
			ibwrt(stanford.hndl,cmd,strlen(cmd));
			break;				
	}

	SetCtrlVal(measure,MEASURE_DELAY,d);
	SetCtrlVal(smallpanel,SMALLPANEL_DELAY_SMALL,d);
	return d;
}

void QuitDelayGenerator(int delayType){ 
	Actual.Delay=SetDelayM(delayType,ZERO_DELAY,Delays.timescale);
}

 

/* ######################################################### */
/* ###													 ### */
/* ###				   GESTIONE BUFFER					 ### */ 
/* ###													 ### */
/* ######################################################### */
// AllocBuffer = Buffer Allocation (Normal buffers and MIL buffers)
// FreeBuf = deallocate normal buffers
// MFreeBuf = deallocate MIL buffers

void AllocBuffer(int ccdType){

	int width = 0;
	int height = 0;

	switch (ccdType) {   
		case SENSI:
			/*	width=WIDTH_SENSI;
			height=HEIGHT_SENSI;
			*/					
			break;
		case QIMAGING:
			width=WIDTH_QIMAGING;
			height=HEIGHT_QIMAGING;
			break;
			
		case R6:
			width=WIDTH_R6;
			height=HEIGHT_R6;
			
			ccdRoi.s1 = 0; 
			ccdRoi.s2 = WIDTH_R6 - 1;  
			ccdRoi.sbin = 1;
			ccdRoi.p1 = 0;
			ccdRoi.p2 = HEIGHT_R6 - 1;   
			ccdRoi.pbin = 1;
			break;
	}

	buffer12=(uns16 *)calloc( width*height,sizeof(uns16));
	buffer12Bkg=(uns16 *)calloc( width*height,sizeof(uns16));
	displayBuffer12=(uns16 *)calloc( width*height,sizeof(uns16));	
	bufferSpectrum=(double *)calloc( width,sizeof(double));
	
	//Hist.Somma=0;
}


void QuitBuffer(void){
	FreeBuf();
}

  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//

/*Funzione per liberare i buffer (non della MIL) */
void FreeBuf(void){  
	/*Dealloco i buffer*/

	free(buffer12);
	free(buffer12Bkg);
	free(displayBuffer12);
	free(bufferSpectrum);	

	return;
}

void RotateBuffer(uns16 *b12,int w, int h){

	uns16 temp;
	int i,j;
	for (i=0;i<h;i++){
		for (j=0;j<w/2;j++){
			temp=*(b12+j+i*w);
			*(b12+j+i*w)=*(b12+w-1-j+i*w);
			*(b12+w-1-j+i*w)=temp;
		}
	}

	for (i=0;i<h/2;i++){
		for (j=0;j<w;j++){
			temp=*(b12+j+i*w);
			*(b12+j+i*w)=*(b12+j+(h-i-1)*w);
			*(b12+j+(h-i-1)*w)=temp;
		}
	}
	

}

  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//


/* ######################################################### */
/* ###													 ### */
/* ###				   GESTIONE UIR 					 ### */ 
/* ###													 ### */
/* ######################################################### */


void ActivateIntensifierButtons(void){

	SetCtrlAttribute(measure,MEASURE_GATE_WIDTH,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_MCP_GATED,ATTR_DIMMED,0);	
	SetCtrlAttribute(measure,MEASURE_GAIN,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_SET_GAIN,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_GAIN_INIT,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_GAIN_TYPE,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_UPDATE_INTENS,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_GET_INTENS_STATUS,ATTR_DIMMED,0);

	

	SetCtrlAttribute(measure,MEASURE_DISPLAY_MODE,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_FIRST,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_STEP,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_LAST,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_ADDITIONAL_SCAN,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_FIRST_2,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_STEP_2,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_LAST_2,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_FIRST_3,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_STEP_3,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_LAST_3,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_AUTOVALUEFILE,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_BKG_SCAN,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_BKG_FIRST,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_BKG_STEP,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_DELAY_BKG_LAST,ATTR_DIMMED,0);
}


  //**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//

void ActivateDELButtons(void){
	SetCtrlAttribute(measure,MEASURE_DELAY,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_TIMESCALE,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_SET_DELAY,ATTR_DIMMED,0);
}

//**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************
void ActivateCCDButtons(ccdType){
	unsigned int x2 = 0;
	unsigned int y2 = 0;
	
	SetCtrlAttribute(measure,MEASURE_ROI_X1,ATTR_DIMMED,0);   
	SetCtrlAttribute(measure,MEASURE_ROI_X2,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_ROI_Y1,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_ROI_Y2,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_BINNING,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_UPDATE_CCD,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_CCD_EXPOSURE,ATTR_DIMMED,0);
	SetCtrlVal(measure,MEASURE_ROI_X1,1);
	SetCtrlVal(measure,MEASURE_ROI_Y1,1); 
	
	switch (ccdType) {
		case SENSI:
			x2= WIDTH_SENSI;
			y2= HEIGHT_SENSI; 
			break;
		case QIMAGING:
			x2= WIDTH_QIMAGING;
			y2= HEIGHT_QIMAGING; 
			break;
		case R6:
			x2= WIDTH_R6;
			y2= HEIGHT_R6; 
			break;
			
	} 
	SetCtrlVal(measure,MEASURE_ROI_X2,x2);
	SetCtrlVal(measure,MEASURE_ROI_Y2,y2);
	

}

void ActivateLaserButtons(void){
	SetCtrlAttribute(measure,MEASURE_LASER_FREQ,ATTR_DIMMED,0);
	SetCtrlAttribute(measure,MEASURE_LASER_PULSES,ATTR_DIMMED,0);
	SetCtrlAttribute(smallpanel,SMALLPANEL_LASER_PULSES,ATTR_DIMMED,0);



}


//**************************************************************************************//
  //**************************************************************************************//
  //**************************************************************************************//

void DeactivateButtons(void){

	SetCtrlAttribute(measure,MEASURE_GATE_WIDTH,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_MCP_GATED,ATTR_DIMMED,1);	
	SetCtrlAttribute(measure,MEASURE_GAIN,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_SET_GAIN,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_GAIN_INIT,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_GAIN_TYPE,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_UPDATE_INTENS,ATTR_DIMMED,1);  
	SetCtrlAttribute(measure,MEASURE_GET_INTENS_STATUS,ATTR_DIMMED,1);


	SetCtrlAttribute(measure,MEASURE_DISPLAY_MODE,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_FIRST,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_STEP,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_LAST,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_ADDITIONAL_SCAN,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_FIRST_2,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_STEP_2,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_LAST_2,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_FIRST_3,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_STEP_3,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_LAST_3,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_BKG_SCAN,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_BKG_FIRST,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_BKG_STEP,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_DELAY_BKG_LAST,ATTR_DIMMED,1);

	SetCtrlAttribute(measure,MEASURE_DELAY_AUTOVALUEFILE,ATTR_DIMMED,1);

	SetCtrlAttribute(measure,MEASURE_DELAY,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_TIMESCALE,ATTR_DIMMED,1);  
	SetCtrlAttribute(measure,MEASURE_SET_DELAY,ATTR_DIMMED,1);
	
	SetCtrlAttribute(measure,MEASURE_ROI_X1,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_ROI_X2,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_ROI_Y1,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_ROI_Y2,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_BINNING,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_UPDATE_CCD,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_CCD_EXPOSURE,ATTR_DIMMED,1);
	
	SetCtrlAttribute(measure,MEASURE_LASER_FREQ,ATTR_DIMMED,1);
	SetCtrlAttribute(measure,MEASURE_LASER_PULSES,ATTR_DIMMED,0);
	SetCtrlAttribute(smallpanel,SMALLPANEL_LASER_PULSES,ATTR_DIMMED,0);



}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//

int LoadBkgM(){

	char bkgPath[LENPATH];
	char bkgFile[FILENAME_LEN];
	FILE *fid;
	int temp;
	int gain, binx, biny, tExpos;
	int window;


	if (Hardware.Buffer==0){
		sprintf(Message,"\nError ! Buffer not allocated yet\n");
		SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
		Beep();
		return -1;
	}

	GetCtrlVal(measure,MEASURE_BKG_PATH,bkgPath);
	GetCtrlVal(measure,MEASURE_BKG_FILE,bkgFile);
	strcat(bkgPath,"\\");
	strcat(bkgPath,bkgFile);
	strcat(bkgPath,".imh");
		
	fid=fopen(bkgPath,"rb");
	if (fid==NULL){
		sprintf(Message,"\nError in opening file %s\n",bkgPath);
		SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
		return -1;
	}
	fscanf(fid,"%12d",&temp);
	fscanf(fid,"%12d",&temp);
	fscanf(fid,"%12d",&gain);	
	fscanf(fid,"%4d",&binx);
	fscanf(fid,"%4d",&biny);	
	fscanf(fid,"%8d", &Bkg.Width);
	fscanf(fid,"%8d", &Bkg.Height);
	fscanf(fid,"%12d", &tExpos);

	fseek(fid,HEADER_LEN,SEEK_SET);	
	fread(buffer12Bkg,sizeof(uns16),Bkg.Width*Bkg.Height,fid);
	fclose(fid);	
	
	sprintf(Message,"\nBkg loaded from file %s\nGain=%d\nBin=%d * %d\nTExp[ms]=%d\n",bkgPath,gain,binx,biny,tExpos);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);	 
	OpenDisplay(&window);
	CreateImage(buffer12Bkg, Bkg.Width, Bkg.Height, image);
	DisplayImage(image,&window);
	StatsCalc(buffer12Bkg,Bkg.Width,Bkg.Height);
	ShowConsole();
	MessagePopup("Close panel","Press Enter to close bkg image");	
	CloseDisplay(&window);
	//FreeImage(image);
	return 1;	
	
}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//

void LoadImgM(){
 	
	int returnCode;
	char FileName[LENPATH];
	FILE *fid;

	int delay, width, gain;
	int bx, by, rowLength, colLength,exposure;
	char comment[LENCOMMENT];
	float t1, t2;
	int t3;
	int window;


	if (Hardware.Buffer==0){
		MessagePopup("Warning","Buffers not allocated yet");
		return;
	}

	returnCode=FileSelectPopup (IMAGE_DIR,IMAGE_EXT,"", "Load Image", VAL_LOAD_BUTTON, 0, 0, 1,
					1, FileName);	
	if(returnCode==0)
		return; 		

	fid=fopen(FileName,"rb");
	if (fid==NULL){
		Beep();
		return;
	}
	fscanf(fid,"%12d",&delay);
	sprintf(Message,"\nDelay(ns) = %d",delay);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);

	fscanf(fid,"%12d",&width);
	sprintf(Message,"\nGate Width(ns) = %d",width);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);

	fscanf(fid,"%12d",&gain);
	sprintf(Message,"\nHRIGain(V) = %d",gain);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	
	fscanf(fid,"%4d",&bx);	
	fscanf(fid,"%4d",&by);
	sprintf(Message,"\nBinning = %d * %d",bx,by);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	
	fscanf(fid,"%8d",&rowLength);
	fscanf(fid,"%8d",&colLength);
	sprintf(Message,"\nImage size = %d * %d",rowLength,colLength);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	
	fscanf(fid,"%12d",&exposure);
	sprintf(Message,"\nTime exposure[ms] = %d",exposure);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	
	fscanf(fid,"%f",&t1);
	fscanf(fid,"%f",&t2);
	fscanf(fid,"%32d",&t3);
	fscanf(fid,"%s",comment);
	sprintf(Message,"\nComment=%s",comment);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	
	fseek(fid,HEADER_LEN,SEEK_SET);
	fread(displayBuffer12,sizeof(uns16),rowLength*colLength,fid);	
	fclose(fid);
	
	if ((Hardware.Intensifier==1)&(Hardware.CcdType==SENSI))
		RotateBuffer(displayBuffer12,rowLength,colLength);
	OpenDisplay(&window);
	CreateImage(displayBuffer12, rowLength, colLength, image);
	DisplayImage(image,&window);
	StatsCalc(displayBuffer12,rowLength,colLength);
	ShowConsole();
	MessagePopup("Close panel","Press Enter to close the image");	
	CloseDisplay(&window);
	//FreeImage(image);

	return;
}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//

// called by button "Single Grab" callback

int SingleGrabM(){
	int GainScelto = ZERO_GAIN;
	int j;
	int hPanel,hControl;
	uns16 *pbuffer12;
	uns16 *pbuffer12Bkg; 
	int window;
	char TimescaleString[STRLEN];


	// controlli iniziali
	if (Hardware.Ccd==0){
		MessagePopup("Error panel","Ccd not initialized yet");
		return -1;
	}

	GetImageSize(Hardware.CcdType,&Ccd.RowLength,&Ccd.ColLength);	
	if (Bkg.On){		
		if((Bkg.Height!=Ccd.ColLength)||(Bkg.Width!=Ccd.RowLength)){
			MessagePopup("Error panel","Background size not correct");
			return -1;
		}
	}

	if ((Hardware.Del==1)&(Hardware.DelayType==DEL150)&(Delays.Set>del150.range)){
		MessagePopup("Error panel","Delay grater than selected range of DEL-150");
		return -1;
	}		
	// fine controlli

	if (Hardware.Intensifier==1){
		UpdateIntensifierM(); 
		Actual.Gain=SetGainM(ZERO_GAIN);
	}
	
	if (Hardware.Del==1){
		Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Set,Delays.timescale);  
		switch (Delays.timescale)  {  
				case (1): //NANOSECOND  
					sprintf(TimescaleString,"nanosecond"); break;
				case (2): //PICOSECOND   
					sprintf(TimescaleString,"picosecond"); break;  
				case (3): //MICROSECOND	
					sprintf(TimescaleString,"microsecond"); break; 
		}	 
		sprintf(Message,"\n Delay set to: %d %s",Actual.Delay,TimescaleString);
		SetCtrlVal (console, CONSOLE_TEXTBOX, Message);	   
	}

	SetCcdSettings(Hardware.CcdType);
	SetTimeExpM(Hardware.CcdType);
	

	/*'Switch' GainType to select the gain: if AUTO, calculates the gain using 'selectgain',
	if MEMORY it selects it now, if MANUAL take the manual value.*/
	if (Hardware.Intensifier==1){  
		switch (Gain.Type){
			case AUTO_GAIN:
				sprintf(Message,"\n Gain ->AUTO\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);			
				SelectGainM(&GainScelto);			
				break;
			case AUTO_GAIN_FAST:
				sprintf(Message,"\n Gain ->AUTO\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				SelectGainM(&GainScelto);			
				break;
			case AUTO_GAIN_TEXP:
				sprintf(Message,"\n Gain & Texp ->AUTO\n");
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
				Ccd.Exposure=ZERO_EXPOSURE;
				SetTimeExpM(Hardware.CcdType);
				SelectGainM(&GainScelto);			
				SetTimeExpM(Hardware.CcdType);
				break;
			case MANUAL:
				sprintf(Message,"\n Gain->MANUAL : %d\n",Gain.Set);
				SetCtrlVal(console,CONSOLE_TEXTBOX,Message); 		
				GainScelto= Gain.Set;		
				break;
		}
		Actual.Gain=SetGainM(GainScelto);
	}

	//SetSeqMode(Hardware.CcdType);	// for SensiCam

	DisplayPanel(console);
	GetUserEvent(0,&hPanel,&hControl);	

	
	GrabCcdImage(Hardware.CcdType,SINGLE_SHOT,buffer12); // Inizia e finisce la lettura di un'immagine (start and stop)
	if (Bkg.On){
		for (j=1,pbuffer12=buffer12,pbuffer12Bkg=buffer12Bkg;j<=Ccd.RowLength*Ccd.ColLength;j++,pbuffer12++,pbuffer12Bkg++){
			if (*pbuffer12>=*pbuffer12Bkg)
				*pbuffer12=*pbuffer12-*pbuffer12Bkg;
			else
				*pbuffer12=0;
		}
		*buffer12=*pbuffer12;
	}
	if ((Hardware.Intensifier==1)&(Hardware.CcdType==SENSI))
		RotateBuffer(buffer12,Ccd.RowLength,Ccd.ColLength); 
	
	switch (Acquisition.Mode){
		case IMAGING: 
			OpenDisplay(&window);
			CreateImage(buffer12, Ccd.RowLength,Ccd.ColLength, image);
			DisplayImage(image,&window);
			break;
		case SPECTROSCOPY:
			DisplayPanel(graph);
			//SetAxisScalingMode (graph, SPECTR_PNL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 1, Ccd.RowLength); 
			CreateSpectrum(buffer12, Ccd.RowLength,Ccd.ColLength, bufferSpectrum);
			DisplaySpectrum(bufferSpectrum,Ccd.RowLength);
			break;
	}
	StatsCalc(buffer12,Ccd.RowLength,Ccd.ColLength);
	ShowConsole();	

	StopCcd(Hardware.CcdType,SINGLE_SHOT);    
	DisplayPanel(keypress);
	do {
		GetUserEvent(0,&hPanel,&hControl);	
	}
	while (hControl!=KEYPRESS_ENTER);
	HidePanel(keypress); 
	switch (Acquisition.Mode){
		case IMAGING: 
			CloseDisplay(&window);		
			break;
		case SPECTROSCOPY:
			//HidePanel(graph);  
			break;
	}
	//FreeImage(image);
	SaveImage(buffer12);
	Beep();	
	DisplayPanel(measure);	
	ResetSimMode(Hardware.CcdType);
	if (Hardware.Intensifier==1){
		Actual.Gain=SetGainM(ZERO_GAIN); 
	}
	return(1);
}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//


int ImageGrabM(void){
	
	int GainScelto = ZERO_GAIN;
	int done;
	int j;
	int hPanel,hControl;
	uns16 *pbuffer12;
	uns16 *pbuffer12Bkg;
	int window;
	char TimescaleString[STRLEN];

	// initial checks
	if (Hardware.Ccd==0){
		MessagePopup("Error panel","Ccd not initialized yet");
		return -1;
	}
	
	GetImageSize(Hardware.CcdType,&Ccd.RowLength,&Ccd.ColLength);		
	if (Bkg.On){	
		if((Bkg.Height!=Ccd.ColLength)||(Bkg.Width!=Ccd.RowLength)){
			MessagePopup("Error panel","Background size not correct");
			return -1;
		}
	}

	if ((Hardware.Del==1)&(Hardware.DelayType==DEL150)&(Delays.Set>del150.range)){
		MessagePopup("Error panel","Delay grater than selected range of DEL-150");
		return -1;
	}		
	
	if (Hardware.Intensifier==1){
		UpdateIntensifierM(); 
		Actual.Gain=SetGainM(ZERO_GAIN);
	}
	// end initial checks
	
	//set delay 
	if (Hardware.Del==1){
		Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Set,Delays.timescale);  
		switch (Delays.timescale)  {  
				case (1): //NANOSECOND  
					sprintf(TimescaleString,"nanosecond"); break;
				case (2): //PICOSECOND   
					sprintf(TimescaleString,"picosecond"); break;  
				case (3): //MICROSECOND	
					sprintf(TimescaleString,"microsecond"); break; 
		}	 
		sprintf(Message,"\n Delay set to: %d %s",Actual.Delay,TimescaleString);
		SetCtrlVal (console, CONSOLE_TEXTBOX, Message);	   
	}
	
	
	
	// set CCD parameters
	//GetCcdSettings(Hardware.CcdType);
	SetCcdSettings(Hardware.CcdType);
	SetTimeExpM(Hardware.CcdType);

	//set intensifier gain
	if (Hardware.Intensifier==1){
		switch (Gain.Type){
		case AUTO_GAIN:
			sprintf(Message,"\n Gain ->AUTO\n");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);		
			SelectGainM(&GainScelto);			
			break;
		case AUTO_GAIN_FAST:
			sprintf(Message,"\n Gain ->AUTO\n");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);		
			SelectGainM(&GainScelto);			
			break;
		case AUTO_GAIN_TEXP:
			sprintf(Message,"\n Gain & Texp ->AUTO\n");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			Ccd.Exposure=ZERO_EXPOSURE;
			//SetTimeExpM(Hardware.CcdType);
			SelectGainM(&GainScelto);			
			SetTimeExpM(Hardware.CcdType);
			break;
		case MANUAL:
			sprintf(Message,"\n Gain->MANUAL : %d\n",Gain.Set);
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message); 
			GainScelto= Gain.Set;		
			break;
		}		
		Actual.Gain=SetGainM(GainScelto);
	}

	//ResetSimMode(Hardware.CcdType); 
	//GetCcdSettings(Hardware.CcdType);
	GrabCcdImage(Hardware.CcdType,CONTINUOS,buffer12);   //start continuously 
	
	if (Bkg.On){
		for (j=1,pbuffer12=buffer12,pbuffer12Bkg=buffer12Bkg;j<=Ccd.RowLength*Ccd.ColLength;j++,pbuffer12++,pbuffer12Bkg++){
			if (*pbuffer12>=*pbuffer12Bkg)
				*pbuffer12=*pbuffer12-*pbuffer12Bkg;
			else
				*pbuffer12=0;
		}
		*buffer12=*pbuffer12;
	}
	
	if ((Hardware.Intensifier==1)&(Hardware.CcdType==SENSI))
		RotateBuffer(buffer12,Ccd.RowLength,Ccd.ColLength); 
	
	DisplayPanel(keypress);
	DisplayPanel(console);
	
	//put the collected image on the display
	switch (Acquisition.Mode){
		case IMAGING: 
			OpenDisplay(&window);
			CreateImage(buffer12, Ccd.RowLength,Ccd.ColLength, image);
			DisplayImage(image,&window);
			break;
		case SPECTROSCOPY:
			DisplayPanel(graph);
			//SetAxisScalingMode (graph, SPECTR_PNL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 1, Ccd.RowLength); 
			CreateSpectrum(buffer12, Ccd.RowLength,Ccd.ColLength, bufferSpectrum);
			DisplaySpectrum(bufferSpectrum,Ccd.RowLength);
			break;
	}
	
	StatsCalc(buffer12,Ccd.RowLength,Ccd.ColLength);
	ShowConsole();
	
	
	GetUserEvent(0,&hPanel,&hControl);	
	while (hControl!=KEYPRESS_ENTER){
		GetUserEvent(0,&hPanel,&hControl);	
		if(hControl==KEYPRESS_ENTER){
			break;
		}				
		do {							
			done=GetCcdStatus(Hardware.CcdType);  			
			if(hControl==KEYPRESS_ENTER){
				break;
			}
		}							
		while (done!=1); 				
		ReadCcd(Hardware.CcdType,buffer12); 	 //read continuously
		if (Bkg.On){
			for (j=1,pbuffer12=buffer12,pbuffer12Bkg=buffer12Bkg;j<=Ccd.RowLength*Ccd.ColLength;j++,pbuffer12++,pbuffer12Bkg++){
				if (*pbuffer12>=*pbuffer12Bkg)
					*pbuffer12=*pbuffer12-*pbuffer12Bkg;				
				else
					*pbuffer12=0;
			}
			*buffer12=*pbuffer12;		
		}
		if ((Hardware.Intensifier==1)&(Hardware.CcdType==SENSI))
			RotateBuffer(buffer12,Ccd.RowLength,Ccd.ColLength);
		
		switch (Acquisition.Mode){
		case IMAGING:
			CreateImage(buffer12, Ccd.RowLength,Ccd.ColLength, image);
			DisplayImage(image,&window);
			break;
		case SPECTROSCOPY:
			CreateSpectrum(buffer12, Ccd.RowLength,Ccd.ColLength, bufferSpectrum);
			DisplaySpectrum(bufferSpectrum,Ccd.RowLength);
			break;   
		}
		StatsCalc(buffer12,Ccd.RowLength,Ccd.ColLength);
		ShowConsole();
	}  
	
	StopCcd(Hardware.CcdType,CONTINUOS); // stop continuous grabbing
	switch (Acquisition.Mode){
		case IMAGING:
			CloseDisplay(&window);
			break;
		case SPECTROSCOPY:
			//HidePanel(graph);
			break;   
		}
	
	//GetCcdSettings(Hardware.CcdType);
	SaveImage(buffer12);
	HidePanel(keypress);
	DisplayPanel(measure);
	
	/*Set HRI to ZERO_GAIN*/
	if (Hardware.Intensifier==1){
		Actual.Gain=SetGainM(ZERO_GAIN); 
	}
	return(1);
}


// MY VERSION TO MODIFY FOR THE REAL TIME TAU FIT MADE BY CHATGPT
/*
int F2026ImageRatioGrabM(void) {
    int hPanel, hControl; 
    int done;
    int j;
    uns16 *pbuffer1;
    uns16 *pbuffer2;
    uns16 *pBkg;
    
	int Delay1;
	int Delay2;
	GetCtrlVal(measure, MEASURE_DELAY_TAU_FIT_1, &Delay1);
	GetCtrlVal(measure, MEASURE_DELAY_TAU_FIT_2, &Delay2);
	
	int Gain1;  // missing!!
	int Gain2; 
	
    // Buffers for the two states and the resulting ratio
    // Assuming buffer12 and buffer12Bkg are globally allocated as in your source
    static float *ratioBuffer = NULL;
    static uns16 *bufferImage1 = NULL;
    static uns16 *bufferImage2 = NULL;
    
    int size = Ccd.RowLength * Ccd.ColLength; // binning? 

    // Allocate internal buffers if not already done
    if (ratioBuffer == NULL) ratioBuffer = malloc(size * sizeof(float));
    if (bufferImage1 == NULL) bufferImage1 = malloc(size * sizeof(uns16));
    if (bufferImage2 == NULL) bufferImage2 = malloc(size * sizeof(uns16));

    // Initial Hardware Setup
    if (Hardware.Ccd == 0) { 
        MessagePopup("Error", "Ccd not initialized");
        return -1;
    }

    SetCcdSettings(Hardware.CcdType); 
    SetTimeExpM(Hardware.CcdType); 
    GrabCcdImage(Hardware.CcdType, CONTINUOS, buffer12); 

    DisplayPanel(keypress); 
    DisplayPanel(graph); 

    int captureStep = 0; // 0 for Image1, 1 for Image2

    while (1) {
        GetUserEvent(0, &hPanel, &hControl); 
        if (hControl == KEYPRESS_ENTER) break; 

        // 1. SET HARDWARE FOR CURRENT STEP
        if (captureStep == 0) {
            SetDelayM(Hardware.DelayType, Delay1, Delays.timescale); // Delay1 from UI 
            if (Hardware.Intensifier == 1) SetGainM(Gain1); // Gain1 from UI 
        } else {
            SetDelayM(Hardware.DelayType, Delay2, Delays.timescale); // Delay2 from UI 
            if (Hardware.Intensifier == 1) SetGainM(Gain2); // Gain2 from UI 
        }

        // 2. WAIT FOR CCD READY
        do {
            done = GetCcdStatus(Hardware.CcdType);
        } while (done != 1); 

        // 3. READ DATA INTO APPROPRIATE BUFFER
        if (captureStep == 0) {
            ReadCcd(Hardware.CcdType, bufferImage1); 
            // Apply Background if enabled 
            if (Bkg.On) {
                for(j=0; j<size; j++) {
                    bufferImage1[j] = (bufferImage1[j] >= buffer12Bkg[j]) ? (bufferImage1[j] - buffer12Bkg[j]) : 0;
                }
            }
            captureStep = 1; // Move to next delay
        } 
        else { // capture step = 1
            ReadCcd(Hardware.CcdType, bufferImage2);
            if (Bkg.On) {
                for(j=0; j<size; j++) {
                    bufferImage2[j] = (bufferImage2[j] >= buffer12Bkg[j]) ? (bufferImage2[j] - buffer12Bkg[j]) : 0;
                }
            }

            // 4. CALCULATE RATIO PIXEL-BY-PIXEL
				// actual formula for the tau calculation (here called ratioBuffer);
				
				for(int i, i<NumPixels; i++) {
					ratioBuffer[j] = (float) (Delay1-Delay2) / (float)(log(bufferImage1[j]/bufferImage2[j]))
				}
				
			
            for (j = 0; j < size; j++) {
                if (bufferImage2[j] > 0) {
                    ratioBuffer[j] = (float)bufferImage1[j] / (float)bufferImage2[j];
                } else {
                    ratioBuffer[j] = 0.0f;
                }
            }

            // 5. DISPLAY RATIO IN GRAPH
            // PlotIntensity is the standard CVI function for 2D data on a Graph control
            //PlotIntensity(graph, SPECTR_PNL_GRAPH, ratioBuffer, Ccd.RowLength, Ccd.ColLength, VAL_FLOAT, 0, 0);
			
			PlotIntensity(taupanel, TAU_PNL_GRAPH_LIFETIME, ratioBuffer, Ccd.RowLength, Ccd.ColLength, VAL_FLOAT, cMap, VAL_RED, 4, 1,0); 
			DrawColorBar(taupanel, TAU_PNL_COLORBAR, cMap, minTau, maxTau);
			
            
            captureStep = 0; // Reset for next pair
        }
        
        StatsCalc(buffer12, Ccd.RowLength, Ccd.ColLength); 
        ShowConsole(); 
    }

    // CLEANUP
    StopCcd(Hardware.CcdType, CONTINUOS); 
    if (Hardware.Intensifier == 1) SetGainM(ZERO_GAIN); 
    
    HidePanel(keypress); 
    return 1;
}

*/

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//


 int AcquireM(void){
	 
	 int temp;
	 char tempchar;
	 int GainScelto;
	 int status;
	 int numD, numDelay, numDelaySubset;
	 int cont, i;
	 double slope[NUMDELAYS_SUBSET];
	 double interpolatedGain;
	 int delayLabel;
	 int delayValueFile;
	 int window;

	 

	
	
	if ((Hardware.Intensifier==0)&(Hardware.Del==0)){
		MessagePopup("Error", "Intensifier or Delay Unit not initialized yet");
		return -1;
	}
	/***************************** CONTROLLI VARI ****************************************/
	if ((Delays.AutoValueFile==1)|(Delays.AutoValueFile==2)){
		status=ConfirmPopup("Warning","Remember: you must have entered the correct number of images to be taken\n");
		if (status==0)
			return -1;	
	}  
	
	GetImageSize(Hardware.CcdType,&Ccd.RowLength,&Ccd.ColLength);		
	if (Bkg.On){		
		if((Bkg.Height!=Ccd.ColLength)||(Bkg.Width!=Ccd.RowLength)){
			MessagePopup("Error panel","Background size not correct");
			return -1;
		}
	}
	
	  
	//***************************************************************************//
	//Create a name for the file and its path
	CreatePath();
	if (OverwritingFile()==1){
			status=ConfirmPopup("Warning","File already exist; Overwrite ?\n");
			if (status==0)
				return -1;
	}

	numDelay=0;
	// Gain values and/or delay values taken from file (1 -> only delay values
	//												    2 -> delay, TimeExp and gain values)
	if ((Delays.AutoValueFile==1)|(Delays.AutoValueFile==2)){
		File.List.FileIn=fopen(File.List.PathIn,"r");
		if(File.List.FileIn==NULL){
			sprintf(Message,"\nThe file does not exist");
			SetCtrlVal(console,CONSOLE_TEXTBOX,Message);
			return -1;
		}
		fseek( File.List.FileIn, 0L, SEEK_SET);
		GetCtrlVal(measure,MEASURE_NUMDELAY_AUTO,&Delays.NumDelaysAuto);
		for(Delays.ID=0; Delays.ID < Delays.NumDelaysAuto;Delays.ID++ ){
			if (Delays.timescale==1)	  
				fscanf( File.List.FileIn, "%5d\t", &temp);	
			else if ((Delays.timescale==2)||(Delays.timescale==3))	  
				fscanf( File.List.FileIn, "%5d%c\t", &temp,&tempchar);	
			
			if (temp<0)
				Delays.Nom[Delays.ID]=abs(temp)*1000;
			else
				Delays.Nom[Delays.ID]=temp; 
			
			fscanf( File.List.FileIn, "%3d\t", &temp);		
			if (Delays.AutoValueFile==2)
				Delays.Gain[Delays.ID]= temp; 			  
			fscanf( File.List.FileIn, "%6d\n", &temp);			
			if (Delays.AutoValueFile==2)
				Delays.TimeExp[Delays.ID]= temp;			
		}
		fclose(File.List.FileIn);
		numDelay=Delays.ID;
	}

	// delay values taken from panel (0 -> scan all delays (AUTO)			
	else if (Delays.AutoValueFile==0){	
		if (SwitchGainMeas == 0)  // OLD method: look at First, Step and Last
		{ 
			/*AUTOMATIC: it selects automatically the step delay between every two images */		  
			numDelay=((Delays.Last[0] - Delays.First[0])/Delays.Step[0])+ 1;
		
			for(Delays.ID=0; Delays.ID < numDelay;Delays.ID++ ){
				Delays.Nom[Delays.ID]=Delays.First[0] + (Delays.ID * Delays.Step[0]);
			}	 
		
			if ((Acquisition.OtherScanTst)==1){
				numD=(int)((Delays.Last[1]-Delays.First[1])/Delays.Step[1])+ 1;
				for(Delays.ID=0; Delays.ID < numD ;Delays.ID++ ){
					Delays.Nom[Delays.ID + numDelay]=Delays.First[1] + (Delays.ID * Delays.Step[1]);
				} 
				numDelay=numDelay+numD;
				if (Delays.First[2]!=0 ){
					numD=(int)((Delays.Last[2]-Delays.First[2])/Delays.Step[2])+ 1;				  
					for(Delays.ID=0; Delays.ID < numD ;Delays.ID++ ){
						Delays.Nom[Delays.ID + numDelay]=Delays.First[2] + (Delays.ID * Delays.Step[2]);
					}
					numDelay=numDelay+numD;
				}			
			}		
			if ((Acquisition.BkgScan)==1){
				numD=(int)((Delays.BkgLast-Delays.BkgFirst)/Delays.BkgStep)+ 1;
				for(Delays.ID=0; Delays.ID < numD ;Delays.ID++ ){
					Delays.Nom[Delays.ID + numDelay]=Delays.BkgFirst + (Delays.ID * Delays.BkgStep);
				} 
				numDelay=numDelay+numD;						
			}
		SetCtrlVal(measure,MEASURE_NUMDELAY_AUTO,numDelay);	
		} 
		else if (SwitchGainMeas == 1) // NEW method-> build Delays.Nom from DelaysForMeasurement
		{
			// reorder array just in case and update textbox
			ReorderArray(&DelaysForMeasurement[0], MAXDELAYSFORMEASUREMENT);
			WriteArrayToTextBox(measure, MEASURE_TEXTBOX_DELMEAS, DelaysForMeasurement, MAXDELAYSFORMEASUREMENT);
			for(int j = 0; j <MAXDELAYSFORMEASUREMENT; j++)
			{
				// exit as soon as -1 is read
				if (DelaysForMeasurement[j] == -1) break;
				numDelay++;
				Delays.Nom[j] = DelaysForMeasurement[j];
			}
			
			SetCtrlVal(measure,MEASURE_NUMDELAY_AUTO,numDelay);
					   
		}
	}
	

	//***************************************************************************//
	if ((Hardware.DelayType==DEL150)&(Delays.Nom[numDelay-1]>del150.range)){
		MessagePopup("Error panel","Delays grater than selected range of DEL-150");
		return -1;
	}
	
	//***************************************************************************//
	// gain values and Texp values calculated automatically
	HidePanel(measure);
	UpdateIntensifierM();   
	SetCcdSettings(Hardware.CcdType);	
	//GetCcdSettings(Hardware.CcdType); 
	

	// gain values and Texp values calculated automatically	
	if (((Delays.AutoValueFile==0)|(Delays.AutoValueFile==1))&(Gain.Type!=AUTO_GAIN_FAST)){			
		for(Delays.ID=0; Delays.ID<numDelay ;Delays.ID++ ){	
			if (Gain.Type==MANUAL){
				Delays.Gain[Delays.ID]=Gain.Set;			
				Delays.TimeExp[Delays.ID]=Ccd.Exposure;
			}
			else if (Gain.Type==AUTO_GAIN){
				Actual.Gain=SetGainM(ZERO_GAIN);				
				Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Nom[Delays.ID],Delays.timescale); 
				//SetTimeExpM(Hardware.CcdType);
				SelectGainM(&GainScelto);
				Delays.Gain[Delays.ID]=GainScelto;			
				Delays.TimeExp[Delays.ID]=Ccd.Exposure;
			} 
			else if (Gain.Type==AUTO_GAIN_TEXP){
				Actual.Gain=SetGainM(ZERO_GAIN);		
				Ccd.Exposure=ZERO_EXPOSURE;
				//SetTimeExpM(Hardware.CcdType); 
				Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Nom[Delays.ID],Delays.timescale); 
				SelectGainM(&GainScelto);
				Delays.Gain[Delays.ID]=GainScelto;			
				Delays.TimeExp[Delays.ID]=Ccd.Exposure;
			} 
		}
	}
	// New version below
	/*
	if (((Delays.AutoValueFile==0)|(Delays.AutoValueFile==1))&(Gain.Type==AUTO_GAIN_FAST)){
		
		numDelaySubset=0;
		for(i=0;i<NUMDELAYS_SUBSET;i++)
			slope[i]=0;

		while(Delays.Subset[numDelaySubset]!=-1)
			numDelaySubset++;

		for (i=0;i<numDelaySubset;i++){
			Actual.Gain=SetGainM(ZERO_GAIN);		
			//SetTimeExpM(Hardware.CcdType); 
			Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Subset[i],Delays.timescale); 
			SelectGainM(&GainScelto);			
			Delays.GainSubset[i]=GainScelto;			
		}

		for (cont=0; cont<(numDelaySubset-1); cont++){
			slope[cont]=(double)(Delays.GainSubset[cont+1]-Delays.GainSubset[cont])/(double)(Delays.Subset[cont+1]-Delays.Subset[cont]);
		}
		
		cont=0;
		for(Delays.ID=0; Delays.ID<numDelay ;Delays.ID++ ){
			if (Delays.Nom[Delays.ID]>Delays.Subset[cont+1])
				cont++;	
			interpolatedGain=Delays.GainSubset[cont]+slope[cont]*(Delays.Nom[Delays.ID]-Delays.Subset[cont]);
			if (fmod(interpolatedGain,1.0)>=0.5)
				Delays.Gain[Delays.ID]=(int)((interpolatedGain)+1.0-fmod(interpolatedGain,1.0));
			else
				Delays.Gain[Delays.ID]=(int)(interpolatedGain-fmod(interpolatedGain,1.0));
			Delays.TimeExp[Delays.ID]=Ccd.Exposure;		  			
		}
	}
	*/
	
	// F2026
	
	if (((Delays.AutoValueFile==0)|(Delays.AutoValueFile==1))&(Gain.Type==AUTO_GAIN_FAST))
	{
		switch(SwitchGainCalc) // check value of binary switch for gain calculation (old vs new)
		{
			case 0: // old
				numDelaySubset=0;
				for(i=0;i<NUMDELAYS_SUBSET;i++)
					slope[i]=0;

				/*	   correction below
				while(Delays.Subset[numDelaySubset]!=-1)
					numDelaySubset++;
				*/
				
				for (i=0;i<NUMDELAYS_SUBSET;i++){
					if(Delays.Subset[i]!=-1){
						numDelaySubset++;
					}
				}

				for (i=0;i<numDelaySubset;i++){
					Actual.Gain=SetGainM(ZERO_GAIN);		
					//SetTimeExpM(Hardware.CcdType); 
					Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Subset[i],Delays.timescale); 
					SelectGainM(&GainScelto);			
					Delays.GainSubset[i]=GainScelto;			
				}

				for (cont=0; cont<(numDelaySubset-1); cont++){
					slope[cont]=(double)(Delays.GainSubset[cont+1]-Delays.GainSubset[cont])/(double)(Delays.Subset[cont+1]-Delays.Subset[cont]);
				}
		
				cont=0;
				for(Delays.ID=0; Delays.ID<numDelay ;Delays.ID++ ){
					if (Delays.Nom[Delays.ID]>Delays.Subset[cont+1])
						cont++;	
					interpolatedGain=Delays.GainSubset[cont]+slope[cont]*(Delays.Nom[Delays.ID]-Delays.Subset[cont]);
					if (fmod(interpolatedGain,1.0)>=0.5)
						Delays.Gain[Delays.ID]=(int)((interpolatedGain)+1.0-fmod(interpolatedGain,1.0));
					else
			 			Delays.Gain[Delays.ID]=(int)(interpolatedGain-fmod(interpolatedGain,1.0));
					Delays.TimeExp[Delays.ID]=Ccd.Exposure;		  			
				}
			break;
			case 1: // new
				numDelaySubset=0;
				for(i=0;i<MAXDELAYSFORGAINCALC;i++)
					slope[i]=0;

				/*
				while(DelaysForGainCalc[numDelaySubset]!=-1)
					numDelaySubset++;
				*/
				
				for (i=0;i<NUMDELAYS_SUBSET;i++){
					if(Delays.Subset[i]!=-1){
						numDelaySubset++;
					}
				}
				

				for (i=0;i<numDelaySubset;i++){
					Actual.Gain=SetGainM(ZERO_GAIN);		
					//SetTimeExpM(Hardware.CcdType); 
					Actual.Delay=SetDelayM(Hardware.DelayType,DelaysForGainCalc[i],Delays.timescale); 
					SelectGainM(&GainScelto);			
					Delays.GainSubset[i]=GainScelto;			
				}

				for (cont=0; cont<(numDelaySubset-1); cont++){
					slope[cont]=(double)(Delays.GainSubset[cont+1]-Delays.GainSubset[cont])/(double)(DelaysForGainCalc[cont+1]-DelaysForGainCalc[cont]);
				}
		
				cont=0;
				for(Delays.ID=0; Delays.ID<numDelay ;Delays.ID++ ){
					if (Delays.Nom[Delays.ID]>DelaysForGainCalc[cont+1])
						cont++;	
					interpolatedGain=Delays.GainSubset[cont]+slope[cont]*(Delays.Nom[Delays.ID]-DelaysForGainCalc[cont]);
					if (fmod(interpolatedGain,1.0)>=0.5)
						Delays.Gain[Delays.ID]=(int)((interpolatedGain)+1.0-fmod(interpolatedGain,1.0));
					else
			 			Delays.Gain[Delays.ID]=(int)(interpolatedGain-fmod(interpolatedGain,1.0));
					Delays.TimeExp[Delays.ID]=Ccd.Exposure;		  			
				}
			break;
		}
				
				
	}
	 
	
	//***************************************************************************//
	/*It writes in a file the gain found*/	
	File.List.FileOut=fopen(File.List.PathOut,"w");
	fseek( File.List.FileOut, 0L, SEEK_SET);	  
	for(Delays.ID=0; Delays.ID<numDelay ;Delays.ID++ ){		
		if (Delays.Nom[Delays.ID]>MAX_LABEL_DELAY)
			delayValueFile=(int)(-(Delays.Nom[Delays.ID])/1000);		
		else
			delayValueFile=Delays.Nom[Delays.ID];   
//fprintf(File.List.FileOut,"%5d\t",delayValueFile);
		if (Delays.timescale==1) 
				fprintf(File.List.FileOut,"%5d\t",delayValueFile);	  
		if (Delays.timescale==2)
			fprintf(File.List.FileOut,"%5d%c\t",delayValueFile,'p');	
		else if (Delays.timescale==3)
			fprintf(File.List.FileOut,"%5d%c\t",delayValueFile,'u');	
		fprintf(File.List.FileOut,"%3d\t",Delays.Gain[Delays.ID]);
		fprintf(File.List.FileOut,"%6d\n",Delays.TimeExp[Delays.ID]);
		if (Delays.Nom[Delays.ID]>MAX_LABEL_DELAY){
			delayLabel=(double)((Delays.Nom[Delays.ID])/1000.0);	
			sprintf(Delays.Label[Delays.ID],"%04d%c",delayLabel,'u'); 
		}
		else{
			delayLabel=Delays.Nom[Delays.ID];
			sprintf(Delays.Label[Delays.ID],"%05d",delayLabel); 
		}
		  
		CreatePath1();
		sprintf(Message,"\n Nome ritardo: %s -> gain %d -> Texp %d", File.Image.Name[Delays.ID],Delays.Gain[Delays.ID],Delays.TimeExp[Delays.ID]);
		SetCtrlVal(console,CONSOLE_TEXTBOX,Message);			
	}   
	fclose(File.List.FileOut);
	DisplayPanel(measure);
	
	//***************************************************************************//
	//IMAGES ACQUISITION
		  
	Actual.Delay=SetDelayM(Hardware.DelayType,ZERO_DELAY,Delays.timescale);  
	Actual.Gain=SetGainM(ZERO_GAIN);
	//SetSeqMode(Hardware.CcdType); 	  
	
	status=GenericMessagePopup ("START ACQUISITION","Acquisition starting...Controllare guadagno\nPress Enter when ready","Ok","Cancel",0,
	  0,LENCOMMENT,0,VAL_GENERIC_POPUP_BTN1,VAL_GENERIC_POPUP_BTN1,VAL_GENERIC_POPUP_BTN3);
	if (status!=1){
		return -1;
	}
	
	HidePanel(measure);
	File.List.File=fopen(File.List.Path,"wt");

	switch (Acquisition.Mode){ 
		case IMAGING: 		
			OpenDisplay(&window);
			break;
		case SPECTROSCOPY:
			DisplayPanel(graph);  
			//SetAxisScalingMode (graph, SPECTR_PNL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 1, Ccd.RowLength); 
			break;
	}
	
	for(Delays.ID=0 ;Delays.ID<numDelay ;Delays.ID++ ){
		Actual.Delay=SetDelayM(Hardware.DelayType,Delays.Nom[Delays.ID],Delays.timescale); 
		Actual.Gain=SetGainM(Delays.Gain[Delays.ID]); 	  
		Ccd.Exposure=Delays.TimeExp[Delays.ID];
		//SetTimeExpM(Hardware.CcdType);	
		if (Delays.Nom[Delays.ID]>MAX_LABEL_DELAY)
			delayLabel=(int)(-(Delays.Nom[Delays.ID])/1000);		
		else
			delayLabel=Delays.Nom[Delays.ID];
		//fprintf(File.List.File,"%5d\n",delayLabel);
		if (Delays.timescale==1)
			fprintf(File.List.File,"%5d\n",delayLabel);  
		else if (Delays.timescale==2)   
			fprintf( File.List.File, "%5d%c\n",delayLabel,'p');	
		else if (Delays.timescale==3)
			fprintf( File.List.File, "%5d%c\n",delayLabel,'u');	
		
		GetImage(buffer12);	// Get image and put it in buffer12
		//GetCcdSettings(Hardware.CcdType);		
		//Actual.Gain=GetRealGain(); 
		
		switch (Acquisition.Mode){
			case IMAGING:
				CreateImage(buffer12, Ccd.RowLength,Ccd.ColLength, image);
				DisplayImage(image,&window);
				break;
			case SPECTROSCOPY:
				CreateSpectrum(buffer12, Ccd.RowLength,Ccd.ColLength, bufferSpectrum);
				DisplaySpectrum(bufferSpectrum,Ccd.RowLength);
				break;
		}
		StatsCalc(buffer12,Ccd.RowLength,Ccd.ColLength);
		ShowConsole();
		SaveAuto(buffer12);			
		Beep();
	}			
	switch (Acquisition.Mode){
		case IMAGING: 
			CloseDisplay(&window);
			//FreeImage(image); 		
			break;	  
		case SPECTROSCOPY:
			//HidePanel(graph);  
			break;
	}	
	fclose(File.List.File);
	AutoIncrementFile();
	//ResetSimMode(Hardware.CcdType);	
	Actual.Gain=SetGainM(ZERO_GAIN);
	Actual.Delay=SetDelayM(Hardware.DelayType,ZERO_DELAY,Delays.timescale); 
	DisplayPanel(measure);
	return 1;
}


void FindCcdExposure(int value_msec,int *roundValue_msec){
	double value_sec, fractionPart;
	double integerPart;

	value_sec = value_msec/1000.0;
	fractionPart=modf( (double)value_sec*Laser.Freq, &integerPart);
	*roundValue_msec=(int)(integerPart*1000/Laser.Freq);
	
	SetCtrlVal(measure,MEASURE_LASER_PULSES,(int)integerPart);
	SetCtrlVal(smallpanel,SMALLPANEL_LASER_PULSES,(int)integerPart);
}


 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//
 /* 			  FUNZIONI DI SUPPORTO PER GRAB E ACQUIRE								 */ 
 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//
void SelectGainM(int *v){

	double evalue;
	double gain;
	double factorGain =0;
	double factorTexp = 0;
	int volt;
	int in;	
	int window;


	evalue=1;
	gain=1;
	volt=ZERO_GAIN;
	
	
	GrabCcdImage(Hardware.CcdType,SINGLE_SHOT,buffer12); 

	if (Acquisition.DisplayMode==SLOW){ 
		switch (Acquisition.Mode){
			case IMAGING: 
				OpenDisplay(&window);
				CreateImage(buffer12, Ccd.RowLength,Ccd.ColLength, image);
				DisplayImage(image,&window);
				break; 
			case SPECTROSCOPY:
				DisplayPanel(graph);  
				//SetAxisScalingMode (graph, SPECTR_PNL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 1, Ccd.RowLength); 
				CreateSpectrum(buffer12, Ccd.RowLength,Ccd.ColLength, bufferSpectrum);
				DisplaySpectrum(bufferSpectrum,Ccd.RowLength);
				break;
		}
	}   
	StatsCalc(buffer12,Ccd.RowLength,Ccd.ColLength);					  
	ShowConsole();
	in=(Hist.Limit<(Gain.ThrLevel*0.9)); 
	sprintf(Message,"Starting from: Gain->: %d TimeExp->: %d\n",volt,Ccd.Exposure);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);		 
	
	while (in==1){
		if (Hist.Limit > GRAY_MIN)
			evalue=evalue*Gain.ThrLevel/Hist.Limit;
		else
			evalue=evalue*10; 

		if (Gain.Type==AUTO_GAIN_TEXP){
			FindOptimal_ExpTime_Gain(evalue,&factorGain,&factorTexp);
			FindCcdExposure((int)(ZERO_EXPOSURE*factorTexp),&Ccd.Exposure);
			SetTimeExpM(Hardware.CcdType);
		}
		else if ((Gain.Type==AUTO_GAIN)|(Gain.Type==AUTO_GAIN_FAST)){
			FindOptimal_Gain(evalue,&factorGain);		
		}

		gain=factorGain;
		volt=FindVoltage(gain);
		Actual.Gain=SetGainM(volt);

		sprintf(Message,"\nEV=%f Gain->: %d TimeExp->: %d\n",evalue,volt,Ccd.Exposure);
		SetCtrlVal (console, CONSOLE_TEXTBOX, Message);

		GrabCcdImage(Hardware.CcdType,SINGLE_SHOT,buffer12);
		if (Acquisition.DisplayMode==SLOW){
			switch (Acquisition.Mode){
				case IMAGING:
					CreateImage(buffer12, Ccd.RowLength,Ccd.ColLength, image);
					DisplayImage(image,&window);
					break;	   
				case SPECTROSCOPY:
					CreateSpectrum(buffer12, Ccd.RowLength,Ccd.ColLength, bufferSpectrum);
					DisplaySpectrum(bufferSpectrum,Ccd.RowLength);
					break;   
			}
		}								  
		StatsCalc(buffer12,Ccd.RowLength,Ccd.ColLength);
		ShowConsole();
		if (((Ccd.Exposure>=MAX_EXPOSURE)&(volt>=MAX_GAIN)&(Gain.Type==AUTO_GAIN_TEXP))|((volt>=MAX_GAIN)&((Gain.Type==AUTO_GAIN)|(Gain.Type==AUTO_GAIN_FAST))))
			in=0;
		else
			in=(Hist.Limit>(Gain.ThrLevel*1.1))|(Hist.Limit<(Gain.ThrLevel*0.9));
	}
	*v=volt;	
	sprintf(Message,"\n...values found\n Gain->: %d TimeExp->: %d\n",volt,Ccd.Exposure);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	if (Acquisition.DisplayMode==SLOW){  
		switch (Acquisition.Mode){
			case IMAGING: 
				CloseDisplay(&window);
				//FreeImage(image);
				break;
				case SPECTROSCOPY:
					//HidePanel(graph);  
					break;
		}
	}
}

/* Set the optimal Exposure time and Gain according to a predefined algorithm	*/
/* EV represents the "Exposure Value", i.e. the inverse of the light intensity	*/
/* EV=1 corresponds to Gain=Gmin=1 and Texp=T_EXP_MIN							*/
/* Slope represents the ratio between Gain change and Texp  change in log scale	*/
/* If Slope=1: Texp and Gain change keeping the same ratio						*/ 
/* If Slope=4: Gain changes 4 times faster than Texp 						 */
/* More precisely: TexpRatio=GRatio^(Slope). If G double, T changes by sqrt(2)	*/
// Gain can change from Gmin=1 and Gmax=250; Glog is the log of the Gain
// Tlog is the log of the Exposure Time

void FindOptimal_ExpTime_Gain (double ev, double *g, double *t){


	if (ev<=1){ // minimo guadagno possibile, si diminuisce il tempo di esposizione
		*g=1.0;
		*t=ev;
	}
	else if ((ev>1) & (ev<gainValues[20])){
		*t=pow(ev,(double)(1/(Gain.Slope+1)));
		*g=pow(ev,(double)(Gain.Slope/(Gain.Slope+1)));
		if ((*g)>gainValues[20]){ // massimo guadagno possibile
			*g=gainValues[20];
			*t=ev/(*g);
		}
	}
	else if (ev>=gainValues[20]){
		*g=gainValues[20];
		*t=ev/(*g);
		if ((*t)>(double)(MAX_EXPOSURE/ZERO_EXPOSURE)){ // massimo tempo di esposizione
			*g=gainValues[20];
			*t=(double)(MAX_EXPOSURE/ZERO_EXPOSURE);
		}
	}

}

void FindOptimal_Gain (double ev, double *g){


	if (ev<=1){ 
		*g=1.0;		
	}
	else if ((ev>1) & (ev<gainValues[20])){
		*g=ev;		
	}
	else if (ev>=gainValues[20]){
		*g=gainValues[20];		
	}

}


 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//
void CreateGainValues(void){
	
	int i;
	for (i=0;i<NUM_GAIN_VALUES-1;i++){
		voltageValues[i]=600+20*i;
	}
	voltageValues[NUM_GAIN_VALUES-1]=999;	

	gainValues[0]=1.00;
	gainValues[1]=1.34;
	gainValues[2]=1.88;
	gainValues[3]=2.55;
	gainValues[4]=3.49;
	gainValues[5]=4.78;
	gainValues[6]=6.42;
	gainValues[7]=8.65;
	gainValues[8]=11.56;
	gainValues[9]=15.60;
	gainValues[10]=20.72;
	gainValues[11]=27.16;
	gainValues[12]=35.43;
	gainValues[13]=46.73;
	gainValues[14]=60.41;
	gainValues[15]=78.18;
	gainValues[16]=100.61;
	gainValues[17]=128.61;
	gainValues[18]=162.51;
	gainValues[19]=209.73;
	gainValues[20]=258.58;

}


int FindVoltage(double gain){

int volt;
int i;
double slope;

i=0;
if (gain>gainValues[0]){
	while ((gain>gainValues[i])&(i<=NUM_GAIN_VALUES)){
		i=i+1;
	}
}
if (i==0)
	volt=voltageValues[0];
else
	if (i>NUM_GAIN_VALUES)
		volt=voltageValues[NUM_GAIN_VALUES-1];
	else{
		slope=(log10(gainValues[i])-log10(gainValues[i-1]))/(voltageValues[i]-voltageValues[i-1]);
		volt=voltageValues[i-1]+((int)((log10(gain)-log10(gainValues[i-1]))/slope));
	}
return volt;	
}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************//
// 'SaveIMLImage': Save image: used in GrabImageM and SingleGrabM

int SaveImage(uns16 *buffer){

	int returnCode;
	size_t numwritten;
	FILE *fid;
	char FileName[FILENAME_LEN];
	char headerBuffer[HEADER_LEN];
	char response[STRLEN];
	char emptyHeader[EMPTY_HEADER];
	int gateWidth;
	int gain;
	int delay;
	char comment[LENCOMMENT+1];

	returnCode=FileSelectPopup (IMAGE_DIR,IMAGE_EXT,"", "Save Image", VAL_SAVE_BUTTON, 0, 0, 1,
			1, FileName);	
	if(returnCode==0)
		return -1;		
	else if(returnCode==1){
		if (GenericMessagePopup ("FILE EXIST!","The file already exist", "CHANGE", "OVERWRITE",
			0, response, FILENAME_LEN, 0, VAL_GENERIC_POPUP_BTN1,VAL_GENERIC_POPUP_BTN1,
			VAL_GENERIC_POPUP_NO_CTRL)==1){
			strcpy(FileName,response);
			strcat(FileName,".iml");
		}
	}
	
	returnCode=PromptPopup ("Comment", "Insert comment:", comment ,LENCOMMENT);
	if (returnCode==0){
		strcpy(Info.Comment,comment);
		strcat(Info.Comment,";");
	}	
	memset((void*)emptyHeader,' ',EMPTY_HEADER);
	memset((void*)headerBuffer,' ',HEADER_LEN);

	fid=fopen(FileName,"wb");
	if (Hardware.Intensifier==1){		
		gain = Actual.Gain;
		gateWidth= Gate.Width;	
	}
	else{
		gain=0;
		gateWidth=0;
	}

	if (Hardware.Del==1){
		delay = Actual.Delay;		
	}
	else{
		delay = 0;
	}	
	// header da mettere a posto;
	//sprintf(headerBuffer,"%12d%12d%12d%4d%4d%8d%8d%12d%12.8f%12.8f%2d%30d;%s",delay,gateWidth,gain,Ccd.Binningx,Ccd.Binningy,Ccd.RowLength,Ccd.ColLength,Ccd.Exposure,(float)(0),(float)(0),Hardware.CcdType,emptyHeader,Info.Comment);
	sprintf(headerBuffer,"%12d%12d%12d%4d%4d%8d%8d%12d%12.8f%12.8f%2d%1d%29d;%s",Actual.Delay,Gate.Width,Actual.Gain,Ccd.Binningx,Ccd.Binningy,Ccd.RowLength,Ccd.ColLength,Ccd.Exposure,(float)(0),(float)(0),Hardware.CcdType,Delays.timescale,emptyHeader,Info.Comment);	
	numwritten=fwrite(headerBuffer,sizeof(char),HEADER_LEN,fid);
	numwritten=fwrite(buffer,sizeof(uns16),Ccd.RowLength*Ccd.ColLength,fid); 
	fclose(fid);	
	return 1;
}


 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************

/*'GetImage': Used in Acquire1*/
void GetImage(uns16 *b12){

	int j;
	uns16 *pbuffer12;
	uns16 *pbuffer12Bkg;
	int width, height;
	
	GetImageSize(Hardware.CcdType,&width,&height);
	GrabCcdImage(Hardware.CcdType,SINGLE_SHOT,b12); 
	if (Bkg.On){
		for (j=1,pbuffer12=b12,pbuffer12Bkg=buffer12Bkg;j<width*height;j++,pbuffer12++,pbuffer12Bkg++){
			if (*pbuffer12>=*pbuffer12Bkg)
				*pbuffer12=*pbuffer12-*pbuffer12Bkg;
			else
				*pbuffer12=0;
		}
		*b12=*pbuffer12;
	}	
	//StopCcd(Hardware.CcdType);
}

  

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************

//'SaveAuto': Save data; use in Acquire1 and AcquireAverage
void SaveAuto(uns16 *buffer){
	
	size_t numwritten;
	FILE *fid;
	char headerBuffer[HEADER_LEN];
	char emptyHeader[EMPTY_HEADER];

	memset((void*)emptyHeader,' ',EMPTY_HEADER);
	memset((void*)headerBuffer,' ',HEADER_LEN);

	fid=fopen(File.Image.Path[Delays.ID],"wb");
	//sprintf(headerBuffer,"%12d%12d%12d%4d%4d%8d%8d%12d%12.8f%12.8f%2d%30d;%s",Actual.Delay,Gate.Width,Actual.Gain,Ccd.Binningx,Ccd.Binningy,Ccd.RowLength,Ccd.ColLength,Ccd.Exposure,(float)(0),(float)(0),Hardware.CcdType,emptyHeader,Info.Comment);
	sprintf(headerBuffer,"%12d%12d%12d%4d%4d%8d%8d%12d%12.8f%12.8f%2d%1d%29d;%s",Actual.Delay,Gate.Width,Actual.Gain,Ccd.Binningx,Ccd.Binningy,Ccd.RowLength,Ccd.ColLength,Ccd.Exposure,(float)(0),(float)(0),Hardware.CcdType,Delays.timescale,emptyHeader,Info.Comment);	
	numwritten=fwrite(headerBuffer,sizeof(char),HEADER_LEN,fid);
	numwritten=fwrite(buffer,sizeof(uns16),Ccd.RowLength*Ccd.ColLength,fid);
	sprintf(Message,"\n File %s \n-> saved",File.Image.Path[Delays.ID]);
	SetCtrlVal (console, CONSOLE_TEXTBOX, Message);
	fclose(fid);
	return;
}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************
//Usata in Acquire1, AcquireAverage, GrabImage, GrabSingle
/* Crea il path */
void CreatePath(void){

	char scode[LENCODE];

	// create list name
	strcpy (File.List.Name, Info.Prefix);
	strcat (File.List.Name, UNDER);
	sprintf(scode,"%03d",Info.Code); 
	strcat (File.List.Name, scode);
	strcat (File.List.Name, Info.Suffix);
	strcat (File.List.Name, UNDER);
	
	//create list path
	strcpy (File.List.Path, Info.Dir);
	strcat (File.List.Path, SLASH);
	strcat (File.List.Path, File.List.Name);
	strcat (File.List.Path, EXTLIST);

	//Input File (the values to set)
	strcpy (File.List.PathIn, Info.Dir);
	strcat (File.List.PathIn, SLASH);
	strcat(File.List.PathIn,File.List.Name);
	strcat(File.List.PathIn, EXTLISTINPUT);

	//Output File (the values to set)
	strcpy (File.List.PathOut, Info.Dir);
	strcat (File.List.PathOut, SLASH);
	strcat(File.List.PathOut,File.List.Name);
	strcat(File.List.PathOut, EXTLISTOUTPUT);
	return;
}


 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************

/* Crea il nome dell'immagine e il Path*/
void CreatePath1(void){
	
	char scode[LENCODE];
	
	//create image path
	strcpy(File.Image.Name[Delays.ID],Info.Prefix);
	strcat(File.Image.Name[Delays.ID], UNDER);
	sprintf(scode,"%03d",Info.Code); 
	strcat(File.Image.Name[Delays.ID],scode);
	strcat(File.Image.Name[Delays.ID],Info.Suffix);
	strcat(File.Image.Name[Delays.ID], UNDER);
	strcat(File.Image.Name[Delays.ID],Delays.Label[Delays.ID]);
	if (Delays.timescale==2)
		strcat(File.Image.Name[Delays.ID],"p");  
	else if (Delays.timescale==3)
		strcat(File.Image.Name[Delays.ID],"u");

	
	
	strcpy(File.Image.Path[Delays.ID],Info.Dir);
	strcat(File.Image.Path[Delays.ID],SLASH);
	strcat(File.Image.Path[Delays.ID],File.Image.Name[Delays.ID]);
	strcat(File.Image.Path[Delays.ID],EXTIMG);

	return;
}


 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************

/* CALCULATE HISTOGRAM & OTHER STATs */

void StatsCalc(uns16 *buffer,int rowL, int colL){ // mettere parte di pixel saturati rossi

	long SumValues_x_Freq=0;
	int AreaSup=0;
	int ig;
	int NumSignificantPoints;
	uns16 *pbuffer12;
	int j;
	
	Hist.Somma=0;
	for(ig=0;ig<NUM_INTENSITIES;ig++){
		Hist.Vals[ig]=0;
	}
	
	for (j=1,pbuffer12=buffer;j<=Ccd.RowLength*Ccd.ColLength;j++,pbuffer12++){
		Hist.Vals[(int)(*pbuffer12)]++;
	}


	//NumSignificantPoints = Gain.ThrNumPix*colL*rowL/1000.0;

	//NumSignificantPoints = Gain.ThrNumPix;
	NumSignificantPoints = Gain.ThrNumPix*colL*rowL/100.0;

	// Average value
	for (ig=0;ig<NUM_INTENSITIES;ig++) {
		SumValues_x_Freq+=(ig+1)*Hist.Vals[ig];		
	}
	
	Hist.Mean=SumValues_x_Freq/(rowL*colL);//mean gray level
	Hist.Intensity=SumValues_x_Freq;
	
	Hist.Max=0;
	for(ig=1;ig<NUM_INTENSITIES;ig++){	
		if(Hist.Vals[ig] > Hist.Max) {			
			Hist.Peak=ig;				//Max gray level
			Hist.Max=Hist.Vals[ig];		//Corresponding counts
		}
	}
	
	/*Total intensity above threshold*/
	AreaSup=0;
	ig=NUM_INTENSITIES-1;
	do
	AreaSup+=Hist.Vals[ig--];
	while ( AreaSup < (NumSignificantPoints) && ig>0);
	Hist.Limit=ig+1;			//Limit gray level
	Hist.Somma=AreaSup;		//Corresponding counts
	
	/* Saturated pixels number (part per thousand))*/
	Hist.SatPercentage=(float)(Hist.Vals[NUM_INTENSITIES-1]/(rowL*colL)*100.0);
	Hist.NumSat=Hist.Vals[NUM_INTENSITIES-1];
	return;
}

 //**************************************************************************************//
 //**************************************************************************************//
 //**************************************************************************************

/* FUNCTION SHOW CONSOLE */ //CONTROLLARE !!!!!!!!!!!!!
void ShowConsole(void)
{
	int hCurve=-1;


	/*Scrive i valori nel pannello*/
	SetCtrlVal (console, CONSOLE_GAIN, Actual.Gain);
	SetCtrlVal (console, CONSOLE_DELAY, Actual.Delay);
	SetCtrlVal (console, CONSOLE_TEXP, Ccd.Exposure);
	SetCtrlVal (console, CONSOLE_MEAN, Hist.Mean);
	SetCtrlVal (console, CONSOLE_LIMIT, Hist.Limit);
	SetCtrlVal (console, CONSOLE_SUM, Hist.Somma);
	SetCtrlVal (console, CONSOLE_PEAK, Hist.Peak);
	SetCtrlVal (console, CONSOLE_NUMSATURATED,Hist.NumSat);
	SetCtrlVal(console,CONSOLE_TOT,Ccd.RowLength*Ccd.ColLength);
	SetCtrlVal (measure, MEASURE_GAIN_SAT,Hist.SatPercentage);

	
	if (Hist.NumSat>0) 
			SetCtrlVal(console, CONSOLE_LEDSAT, 1);
	else	
		SetCtrlVal(console, CONSOLE_LEDSAT, 0);


	DeleteGraphPlot (console, CONSOLE_GRAPH, hCurve, VAL_DELAYED_DRAW);
	hCurve = PlotY (console, CONSOLE_GRAPH, Hist.Vals,
					NUM_INTENSITIES, VAL_INTEGER, VAL_FAT_LINE,
					VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
					
}

 //**************************************************************************************//


void AutoIncrementFile(void){
	
	int code;
	
	GetCtrlVal(measure,MEASURE_INFO_CODE,&code);
	code=code+1;
	SetCtrlVal(measure,MEASURE_INFO_CODE,code);
	Info.Code=code;


}

 //**************************************************************************************//

int OverwritingFile(void) { 

	int exist;
	FILE *fid;

	fid=fopen(File.List.Path,"r");	
	if (fid==NULL)
		exist=0;
	else
		exist=1;

	return exist;
}

//******************************My addition********************************************************// 
void CVICALLBACK SaveSettingsTxt (int menuBar, int menuItem, void *callbackData,
								  int panel)
{
	
	char path[MAX_PATHNAME_LEN] = "";
	GetSettingsPath(path);
	
	int overwrite = 0;
	int fExists = FileExists(path, NULL); // 1 if exists, 0 if not
	
	GetCtrlVal(measure, MEASURE_OVERWRITE_SETTINGS, &overwrite);
	
	// check if file exists. If it does not, proceed with creating and saving. If it does, check the overwrite button value
	//if (!FileExists(path, NULL) || ((FileExists(path, NULL)) && overwrite ))
	if (!fExists) // if file does not exist
	{
		/* proceed to save*/
	}
	else if(fExists && overwrite ) // file exists and overwrite is 1
	{
		/* proceed to save*/
	}
	else if(fExists && !overwrite ) // file exist, but do NOT overwrite
	{
		MessagePopup("Warning", "Press the Overwrite button to overwrite existent Settings file.");
		return; // stop execution
	}
	// save 
	FILE *fp = fopen(path, "w");
	PrintSettingsTxt(fp, &tMeas);
	fclose(fp);
	SetCtrlVal(measure, MEASURE_OVERWRITE_SETTINGS, 0); // set the overwrite button to off
	
}

void CVICALLBACK LoadSettingsTxt (int menuBar, int menuItem, void *callbackData,
								  int panel)
{
	char path[MAX_PATHNAME_LEN] = "";
	ssize_t size; 
	GetSettingsPath(path);
	// if file does not exist notify user and return

	if(GetFileInfo(path, &size) == 0)  // file does not exist
	{
		MessagePopup ("Warning", "Settings File does not exist. Please check Settings Folder and File Name.");  
		return;
	}
	FILE *fp = fopen(path, "r");
	ReadFromFile( fp, &tMeas);
	fclose(fp);
	
	// write delay arrays to text boxes
	WriteArrayToTextBox(measure, MEASURE_TEXTBOX_DELGAINCALC, DelaysForGainCalc,  MAXDELAYSFORGAINCALC);
	WriteArrayToTextBox(measure, MEASURE_TEXTBOX_DELMEAS, DelaysForMeasurement,  MAXDELAYSFORMEASUREMENT);
	
	// miscellaneous
	SetCtrlVal(measure, MEASURE_OVERWRITE_SETTINGS, 0); // set the overwrite button to off
	
	if (Gain.Type==AUTO_GAIN_FAST)
	{
		 // If AUTO_GAIN_FAST, look at the SwitchGainCalc value
		 SetCtrlAttribute(measure, MEASURE_SWITCH_GAIN_CALC, ATTR_DIMMED, 0); 
		 SwitchGain(measure, MEASURE_SWITCH_GAIN_CALC, EVENT_COMMIT, NULL,0,0);
	}
	else
	{
		// Dim UI elements
		SetCtrlAttribute(measure, MEASURE_SWITCH_GAIN_CALC, ATTR_DIMMED, 1); 
		DimDelayNumBoxes(1);
		SetCtrlAttribute(measure, MEASURE_TEXTBOX_DELGAINCALC, ATTR_DIMMED, 1);  
		SetCtrlAttribute(measure, MEASURE_ENTER_TO_ORDER, ATTR_DIMMED,1);
		
	}
		
	SwitchGain (measure, MEASURE_SWITCH_GAIN_MEAS, EVENT_COMMIT, NULL, 0, 0); // execute the binary switch callback
	Ccd.Binningy = Ccd.Binningx;
	
	
}






int CVICALLBACK TextBoxLineModified (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	// initialization of arrays that contain the values inside the textboxes
	int *arr = NULL;
	int size = 0;
	if (control ==  MEASURE_TEXTBOX_DELGAINCALC)
	{
		arr = &DelaysForGainCalc[0];
		size = MAXDELAYSFORGAINCALC;
	}
	else if (control == MEASURE_TEXTBOX_DELMEAS)
	{
		arr = &DelaysForMeasurement[0];
		size = MAXDELAYSFORMEASUREMENT;
	}
		
	switch (event)
	{
		case EVENT_COMMIT:
			// check if modified line is valid integer
			int numLines;
			char line[64];
			
			GetNumTextBoxLines(panel, control, &numLines);
			
			// user deleted lines by mistake, append -1 to fill rest  
			if(numLines < size) 
			{
				for( int i = 0; i<(size-numLines); i++)
				{
					SetCtrlVal(panel, control, "-1\n"); 
				}
			} 
			
			// check all lines, replace errors with -1
			for(int i = 0; i<size; i++)
			{
				GetTextBoxLine(panel, control, i, line);
				if(!IsValidInt(line))
				{
					// if invalid integer, set to -1
					ReplaceTextBoxLine(panel, control, i, "-1");
				}
				else if ( (IsValidInt(line)) && (atoi(line)<-1))
				{
					// if valid integer, but negative (<-1) set to -1
					ReplaceTextBoxLine(panel, control, i, "-1");
				}
					
			}
			
			
			//  save textbox content into arr
			ReadTextBoxToArray(panel, control, arr,  size);
			break;
			
		case EVENT_KEYPRESS:
			if (eventData1 == VAL_ENTER_VKEY)
			{
				// call the callback function with the EVENT_COMMMIT case
				TextBoxLineModified (panel, control, EVENT_COMMIT, &callbackData, eventData1, eventData2);
				// reorder the array
				ReorderArray(arr, size);
				WriteArrayToTextBox(panel, control, arr,  size);
				SetCtrlAttribute(panel, control, ATTR_TEXT_SELECTION_START, 0);
				SetCtrlAttribute(panel, control, ATTR_FIRST_VISIBLE_LINE, 0);
			}
			break;
			
	}
	return 0;
}

int CVICALLBACK SwitchGain (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int textbox;
	int *ptextbox = &textbox;
	
	int txtmsg;
	int *ptxtmsg = &txtmsg;
	
	
	// recognize which switch is clicked and store values
	int *pSwitchGain = NULL; // will point to the value of the old-new switches
	
	if (control == MEASURE_SWITCH_GAIN_CALC)
	{
		*ptextbox = MEASURE_TEXTBOX_DELGAINCALC;
		*ptxtmsg = MEASURE_ENTER_TO_ORDER; // "Press enter to order" Text Message UI control
		pSwitchGain = &SwitchGainCalc; // if we click on the Gain Calc binary switch, point to SwitchGainCalc global var 
	}
	else if ( control == MEASURE_SWITCH_GAIN_MEAS)
	{
		*ptextbox = MEASURE_TEXTBOX_DELMEAS;
		*ptxtmsg = MEASURE_ENTER_TO_ORDER_2; // "Press enter to order" Text Message UI control 2
		pSwitchGain = &SwitchGainMeas; // if we click on the Gain Meas binary switch, point to SwitchGainMeas global var
	}
	
	
	switch (event)
	{
		case EVENT_COMMIT: // if switch is NEW or OLD 
			GetCtrlVal(panel, control, pSwitchGain); // store value in appropriate variable
			SetCtrlAttribute(measure, *ptextbox, ATTR_DIMMED, !(*pSwitchGain));  //if switch is NEW, UNdim text box
			SetCtrlAttribute(measure, *ptxtmsg, ATTR_DIMMED, !(*pSwitchGain));   // and UNdim Press Enter text message

			// dim or UNdim appropriate delay boxes
			switch (control)  
			{
				case  MEASURE_SWITCH_GAIN_CALC:
						DimDelayNumBoxes(*pSwitchGain); 
				break;
				case  MEASURE_SWITCH_GAIN_MEAS:
						DimDelayMeasScans(*pSwitchGain);
				break;
			}
		break;
	}
	return 0;
}


//******************************Tau panel functions********************************************************//    


int CVICALLBACK FastTauFit (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			InitTauPanel();
			DisplayPanel (taupanel);
			
			
			int tau1;
			int tau2;
			GetCtrlVal(measure, MEASURE_DELAY_TAU_FIT_1, &tau1);
			GetCtrlVal(measure, MEASURE_DELAY_TAU_FIT_2, &tau2); 
			
			
			
			cMap = CreateColorMap(minTau, maxTau);
			
		
		 	//F2026ImageRatioGrabM; WIP
			PlotIntensity(taupanel, TAU_PNL_GRAPH_LIFETIME, tau, Nx, Ny, VAL_DOUBLE, cMap, VAL_RED, 4, 1,0); 
			DrawColorBar(taupanel, TAU_PNL_COLORBAR, cMap, minTau, maxTau);
			
			// actual formula for the tau calculation;
			/*
			for(int i, i<NumPixels; i++) {
				taumap[i] = (double)(tau1-tau2)/log(intensity_final[i]/intensity_initial[i]);
			}
			*/

			break;
	}
	return 0;
}


int CVICALLBACK BrowseSettingsFolder (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			char selectedDir[MAXLINELENGTH] = "";
			int result;
			result = DirSelectPopup("C:\\", "Select folder to save settings",1,1, selectedDir);
			if(result ==1)		  // user selected folder
			{
			  SetCtrlVal(panel, MEASURE_SETTINGS_FOLDER, selectedDir);
			}
			break;
	}
	return 0;
}



void InitTauPanel(){
	getMinMax(tau, Nx*Ny, &minTau, &maxTau);
	SetCtrlAttribute(taupanel, TAU_PNL_MAX_TAU, ATTR_MAX_VALUE, maxTau);
	SetCtrlAttribute(taupanel, TAU_PNL_MAX_TAU, ATTR_MIN_VALUE, minTau);
	SetCtrlAttribute(taupanel, TAU_PNL_MIN_TAU, ATTR_MAX_VALUE, maxTau);
	SetCtrlAttribute(taupanel, TAU_PNL_MIN_TAU, ATTR_MIN_VALUE, minTau);
	
	SetCtrlVal(taupanel, TAU_PNL_MAX_TAU, maxTau);
	SetCtrlVal(taupanel, TAU_PNL_MIN_TAU, minTau); 
}
	

ColorMapEntry *CreateColorMap(double minVal, double maxVal)
{
	static ColorMapEntry colorMap[4];
	double range = maxVal - minVal;
	
	colorMap[0].color = VAL_BLUE;
	colorMap[0].dataValue.valDouble = minVal;
	colorMap[1].color = VAL_CYAN;
	colorMap[1].dataValue.valDouble = minVal + range*(1.0/3.0);
	colorMap[2].color = VAL_YELLOW;
	colorMap[2].dataValue.valDouble = minVal + range*(2.0/3.0);
	colorMap[3].color = VAL_RED;
	colorMap[3].dataValue.valDouble = maxVal;
	
	return  colorMap;
}

void getMinMax(double *data, int size, double *min, double *max)
{
	*min = data[0];
	*max = data[0];
	
	for(int i = 0; i<size; i++) {
		if (data[i] < *min) *min = data[i];
		if (data[i] > *max) *max = data[i];
	}
}


void DrawColorBar(int panel, int colorBarCtrl, ColorMapEntry *cMap, double minVal, double maxVal)
{
    int nSteps = 256;
    double range = maxVal - minVal;
    int barWidth;
    GetCtrlAttribute(panel, colorBarCtrl, ATTR_WIDTH, &barWidth);
    double *gradient = malloc(barWidth * nSteps * sizeof(double));

    for(int row = 0; row < nSteps; row++){
        for(int col = 0; col < barWidth; col++){
            gradient[row * barWidth + col] = minVal + (double)row / (nSteps - 1) * range;
        }
    }

    DeleteGraphPlot(panel, colorBarCtrl, -1, VAL_IMMEDIATE_DRAW);
    
    // yGain and yOffset map pixel rows to your data range
    double yGain   = range / (nSteps - 1);
    double yOffset = minVal;
    
    PlotScaledIntensity(panel, colorBarCtrl, gradient, barWidth, nSteps, VAL_DOUBLE,
                        yGain, yOffset,   // y scaling
                        1.0, 0.0,         // x scaling (doesn't matter for colorbar)
                        cMap, cMap[3].color, 4, 1, 0);
    free(gradient);

    SetAxisScalingMode(panel, colorBarCtrl, VAL_RIGHT_YAXIS, VAL_MANUAL, minVal, maxVal);
}
  

int CVICALLBACK TauPanelCallBack (int panel, int event, void *callbackData,
								  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel(panel);

			break;
	}
	return 0;
}

int CVICALLBACK SetMaxColorBar (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double newMax;
			double newMin;
			GetCtrlVal(taupanel, TAU_PNL_MAX_TAU, &newMax);
			GetCtrlVal(taupanel, TAU_PNL_MIN_TAU, &newMin);
			cMap = CreateColorMap(newMin, newMax);
			
			PlotIntensity(taupanel, TAU_PNL_GRAPH_LIFETIME, tau, Nx, Ny, VAL_DOUBLE, cMap, VAL_RED, 4, 1,0); 
			DrawColorBar(taupanel, TAU_PNL_COLORBAR, cMap, newMin, newMax);
			
			

			break;
	}
	return 0;
}

int CVICALLBACK SetMinColorBar (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double newMax;
			double newMin;
			GetCtrlVal(taupanel, TAU_PNL_MAX_TAU, &newMax);
			GetCtrlVal(taupanel, TAU_PNL_MIN_TAU, &newMin);
			cMap = CreateColorMap(newMin, newMax);
			
			PlotIntensity(taupanel, TAU_PNL_GRAPH_LIFETIME, tau, Nx, Ny, VAL_DOUBLE, cMap, VAL_RED, 4, 1,0); 
			DrawColorBar(taupanel, TAU_PNL_COLORBAR, cMap, newMin, newMax);
			
			


			break;
	}
	return 0;
}


int CVICALLBACK ConsolePanelCallback (int panel, int event, void *callbackData,
									  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel(panel);
			break;
	}
	return 0;
}

int CVICALLBACK MeasurePanelCallback (int panel, int event, void *callbackData,
									  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			//Copy contents of Quit (MENU, MENU_FILE_QUIT, &callbackData,panel); here:
			int flag;
	
			if (Hardware.Ccd==1){
				flag=QuitCCD(Hardware.CcdType); 
				if (flag<0) {
					MessagePopup ("Warning", "CCD camera was not properly closed !");  
				}
			
				if (Hardware.Buffer==1) {	
					QuitBuffer();
					Hardware.Buffer=0;
				}
			}	
	
			if (Hardware.Del==1){	
				QuitDelayGenerator(Hardware.DelayType);
		
			}	
	
			if (Hardware.Intensifier==1){
				QuitIntensifier();			
			}
			imaqDispose(image);  
			QuitUserInterface(0);
			QuitProg=1;
			

			break;
	}
	return 0;
}

void CVICALLBACK ConsolePanel (int menuBar, int menuItem, void *callbackData,
							   int panel)
{
	DisplayPanel(console);
}

int CVICALLBACK SmallPanelCallback (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			//reset gray level settings
			Ccd.grayH =GRAY_HIGH+1;
			Ccd.grayL =GRAY_LOW;
			
			SetCtrlVal(panel, SMALLPANEL_GRAY_HIGH, GRAY_HIGH+1); // Set to 16382+1
			SetCtrlVal(panel, SMALLPANEL_GRAY_LOW, GRAY_LOW); // Set to 0
			HidePanel(panel); 
			break;
	}
	return 0;
}



// More functions from the utils.c file

// populate struct of type stMeas
void CreateTableMeas(stMeas *s){
int idx = 0;
# define X(ctrl, pnl, addr, ctrlName, type) \
	s->Ctrl[idx] = ctrl; \
	s->Pnl[idx] =  pnl; \
	s->Addr[idx] = addr; \
	strcpy(s->CtrlName[idx], #ctrlName); \
	s->Type[idx] = type; \
	idx++;


//MEASURE_TABLE   
#include "measure_pnl_ctrls.def"
NumMeas = idx;


//SMALLPANEL_TABLE
#include "small_pnl_ctrls.def"   
NumSmallPnl = idx;

//D150PANEL_TABLE
#include "d150_pnl_ctrls.def"   
NumD150Pnl = idx;

//STANFORDPANEL_TABLE
#include "stanford_pnl_ctrls.def"   
NumStanfordPnl = idx;


// DELAYS FOR GAIN CALC AND MEASUREMENT
# include "delays_for_gain_calc_and_measurement.def"
NumDelaysGainCalcMeasurement = idx;

//CONSOLEPROFILE_TABLE
// #include "consoleprofile_pnl_ctrls.def"   
//NumConsoleProfile=idx;


NumCtrl = idx;

# undef X

}



// reads the controls and updates the values in the struct // NOT USED 
void ReadAllControls(stMeas *s)
{
	for(int i = 0; i<NumCtrl; i++)
	{
		GetCtrlVal(s->Pnl[i], s->Ctrl[i], s->Addr[i]);
	}
}



// checks whether a UI control is in an UIException array
int isException(int panel, int control, UIException *ex, int nExceptions)
{
	for (int i = 0; i<nExceptions; i++)
	{
		if (panel == ex[i].panel && control == ex[i].control)
		{
			return 1; // found exception!
		}
	}
	return 0;
	
}





// print the values with the appropriate format identifier
# define PRINT_VALUE(ctype, void_addr) (*(ctype*)(void_addr))
void PrintSettingsTxt(FILE *fp, stMeas *s)
{
	// print name and type into designated file
	DATATYPE type;
	
	// instantiate exceptions
	 UIException ex[] = {
		 {smallpanel, SMALLPANEL_DELAY_SMALL},   // shares address with MEASURE_DELAY &Delays.Set
		 {smallpanel, SMALLPANEL_GAIN_SMALL}	 // shares address with MEASURE_GAIN &Gain.Set
	 };
	 
	 
	 // Dummy controls are UI controls that help in saving to txt the values of all array elements.
	 // In the .def fille the panel and control identifiers do not change, while the address saved in tMeas does
	 UIException dummyCtrl[] = {
		 {measure, MEASURE_DUMMY_DELAYSGAINCALC},
		 {measure, MEASURE_DUMMY_DEL_MEASUREMENT}
	 };

	for (int i = 0; i < NumCtrl; i++)
	{
		// skip exceptions, controls we don't want to save because they share the same address in tMeas as other controls   
		if (isException(s->Pnl[i], s->Ctrl[i], ex, sizeof(ex)/sizeof(ex[0])) )
			continue;
	
		// reading all the values in the UI controls and saving them in the struct
		// necessary for UI controls whose default value shown is different from the 
		// initialization value at the start of the program 

		if (isException(s->Pnl[i], s->Ctrl[i], dummyCtrl, sizeof(dummyCtrl)/sizeof(dummyCtrl[0])) )
		{
			// if the UI control we are saving is a dummy, do NOT read and store its value
			/* do nothing */
		} else {
			// read and store the UI control value
			GetCtrlVal(s->Pnl[i], s->Ctrl[i], s->Addr[i]);
		}
		
		type = s->Type[i]; 
		
		
		fprintf(fp, "%s\t%s\t", s->CtrlName[i], TYPENAMES[type]);
		if (type == CHAR_ARRAY)	  // no need to dereference for char array type
		{
			fprintf(fp, "%s\n", (char*) s->Addr[i]);
			continue;
		} 
		else 
		{
		# define X(enumtype, fmt, ctype)								\
			if (type == enumtype)  										\
			{															\
				fprintf(fp, fmt "\n", PRINT_VALUE(ctype, s->Addr[i]));	\
				continue ;												\
			}
		# include "types.def"
		# undef X
		}
    fprintf(fp, "%s\tUNRECOGNIZED_TYPE\t-1\n", s->CtrlName[i]);
	}
}





// reads the value as a character array in s, then based on the type,
// stores the value in out address 
void String2Value(char *s, DATATYPE type, void *out)
{
    #define X(enumtype, fmt, ctype)                             \
        if (type == enumtype) {                                 \
            if (type == CHAR_ARRAY) {                           \
                strcpy((char*)out, s);                          \
            } else {                                            \
                sscanf(s, fmt, (ctype*)out);                    \
            }                                                   \
        }

	# include "types.def"

    #undef X
}

// reads the value in the address stored in the struct tMeas and updates the value 
// shown in the corresponding UI control
void UpdateControl(int panelHandle, int controlID, DATATYPE type, void *addr)
{
# define X(enumtype, fmt, ctype) 		\
	if(type == enumtype) 				\
	{									\
		if(type == CHAR_ARRAY)			\
		{								\
			SetCtrlVal(panelHandle, controlID, (char*)addr); \
		} else{							\
			SetCtrlVal(panelHandle, controlID, *(ctype*)addr);\
		}								\
	} 									
	# include "types.def"
# undef X
	
}


// reads the path for the settings folder and file name from the controls in the 
// measure panel and stores it in the input string 
void GetSettingsPath(char * s){
	char folderPath[MAXLINELENGTH] = "";
	char fileName[MAXLINELENGTH] =  "";
	GetCtrlVal(measure, MEASURE_SETTINGS_FOLDER, folderPath);
	GetCtrlVal(measure, MEASURE_SETTINGS_NAME, fileName); 
	MakePathname(folderPath, fileName, s);
}


// Read from file to struct and updates the corresponding controls 
void ReadFromFile( FILE * fp, stMeas *s)
{
	char content[MAXTABLEMEAS][MAXLINELENGTH] = {0};
	
	// copy file contents into content  
	int currentLine = 0;
	while (!feof(fp) && !ferror(fp))
	{
		if (fgets(content[currentLine], MAXLINELENGTH, fp) != NULL)
		{
			currentLine++;	
		}
		
	}
		
	// helper variables
	char varname_string[MAXLINELENGTH];
	char type_string[MAXLINELENGTH]; 
	char value_string[MAXLINELENGTH]; 
	
	// loop inside content
	for (int i = 0; i<currentLine; i++)
	{
		sscanf(content[i], "%s\t%s\t%s", varname_string, type_string, value_string);
		
		// look for corresponding entry in lookup table
		int found = 0;
		for(int j = 0; j<NumCtrl; j++)
		{
			if (strcmp(s->CtrlName[j], varname_string) == 0) 
			{
				found = 1;
				String2Value(value_string, s->Type[j], s->Addr[j]); // save variable in tMeas.Addr[j]
				UpdateControl(s->Pnl[j], s->Ctrl[j], s->Type[j], s->Addr[j]);
				break;
			}
		}
		if (found == 0) 
		{
			printf("Warning: variable %s is not found. Check spelling.\n", varname_string);
			continue;
		}
	}
	
		
}

// initialize the csvDelays array to -1
void InitDelayArray(int DelayArr[], int length){
	 for(int i = 0; i <length; i++)
	 {
		 DelayArr[i] = -1;
	 }
	
}


//// WIP
void ReadCsvDelays( FILE * fp, int *DelArray, int textBoxPanel, int textBoxControl)
{
	char content[MAXCSVDELAYS][MAXLINELENGTH] = {0};
	char textBoxContent[MAXCSVDELAYS*MAXLINELENGTH] = {0};
	
	// copy file contents into content  
	int currentLine = 0;
	while (fgets(content[currentLine], MAXLINELENGTH, fp) != NULL)
	{
		DelArray[currentLine] = atoi(content[currentLine]);
		strcat(textBoxContent, content[currentLine]);
		//strcat(textBoxContent, "\n");
		//printf("%d\n", DelArray[currentLine]);
	    currentLine++;
	}
	
	//update the content in the designated textbox
	SetCtrlVal(textBoxPanel, textBoxControl, textBoxContent);
		
/*		
	// helper variables
	char value_string[MAXLINELENGTH]; 
	
	// loop inside content
	for (int i = 0; i<currentLine; i++)
	{
		sscanf(content[i], "%s", value_string);
		
		// look for corresponding entry in lookup table
		int found = 0;
		for(int j = 0; j<NumCtrl; j++)
		{
			if (strcmp(s->CtrlName[j], varname_string) == 0) 
			{
				found = 1;
				String2Value(value_string, s->Type[j], s->Addr[j]); // save variable in tMeas.Addr[j]
				UpdateControl(s->Pnl[j], s->Ctrl[j], s->Type[j], s->Addr[j]);
				break;
			}
		}
		if (found == 0) 
		{
			printf("Warning: variable %s is not found. Check spelling.\n", varname_string);
			continue;
		}
	}
*/	
		
}


void WriteArrayToTextBox(int panel, int textBoxControl, int arr[], int numel)
{
	char line[MAXLINELENGTH];
	ResetTextBox(panel, textBoxControl, "");
	
	for (int i = 0; i<numel; i++)
	{
		sprintf(line, "%d\n", arr[i]);
		SetCtrlVal(panel, textBoxControl, line);
	}
	
}


void ReadTextBoxToArray(int panel, int textBoxControl, int arr[], int numel)
{
	// initialize array to -1
	InitDelayArray(arr, numel);
	
	char line[64];
	
	for (int i = 0; i <numel; i++)
	{
		GetTextBoxLine(panel, textBoxControl, i, line);
		arr[i] = atoi(line); // convert string to integer

	}
}


int IsValidInt(char * str)
{
	char *endPtr;
	
	if (str == NULL || str[0] == '\0')
		return 0; // empty string
	
	strtol(str, &endPtr, 10); //reads until the last integer
	return (*endPtr == '\0'); // 1 if fully consumed, 0 if leftover characters
}

void DimDelayNumBoxes(int yn)
{
			SetCtrlAttribute(measure,MEASURE_DELAY_1,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_2,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_3,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_4,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_5,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_6,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_7,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_8,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_9,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_10,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_11,ATTR_DIMMED,yn);
			SetCtrlAttribute(measure,MEASURE_DELAY_12,ATTR_DIMMED,yn);
			
			SetCtrlAttribute(measure,MEASURE_TEXTMSG_24, ATTR_DIMMED,yn);
		
}

void DimDelayMeasScans(int yn){
	
			SetCtrlAttribute(measure, MEASURE_DELAY_FIRST, ATTR_DIMMED, yn);
			SetCtrlAttribute(measure, MEASURE_DELAY_STEP, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_LAST, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_ADDITIONAL_SCAN, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_FIRST_2, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_STEP_2, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_LAST_2, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_FIRST_3, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_STEP_3, ATTR_DIMMED, yn);  
			SetCtrlAttribute(measure, MEASURE_DELAY_LAST_3, ATTR_DIMMED, yn); 
			
			SetCtrlAttribute(measure, MEASURE_TEXTMSG_11, ATTR_DIMMED, yn);
			SetCtrlAttribute(measure, MEASURE_TEXTMSG_16, ATTR_DIMMED, yn);
}
	

int comp(const void *a, const void *b)
{
	return (*(int*)a-*(int*)b);
}

void ReorderArray(int arr[], int numel)
{
	// Reorders in ascendent order array arr of numel elements
	int *tempArr;
	int validCount = 0;
	int i, j;
	for (i = 0; i<numel; i++){
		if (arr[i] != -1) validCount++;
	}
	
	if(validCount == 0) return; // nothing to sort
	
	// create temporary array to hold the valid  values
	tempArr = malloc(validCount * sizeof(int));
	j = 0;
	for (i = 0; i<numel; i ++)
	{
		if(arr[i] != -1){
			tempArr[j] = arr[i];
			j++;
		}
	}
	
	// sort tempArr[]
	qsort(tempArr,  validCount, sizeof(tempArr[0]), comp);
	
	// reorder arr[]
	for (i = 0; i<numel; i++)
	{
		if (i<validCount)
		{
			arr[i] = tempArr[i];
		}
		else
		{
			arr[i] = -1;
		}
	}
	free(tempArr);

}



	



/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CONSOLE                          1       /* callback function: ConsolePanelCallback */
#define  CONSOLE_GRAPH                    2       /* control type: graph, callback function: (none) */
#define  CONSOLE_GAIN                     3       /* control type: numeric, callback function: (none) */
#define  CONSOLE_MEAN                     4       /* control type: numeric, callback function: (none) */
#define  CONSOLE_TEXP                     5       /* control type: numeric, callback function: (none) */
#define  CONSOLE_DELAY                    6       /* control type: numeric, callback function: (none) */
#define  CONSOLE_LIMIT                    7       /* control type: numeric, callback function: (none) */
#define  CONSOLE_SUM                      8       /* control type: numeric, callback function: (none) */
#define  CONSOLE_PEAK                     9       /* control type: numeric, callback function: (none) */
#define  CONSOLE_TOT                      10      /* control type: numeric, callback function: (none) */
#define  CONSOLE_NUMSATURATED             11      /* control type: numeric, callback function: (none) */
#define  CONSOLE_LEDSAT                   12      /* control type: LED, callback function: (none) */
#define  CONSOLE_TEXTBOX                  13      /* control type: textBox, callback function: (none) */
#define  CONSOLE_DISPLAY_MEASPNL          14      /* control type: command, callback function: DisplayMeasurePanel */

#define  D150PANEL                        2
#define  D150PANEL_TEXTMSG                2       /* control type: textMsg, callback function: (none) */
#define  D150PANEL_RANGE                  3       /* control type: numeric, callback function: UpdateMeasure */
#define  D150PANEL_TEXTMSG_2              4       /* control type: textMsg, callback function: (none) */
#define  D150PANEL_TRIGGERSLOPE           5       /* control type: binary, callback function: UpdateMeasure */
#define  D150PANEL_TRIGGERLED             6       /* control type: LED, callback function: (none) */
#define  D150PANEL_TRIGGERTHR             7       /* control type: numeric, callback function: UpdateMeasure */
#define  D150PANEL_TEXTMSG_3              8       /* control type: textMsg, callback function: (none) */
#define  D150PANEL_INVERT                 9       /* control type: binary, callback function: UpdateMeasure */
#define  D150PANEL_UPDATE                 10      /* control type: command, callback function: UpdateD150 */
#define  D150PANEL_CLOSEBUTTON            11      /* control type: command, callback function: CloseD150Panel */

#define  KEYPRESS                         3
#define  KEYPRESS_ENTER                   2       /* control type: command, callback function: (none) */
#define  KEYPRESS_TEXTMSG                 3       /* control type: textMsg, callback function: (none) */

#define  MEASURE                          4       /* callback function: MeasurePanelCallback */
#define  MEASURE_TEXTMSG_21               2       /* control type: textMsg, callback function: (none) */
#define  MEASURE_GATE_WIDTH               3       /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_GAIN                     4       /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_GAIN_INIT                5       /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_GAIN_TYPE                6       /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_UPDATE_INTENS            7       /* control type: command, callback function: UpdateIntensifier */
#define  MEASURE_GET_INTENS_STATUS        8       /* control type: command, callback function: GetIntensifierStatus */
#define  MEASURE_SET_GAIN                 9       /* control type: command, callback function: SetGain */
#define  MEASURE_TEXTMSG_12               10      /* control type: textMsg, callback function: (none) */
#define  MEASURE_CCD_MODULE               11      /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_22               12      /* control type: textMsg, callback function: (none) */
#define  MEASURE_ROI_X1                   13      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_ROI_Y1                   14      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_ROI_X2                   15      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_ROI_Y2                   16      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_R6_PARM                  17      /* control type: ring, callback function: (none) */
#define  MEASURE_TEXTMSG_2                18      /* control type: textMsg, callback function: (none) */
#define  MEASURE_TIMESCALE                19      /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_BINNING                  20      /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_GET_CCD_PARAM_VALUE      21      /* control type: command, callback function: GetCcdParamValue */
#define  MEASURE_UPDATE_CCD               22      /* control type: command, callback function: UpdateCcd */
#define  MEASURE_CCD_PARM_VALUE           23      /* control type: numeric, callback function: (none) */
#define  MEASURE_CCD_EXPOSURE             24      /* control type: numeric, callback function: SetTimeExp */
#define  MEASURE_TEXTMSG_19               25      /* control type: textMsg, callback function: (none) */
#define  MEASURE_DELAY_MODULE             26      /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_DELAY_TAU_FIT_2          27      /* control type: numeric, callback function: (none) */
#define  MEASURE_DELAY_TAU_FIT_1          28      /* control type: numeric, callback function: (none) */
#define  MEASURE_DELAY                    29      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_SET_DELAY                30      /* control type: command, callback function: SetDelay */
#define  MEASURE_TEXTMSG_25               31      /* control type: textMsg, callback function: (none) */
#define  MEASURE_LASER_FREQ               32      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_LASER_PULSES             33      /* control type: numeric, callback function: (none) */
#define  MEASURE_INT_ON                   34      /* control type: binary, callback function: UpdateMeasure */
#define  MEASURE_CCD_ON                   35      /* control type: binary, callback function: UpdateMeasure */
#define  MEASURE_DEL_ON                   36      /* control type: binary, callback function: UpdateMeasure */
#define  MEASURE_LASER_ON                 37      /* control type: binary, callback function: UpdateMeasure */
#define  MEASURE_INIT_HW                  38      /* control type: command, callback function: InitHw */
#define  MEASURE_INFO_PREFIX              39      /* control type: string, callback function: UpdateMeasure */
#define  MEASURE_INFO_CODE                40      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_INFO_SUFFIX              41      /* control type: string, callback function: UpdateMeasure */
#define  MEASURE_INFO_COMMENT             42      /* control type: string, callback function: UpdateMeasure */
#define  MEASURE_INFO_DIR                 43      /* control type: string, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_9                44      /* control type: textMsg, callback function: (none) */
#define  MEASURE_GAIN_LEVEL_THR           45      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_GAIN_NUMPIX_THR          46      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_GAIN_SAT                 47      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_GAIN_SLOPE               48      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_18               49      /* control type: textMsg, callback function: (none) */
#define  MEASURE_BKG_LED                  50      /* control type: LED, callback function: (none) */
#define  MEASURE_BKGSWITCH                51      /* control type: binary, callback function: BkgOnOff */
#define  MEASURE_CCD_PARM_STRING          52      /* control type: string, callback function: (none) */
#define  MEASURE_BKG_PATH                 53      /* control type: string, callback function: (none) */
#define  MEASURE_BKG_FILE                 54      /* control type: string, callback function: (none) */
#define  MEASURE_IMAGE_MODE               55      /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_20               56      /* control type: textMsg, callback function: (none) */
#define  MEASURE_SINGLE_GRAB              57      /* control type: command, callback function: SingleGrab */
#define  MEASURE_BROWSE_SETTINGS          58      /* control type: command, callback function: BrowseSettingsFolder */
#define  MEASURE_FAST_TAU_FIT             59      /* control type: command, callback function: FastTauFit */
#define  MEASURE_GRAB_IMAGE               60      /* control type: command, callback function: ImageGrab */
#define  MEASURE_NUM_AVERAGE              61      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_15               62      /* control type: textMsg, callback function: (none) */
#define  MEASURE_TEXTMSG_11               63      /* control type: textMsg, callback function: (none) */
#define  MEASURE_DELAY_FIRST              64      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_STEP               65      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_LAST               66      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_ADDITIONAL_SCAN          67      /* control type: radioButton, callback function: UpdateMeasure */
#define  MEASURE_DELAY_FIRST_2            68      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_STEP_2             69      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_LAST_2             70      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_FIRST_3            71      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_STEP_3             72      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_LAST_3             73      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_BKG_SCAN                 74      /* control type: radioButton, callback function: UpdateMeasure */
#define  MEASURE_DELAY_BKG_FIRST          75      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_BKG_STEP           76      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_BKG_LAST           77      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_AUTOVALUEFILE      78      /* control type: ring, callback function: UpdateMeasure */
#define  MEASURE_NUMDELAY_AUTO            79      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_24               80      /* control type: textMsg, callback function: (none) */
#define  MEASURE_DELAY_1                  81      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_2                  82      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_3                  83      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_4                  84      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_5                  85      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_6                  86      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_7                  87      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_8                  88      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_9                  89      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_10                 90      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_11                 91      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_DELAY_12                 92      /* control type: numeric, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_5                93      /* control type: textMsg, callback function: (none) */
#define  MEASURE_DISPLAY_MODE             94      /* control type: slide, callback function: UpdateMeasure */
#define  MEASURE_TEXTMSG_23               95      /* control type: textMsg, callback function: (none) */
#define  MEASURE_TEXTMSG_16               96      /* control type: textMsg, callback function: (none) */
#define  MEASURE_TEXTMSG_8                97      /* control type: textMsg, callback function: (none) */
#define  MEASURE_MCP_GATED                98      /* control type: slide, callback function: UpdateMeasure */
#define  MEASURE_SETTINGS_NAME            99      /* control type: string, callback function: (none) */
#define  MEASURE_SETTINGS_FOLDER          100     /* control type: string, callback function: (none) */
#define  MEASURE_TEXTBOX_DELMEAS          101     /* control type: textBox, callback function: TextBoxLineModified */
#define  MEASURE_TEXTBOX_DELGAINCALC      102     /* control type: textBox, callback function: TextBoxLineModified */
#define  MEASURE_DUMMY_DELAYSGAINCALC     103     /* control type: numeric, callback function: (none) */
#define  MEASURE_DUMMY_DEL_MEASUREMENT    104     /* control type: numeric, callback function: (none) */
#define  MEASURE_SWITCH_GAIN_MEAS         105     /* control type: binary, callback function: SwitchGain */
#define  MEASURE_SWITCH_GAIN_CALC         106     /* control type: binary, callback function: SwitchGain */
#define  MEASURE_ENTER_TO_ORDER           107     /* control type: textMsg, callback function: (none) */
#define  MEASURE_OVERWRITE_SETTINGS       108     /* control type: radioButton, callback function: (none) */
#define  MEASURE_ENTER_TO_ORDER_2         109     /* control type: textMsg, callback function: (none) */

#define  SMALLPANEL                       5       /* callback function: SmallPanelCallback */
#define  SMALLPANEL_TEXTMSG_12            2       /* control type: textMsg, callback function: (none) */
#define  SMALLPANEL_CCD_EXPOSURE          3       /* control type: numeric, callback function: SetTimeExp */
#define  SMALLPANEL_TEXTMSG_11            4       /* control type: textMsg, callback function: (none) */
#define  SMALLPANEL_DELAY_SMALL           5       /* control type: numeric, callback function: UpdateMeasure */
#define  SMALLPANEL_SET_DELAY             6       /* control type: command, callback function: SetDelay */
#define  SMALLPANEL_TEXTMSG_10            7       /* control type: textMsg, callback function: (none) */
#define  SMALLPANEL_GAIN_SMALL            8       /* control type: numeric, callback function: UpdateMeasure */
#define  SMALLPANEL_SET_GAIN              9       /* control type: command, callback function: SetGain */
#define  SMALLPANEL_DECORATION_8          10      /* control type: deco, callback function: (none) */
#define  SMALLPANEL_DECORATION_7          11      /* control type: deco, callback function: (none) */
#define  SMALLPANEL_GRAY_LOW              12      /* control type: scale, callback function: UpdateMeasure */
#define  SMALLPANEL_LASER_PULSES          13      /* control type: numeric, callback function: (none) */
#define  SMALLPANEL_GRAY_HIGH             14      /* control type: scale, callback function: UpdateMeasure */
#define  SMALLPANEL_GRAY_AUTO             15      /* control type: binary, callback function: UpdateMeasure */

#define  SPECTR_PNL                       6
#define  SPECTR_PNL_GRAPH                 2       /* control type: graph, callback function: (none) */

#define  STFORD_PNL                       7
#define  STFORD_PNL_TEXTMSG_4             2       /* control type: textMsg, callback function: (none) */
#define  STFORD_PNL_TRIG_EXTERN           3       /* control type: binary, callback function: UpdateMeasure */
#define  STFORD_PNL_TRIGGERSLOPE          4       /* control type: binary, callback function: UpdateMeasure */
#define  STFORD_PNL_TRIGGERLED            5       /* control type: LED, callback function: (none) */
#define  STFORD_PNL_TRIGGERTHR            6       /* control type: numeric, callback function: UpdateMeasure */
#define  STFORD_PNL_TEXTMSG_3             7       /* control type: textMsg, callback function: (none) */
#define  STFORD_PNL_INVERT                8       /* control type: binary, callback function: UpdateMeasure */
#define  STFORD_PNL_UPDATE                9       /* control type: command, callback function: UpdateStanford */
#define  STFORD_PNL_CLOSEBUTTON           10      /* control type: command, callback function: CloseStanfordPanel */

#define  TAU_PNL                          8       /* callback function: TauPanelCallBack */
#define  TAU_PNL_COLORBAR                 2       /* control type: graph, callback function: (none) */
#define  TAU_PNL_GRAPH_LIFETIME           3       /* control type: graph, callback function: (none) */
#define  TAU_PNL_MAX_TAU                  4       /* control type: scale, callback function: SetMaxColorBar */
#define  TAU_PNL_MIN_TAU                  5       /* control type: scale, callback function: SetMinColorBar */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENU                             1
#define  MENU_FILE                        2
#define  MENU_FILE_LOAD_IMG               3       /* callback function: LoadImg */
#define  MENU_FILE_SEPARATOR_2            4
#define  MENU_FILE_SAVE_SETTINGS_TXT      5       /* callback function: SaveSettingsTxt */
#define  MENU_FILE_LOAD_SETTINGS_TXT      6       /* callback function: LoadSettingsTxt */
#define  MENU_FILE_QUIT                   7       /* callback function: Quit */
#define  MENU_VIEW                        8
#define  MENU_VIEW_VIEWCCD                9       /* callback function: ccdpanel */
#define  MENU_VIEW_VIEWGRAY               10      /* callback function: GrayPanel */
#define  MENU_VIEW_VIEWSMALLPANEL         11      /* callback function: SmallPanel */
#define  MENU_VIEW_VIEWDEL150PANEL        12      /* callback function: Del150Panel */
#define  MENU_VIEW_VIEWSTANFORDPANEL      13      /* callback function: StanfordPanel */
#define  MENU_VIEW_VIEWCONSOLEPANEL       14      /* callback function: ConsolePanel */
#define  MENU_RUN                         15
#define  MENU_RUN_ACQUIRE                 16      /* callback function: Acquire */
#define  MENU_HELP                        17
#define  MENU_HELP_ABOUT                  18      /* callback function: About */


     /* Callback Prototypes: */

void CVICALLBACK About(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Acquire(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK BkgOnOff(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BrowseSettingsFolder(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ccdpanel(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK CloseD150Panel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseStanfordPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ConsolePanel(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ConsolePanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Del150Panel(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK DisplayMeasurePanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FastTauFit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetCcdParamValue(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetIntensifierStatus(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK GrayPanel(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ImageGrab(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InitHw(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK LoadImg(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK LoadSettingsTxt(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK MeasurePanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Quit(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK SaveSettingsTxt(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK SetDelay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetGain(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetMaxColorBar(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetMinColorBar(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetTimeExp(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SingleGrab(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK SmallPanel(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK SmallPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK StanfordPanel(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK SwitchGain(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TauPanelCallBack(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TextBoxLineModified(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateCcd(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateD150(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateIntensifier(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateMeasure(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateStanford(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

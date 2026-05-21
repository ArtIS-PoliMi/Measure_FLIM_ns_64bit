#define MAXTABLEMEAS	500

int NumMeas, NumSmallPnl, NumD150Pnl, NumStanfordPnl, NumCtrl;

struct stMeas{
	int Ctrl[MAXTABLEMEAS];
	int Pnl[MAXTABLEMEAS];
	void *Addr[MAXTABLEMEAS];
} tMeas;

void AddMeas(int idx,int Control,int Panel, void *Address)
{
	tMeas.Ctrl[idx]=Control;
	tMeas.Pnl[idx]=Panel;
	tMeas.Addr[idx]=Address;

}

void CreateTableMeas(void)
{
int idx=0;
AddMeas(idx++,MEASURE_GATE_WIDTH,measure,&Gate.Width);
AddMeas(idx++,MEASURE_MCP_GATED,measure,&Gate.McpGateOn);

AddMeas(idx++,MEASURE_GAIN,measure,&Gain.Set);
AddMeas(idx++,MEASURE_GAIN_INIT,measure,&Gain.Init);
AddMeas(idx++,MEASURE_GAIN_TYPE,measure,&Gain.Type);

AddMeas(idx++,MEASURE_CCD_MODULE,measure,&Hardware.CcdType);
AddMeas(idx++,MEASURE_ROI_X1,measure,&Ccd.Roix1);
AddMeas(idx++,MEASURE_ROI_X2,measure,&Ccd.Roix2);
AddMeas(idx++,MEASURE_ROI_Y1,measure,&Ccd.Roiy1);
AddMeas(idx++,MEASURE_ROI_Y2,measure,&Ccd.Roiy2);  
AddMeas(idx++,MEASURE_BINNING,measure,&Ccd.Binningx);

//AddMeas(idx++,MEASURE_CCD_EXPOSURE,measure,&Ccd.Exposure);


AddMeas(idx++,MEASURE_DELAY_MODULE,measure,&Hardware.DelayType);
AddMeas(idx++,MEASURE_DELAY,measure,&Delays.Set);
AddMeas(idx++,MEASURE_TIMESCALE,measure,&Delays.timescale);


AddMeas(idx++,MEASURE_LASER_ON,measure,&Hardware.Laser);
AddMeas(idx++,MEASURE_LASER_FREQ,measure,&Laser.Freq);

AddMeas(idx++,MEASURE_INT_ON,measure,&Hardware.Intensifier);
AddMeas(idx++,MEASURE_CCD_ON,measure,&Hardware.Ccd);
AddMeas(idx++,MEASURE_DEL_ON,measure,&Hardware.Del);

AddMeas(idx++,MEASURE_INFO_PREFIX,measure,Info.Prefix);
AddMeas(idx++,MEASURE_INFO_CODE,measure,&Info.Code);
AddMeas(idx++,MEASURE_INFO_SUFFIX,measure,Info.Suffix);
AddMeas(idx++,MEASURE_INFO_COMMENT,measure,Info.Comment);
AddMeas(idx++,MEASURE_INFO_DIR,measure,Info.Dir);

AddMeas(idx++,MEASURE_GAIN_LEVEL_THR,measure,&Gain.ThrLevel);
AddMeas(idx++,MEASURE_GAIN_NUMPIX_THR,measure,&Gain.ThrNumPix);
AddMeas(idx++,MEASURE_GAIN_SAT,measure,&Gain.Sat);
AddMeas(idx++,MEASURE_GAIN_SLOPE,measure,&Gain.Slope);

AddMeas(idx++,MEASURE_NUM_AVERAGE,measure,&Acquisition.NumAverage);
AddMeas(idx++,MEASURE_IMAGE_MODE,measure,&Acquisition.Mode);

AddMeas(idx++,MEASURE_DISPLAY_MODE,measure,&Acquisition.DisplayMode);

AddMeas(idx++,MEASURE_DELAY_FIRST,measure,&Delays.First[0]);
AddMeas(idx++,MEASURE_DELAY_STEP,measure,&Delays.Step[0]);
AddMeas(idx++,MEASURE_DELAY_LAST,measure,&Delays.Last[0]);
AddMeas(idx++,MEASURE_DELAY_FIRST_2,measure,&Delays.First[1]);
AddMeas(idx++,MEASURE_DELAY_STEP_2,measure,&Delays.Step[1]);
AddMeas(idx++,MEASURE_DELAY_LAST_2,measure,&Delays.Last[1]);
AddMeas(idx++,MEASURE_DELAY_FIRST_3,measure,&Delays.First[2]);
AddMeas(idx++,MEASURE_DELAY_STEP_3,measure,&Delays.Step[2]);
AddMeas(idx++,MEASURE_DELAY_LAST_3,measure,&Delays.Last[2]);
AddMeas(idx++,MEASURE_ADDITIONAL_SCAN,measure,&Acquisition.OtherScanTst);
AddMeas(idx++,MEASURE_DELAY_BKG_FIRST,measure,&Delays.BkgFirst);
AddMeas(idx++,MEASURE_DELAY_BKG_STEP,measure,&Delays.BkgStep);
AddMeas(idx++,MEASURE_DELAY_BKG_LAST,measure,&Delays.BkgLast);
AddMeas(idx++,MEASURE_BKG_SCAN,measure,&Acquisition.BkgScan);

AddMeas(idx++,MEASURE_DELAY_1,measure,&Delays.Subset[0]);
AddMeas(idx++,MEASURE_DELAY_2,measure,&Delays.Subset[1]);
AddMeas(idx++,MEASURE_DELAY_3,measure,&Delays.Subset[2]);
AddMeas(idx++,MEASURE_DELAY_4,measure,&Delays.Subset[3]);
AddMeas(idx++,MEASURE_DELAY_5,measure,&Delays.Subset[4]);
AddMeas(idx++,MEASURE_DELAY_6,measure,&Delays.Subset[5]);
AddMeas(idx++,MEASURE_DELAY_7,measure,&Delays.Subset[6]);
AddMeas(idx++,MEASURE_DELAY_8,measure,&Delays.Subset[7]);
AddMeas(idx++,MEASURE_DELAY_9,measure,&Delays.Subset[8]);
AddMeas(idx++,MEASURE_DELAY_10,measure,&Delays.Subset[9]);
AddMeas(idx++,MEASURE_DELAY_11,measure,&Delays.Subset[10]);
AddMeas(idx++,MEASURE_DELAY_12,measure,&Delays.Subset[11]);

AddMeas(idx++,MEASURE_DELAY_AUTOVALUEFILE,measure,&Delays.AutoValueFile);
AddMeas(idx++,MEASURE_NUMDELAY_AUTO,measure,&Delays.NumDelaysAuto);
NumMeas=idx;

AddMeas(idx++,SMALLPANEL_DELAY,smallpanel,&Delays.Set);
AddMeas(idx++,SMALLPANEL_GAIN,smallpanel,&Gain.Set);
AddMeas(idx++,SMALLPANEL_GRAY_HIGH,smallpanel,&Ccd.grayH);
AddMeas(idx++,SMALLPANEL_GRAY_LOW,smallpanel,&Ccd.grayL);
AddMeas(idx++,SMALLPANEL_GRAY_AUTO,smallpanel,&Ccd.displayGrayLevelAuto);


NumSmallPnl=idx;

AddMeas(idx++,D150PANEL_RANGE,d150panel,&del150.range);
AddMeas(idx++,D150PANEL_TRIGGERSLOPE,d150panel,&del150.triggerSlope);
AddMeas(idx++,D150PANEL_TRIGGERTHR,d150panel,&del150.triggerLevel);
AddMeas(idx++,D150PANEL_INVERT,d150panel,&del150.invertedOutput);

NumD150Pnl=idx;

AddMeas(idx++,STFORD_PNL_TRIG_EXTERN,stanfordPnl,&stanford.triggerExternal);
AddMeas(idx++,STFORD_PNL_TRIGGERSLOPE,stanfordPnl,&stanford.triggerSlope);
AddMeas(idx++,STFORD_PNL_TRIGGERTHR,stanfordPnl,&stanford.triggerLevel);
AddMeas(idx++,STFORD_PNL_INVERT,stanfordPnl,&stanford.invertedOutput);

NumStanfordPnl=idx;
NumCtrl=idx;

}


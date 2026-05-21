#include <userint.h>
#include <ansi_c.h>
#include "master.h"
#include "pvcam.h"
#include "R6_utilities.h"


int IsParamAvailable(int16 hCam, uns32 paramID)
{
    rs_bool isAvailable;
    if (PV_OK != pl_get_param(hCam, paramID, ATTR_AVAIL, (void *)&isAvailable))
		return -1;
    
    if (isAvailable == FALSE)
        return -1;
	
    return 1;
}

int GetParamValue(int16 hCam, uns32 paramID, void *currentValue){

	uns16 dataType;
	
	if (!IsParamAvailable(hCam,paramID))
		return -1;
	
	pl_get_param(hCam, paramID, ATTR_TYPE, &dataType);   
	switch (dataType){
		case TYPE_INT16:
			int16 int16_Value;
			pl_get_param(hCam, paramID, ATTR_CURRENT, &int16_Value);  
			*((int16*)currentValue)=int16_Value;  
			break;
			
		case TYPE_UNS16:
			uns16 uns16_Value; 
			pl_get_param(hCam, paramID, ATTR_CURRENT, &uns16_Value);     
			*((uns16*)currentValue)=uns16_Value;  
			break;
			
		case TYPE_BOOLEAN:  
			rs_bool bool_Value=0;  
			pl_get_param(hCam, paramID, ATTR_CURRENT, &bool_Value);
			*((rs_bool*)currentValue)=bool_Value;  

			break;	
		case TYPE_UNS32:  
			uns32 uns32_Value=0;  
			pl_get_param(hCam, paramID, ATTR_CURRENT, &uns32_Value);
			*((uns32*)currentValue)=uns32_Value;  
			break; 
		/*
		case 0:  
			ulong64 ulong64_Value=0;  
			pl_get_param(hCam, paramID, ATTR_CURRENT, &ulong64_Value);
			*((ulong64*)currentValue)=ulong64_Value;  
			break;	
		*/
		
	}
	
	return 1;
	
}

int GetEnumParamValue(int16 hCam, uns32 paramID, char *descriptionTxt){

	uns32 index;		 
	int32 value;
	
	
	if (!IsParamAvailable(hCam,paramID)) 
		return -1;
		pl_get_param(hCam, paramID, ATTR_CURRENT, &index);  
		pl_get_enum_param(hCam, paramID, index, &value, descriptionTxt,1000);
		return 1;
	
}



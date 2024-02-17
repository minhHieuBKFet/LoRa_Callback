#include "DataStructure.h"
#include <cmath>

ERROR_CODE creatCalibDataString(char *_calibDataString, struct calibData _calibData)
{
	sprintf(_calibDataString, "%u|%u|%u\n",
							  _calibData.temperature_calibInt_u32,
							  _calibData.depth_calibInt_u32,
                              _calibData.DO_value_calibInt_u32 );
	if (_calibDataString[0] == '\0')
	{
		log_e("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {    
        log_i("%s",_calibDataString);

		log_i("Create string complete!");
		return ERROR_NONE;
	}
}


ERROR_CODE createSensorDataString(char *_sensorDataString,
                                  const char *_nameDevice,
                                  const char *_dateTimeString,
                                  struct sensorData _sensorData_st)
{
    strcpy(_sensorDataString, "");
    sprintf(_sensorDataString, "%s,%.1f,%.1f,%.1f,%.1f,%u,",
                                _dateTimeString,
								_sensorData_st.lat_f,
								_sensorData_st.lon_f,
                                _sensorData_st.temperature,
                                _sensorData_st.depth,
                                _sensorData_st.DO_value);
    
	if (_sensorDataString[0] == '\0')
	{
		log_e("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {
        
        log_i("%s",_sensorDataString);
		log_i("Create string complete!");
		return ERROR_NONE;
	}
}

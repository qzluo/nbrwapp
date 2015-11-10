/********************************************************************
 File: redWoodDataParse.h
 Function: Parse the get redwood data to a special format.     
 Author: Luobihe
 Date: 2015-6-29
*********************************************************************/

#ifndef __REDWOODDATAPARSE_H__
#define __REDWOODDATAPARSE_H__

#include "cJSON.h"

#define TRANSER_JSON_VERSION  "1.1"

/*
Parse the data from redwood device.
General some point in the data and packet them in a
special format to transfer
*/
class CRedWoodDataParse {
private:
    cJSON *rawJson;
    cJSON *oldDataJson;
    cJSON *updatedDataJson;
    char* pParseData;
    char* pAllParseData;

private:
    //由输入字符串生成完整的json数据
    int ParseFromStr(const char *value);

    //更新数据并保存到 updateDataJson 中，原有的数据转移到 oldDataJson
    int UpdateData(void);

    //根据需求，产生要输出的文本字符串
    int GenerateOutData(void);

    //add sensor data
    int AddSensorDataToJson(cJSON* sensorJson, cJSON* descJson);

    //由 json 产生输出文本到 pParseData
    int GenerateOutDataFromJson(cJSON* json);

    //由 json 产生输出文本到 pAllParseData
    int GenerateFullOutDataFromJson(cJSON* json);

    void testAddSensorData();
    void testUpdateData();
    void testGenerateOutData();

public:
    CRedWoodDataParse(void);
    ~ CRedWoodDataParse(void);

    //清理资源
    void Free();

    //由输入字符串生成要输出的字符串
    int ParseRedWoodDataFromStr(const char* pRWStr);  

    const char* GetParseData(void) {return pParseData;}

    const char* GetAllParseData(void) { return pAllParseData;}

    void Test();

};

#endif  //__REDWOODDATAPARSE_H__
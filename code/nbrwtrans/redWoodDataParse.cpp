/********************************************************************
 File: redWoodDataParse.cpp
 Function: Parse the get redwood data to a special format.     
 Author: Luobihe
 Date: 2015-6-29
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "redWoodDataParse.h"    


CRedWoodDataParse::CRedWoodDataParse(void)
{
    rawJson         = NULL;
    oldDataJson     = NULL;
    updatedDataJson = NULL;    
    pParseData      = NULL;
    pAllParseData   = NULL;
}

CRedWoodDataParse::~ CRedWoodDataParse(void)
{
    Free();
}                    

void CRedWoodDataParse::Free()
{
    if (rawJson) {
        cJSON_Delete(rawJson);
        rawJson = NULL;
    }

    if (oldDataJson) {
        cJSON_Delete(oldDataJson);
        oldDataJson = NULL;
    }

    if (updatedDataJson) {
        cJSON_Delete(updatedDataJson);
        updatedDataJson = NULL;
    }

    if (pParseData) {
        free(pParseData);
        pParseData = NULL;
    }

    if (pAllParseData) {
        free(pAllParseData);
        pAllParseData = NULL;
    }
}

int CRedWoodDataParse::ParseRedWoodDataFromStr(const char* pRWStr)
{
    //parse the redwood data to json
    if (ParseFromStr(pRWStr))
        return -1;

    //update the new data in json
    if (UpdateData())
        return -1;

    //generate the output data in string
    if (GenerateOutData())
        return -1;
    
    return 0;
}

int CRedWoodDataParse::ParseFromStr(const char *value)
{
    if (value == NULL)
        return -1;

    if (rawJson) {
        cJSON_Delete(rawJson);
        rawJson = NULL;
    }

    rawJson = cJSON_Parse(value);
    if (!rawJson)
        return -1;
    
    return 0;
}

/*-------------------------------------------------------------------
Function: UpdateData();
Purpose: Upatate json data from raw json. For user, ParseFromStr 
         should be called before called this function.
Parameters: 
Return: 0 --- successful, -1 --- failed
-------------------------------------------------------------------*/
int CRedWoodDataParse::UpdateData(void)
{
    if (!rawJson)
        return -1;

    //save data gotten last time
    if (updatedDataJson) {
        if (oldDataJson) {
            cJSON_Delete(oldDataJson);
            oldDataJson = NULL;
        }

        oldDataJson = updatedDataJson;
    }
    
    //create new json from raw data
    cJSON* fixtureJson = NULL;    //son 'fuxture' of rawjson
    int iDataCount = 0;
    cJSON* srcJson = NULL;
    cJSON* descJson = NULL;       //as variable to save fixture
    cJSON* srcJsonTemp = NULL;
    cJSON* descJsonTemp = NULL;

    fixtureJson = cJSON_GetObjectItem(rawJson, "fixture");
    if (!fixtureJson)
        return -1;

    iDataCount = cJSON_GetArraySize(fixtureJson); 

    updatedDataJson = cJSON_CreateObject();	
    cJSON_AddStringToObject(updatedDataJson, "version", TRANSER_JSON_VERSION);
    cJSON_AddNumberToObject(updatedDataJson, "data length", iDataCount);
    cJSON_AddNumberToObject(updatedDataJson, "currentTime", 
                            cJSON_GetObjectItem(rawJson, "currentTime")->valueint);

    cJSON_AddItemToObject(updatedDataJson, "fixture",  
        descJson = cJSON_CreateArray());

    //create each sub json
    for (int i = 0; i < iDataCount; i++) {        
        srcJsonTemp = cJSON_GetArrayItem(fixtureJson, i);
        descJsonTemp = cJSON_Duplicate(srcJsonTemp, 1);
        cJSON_AddItemToArray(descJson, descJsonTemp);

        //将要保存目标结点的 sensorStats 节点删除
        cJSON_DeleteItemFromObject(descJsonTemp, "sensorStats");

        //将原有数据的 sensorStats 节点中的数据经转化保存到 目标结点
        AddSensorDataToJson(cJSON_GetObjectItem(srcJsonTemp, "sensorStats"), descJsonTemp);        
    }

    GenerateFullOutDataFromJson(updatedDataJson);

    return 0;
}

/*-------------------------------------------------------------------
Function: GenerateOutData();
Purpose: 通过比较现有数据跟原有数据，产生要输出的字符串.
Parameters: 
Return: 0 --- successful, -1 --- failed
-------------------------------------------------------------------*/
int CRedWoodDataParse::GenerateOutData(void)
{
    int iRet = 0;

    if (!oldDataJson) 
        return GenerateOutDataFromJson(updatedDataJson);
    else {
        assert(updatedDataJson);

        cJSON* outJson = NULL;
        outJson = cJSON_Duplicate(updatedDataJson, 1);
        if (!outJson)
            return -1;

        cJSON* newJsonFixture = NULL;
        cJSON* oldJsonFixture = NULL;

        newJsonFixture = cJSON_GetObjectItem(outJson, "fixture");
        oldJsonFixture = cJSON_GetObjectItem(oldDataJson, "fixture");

        if (!newJsonFixture || !oldJsonFixture) {
            cJSON_Delete(outJson);
            return -1;
        }        

        int iDataLen = 0;
        iDataLen = cJSON_GetArraySize(newJsonFixture); 

        assert(iDataLen == cJSON_GetArraySize(oldJsonFixture));

        //比较 fixture 节点的 每个子节点，如果相同，则从输出中删除
        for (int i = iDataLen - 1; i >= 0; i--) {
            cJSON* oldJsonTemp = cJSON_GetArrayItem(oldJsonFixture, i);
            cJSON* newJsonTemp = cJSON_GetArrayItem(newJsonFixture, i);

            char* pOldDataTemp = cJSON_Print(oldJsonTemp);
            char* pNewDataTemp = cJSON_Print(newJsonTemp);

            if (strcmp(pOldDataTemp, pNewDataTemp) == 0)
                cJSON_DeleteItemFromArray(newJsonFixture, i);

            free(pOldDataTemp);
            free(pNewDataTemp);
        }

        //改变输出的数据个数
        if (iDataLen != cJSON_GetArraySize(newJsonFixture)) {
            iDataLen = cJSON_GetArraySize(newJsonFixture); 
            cJSON_ReplaceItemInObject(outJson, "data length", cJSON_CreateNumber(iDataLen));
        }

        //生成目标字符串，以便输出
        iRet = GenerateOutDataFromJson(outJson);
        cJSON_Delete(outJson);
    }

    return iRet;
}

/*-------------------------------------------------------------------
Function: GenerateOutDataFromJson(json);
Purpose: 由输入的json 生成要传输的文本字符串,并保存到pParseData.
Parameters: json -- [IN], 待生成的 json
Return: 0 --- successful, -1 --- failed
-------------------------------------------------------------------*/
int CRedWoodDataParse::GenerateOutDataFromJson(cJSON* json)
{
    if (!json)
        return -1;

    if (pParseData) {
        free(pParseData);
        pParseData = NULL;
    }

    pParseData = cJSON_Print(json);

    return 0;
}

/*-------------------------------------------------------------------
Function: GenerateOutDataFromJson(json);
Purpose: 由输入的json 生成要传输的文本字符串,并保存到pAllParseData.
Parameters: json -- [IN], 待生成的 json
Return: 0 --- successful, -1 --- failed
-------------------------------------------------------------------*/
int CRedWoodDataParse::GenerateFullOutDataFromJson(cJSON* json)
{
    if (!json)
        return -1;

    if (pAllParseData) {
        free(pAllParseData);
        pAllParseData = NULL;
    }

    pAllParseData = cJSON_Print(json);

    return 0;
}

int CRedWoodDataParse::AddSensorDataToJson(cJSON* sensorJson, cJSON* descJson)
{
    if (sensorJson == NULL)
        return -1;

    cJSON* jsonSon        = NULL;
    cJSON* jsonGrandChild = NULL;
    jsonSon = sensorJson->child;

    while (jsonSon) {
        jsonGrandChild = jsonSon->child;

        if (jsonGrandChild) {
            //get grandchild value
            switch (jsonGrandChild->type) {
            case cJSON_Number:
                cJSON_AddNumberToObject(descJson, 
                    jsonSon->string, jsonGrandChild->valuedouble);
                break;

            case cJSON_String:
                cJSON_AddStringToObject(descJson, 
                    jsonSon->string, jsonGrandChild->valuestring);
                break;

            default:
                break;
            }
        }
        else {
            cJSON_AddNullToObject(descJson, jsonSon->string);
        }

        jsonSon = jsonSon->next;
    }

    return 0;
}

void CRedWoodDataParse::testAddSensorData()
{
    FILE *f;long len;char *data;

    f=fopen("json.dat","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
    data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);

    char *out;cJSON *json, *newjson;
    json=cJSON_Parse(data);

    newjson = cJSON_CreateObject();
    AddSensorDataToJson(cJSON_GetObjectItem(json, "sensorStats"), newjson);

    out=cJSON_Print(newjson);
    cJSON_Delete(json);
    cJSON_Delete(newjson);
    printf("%s\n",out);
    free(out);

    free(data);

}

void CRedWoodDataParse::testUpdateData()
{
    FILE *f;long len;char *data;

    f=fopen("json.dat","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
    data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);

    ParseFromStr(data);

    UpdateData();

    
    GenerateOutDataFromJson(updatedDataJson);
    printf(pParseData);
}

void CRedWoodDataParse::testGenerateOutData()
{
    FILE *f;long len;char *data;

    f=fopen("json.dat","rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
    data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);

    ParseFromStr(data);

    UpdateData();
    UpdateData();

    
    GenerateOutData();
    printf(pParseData);

}

void CRedWoodDataParse::Test()
{
    //testAddSensorData();

    testGenerateOutData();

   // UpdateData();
}
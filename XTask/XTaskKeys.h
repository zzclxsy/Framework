#ifndef XTASKKEYS_H
#define XTASKKEYS_H
#include "string"

const std::string TASK_KEY_ID = "testId";
const std::string TASK_KEY_NAME = "testName";

const std::string REC_TASK_STATUS = "taskState";//finish, clear,start

//*****************回复类型标识**********************
//key
const std::string RRPLY_TASK_TYPE = "type";
//value
const std::string task_parse = "taskParse";//任务解析
const std::string task_start = "taskStart";//任务开始
const std::string task_updata = "taskUdpata";//任务执行中
const std::string task_finished = "taskFinished";//任务结束
const std::string task_burst_error = "taskBurstError";//任务突发错误

//key
const std::string RRPLY_TASK_STATUS = "succeed";//yes or no
const std::string RRPLY_TASK_RESULT = "taskResult";//结果
const std::string RRPLY_TASK_ERROR = "error"; //错误原因

const std::string no_data = "null";

/*                  回复JSON数据包格式
 *          Data[TASK_KEY_ID]       任务id
 *          Data[RRPLY_TASK_TYPE]   任务回复类型
 *          Data[RRPLY_TASK_STATUS]       任务状态是成功还是失败
 *          Data[RRPLY_TASK_RESULT]       任务结果
 *          Data[RRPLY_TASK_ERROR]        任务错误原因
 */
#endif // XTASKKEYS_H

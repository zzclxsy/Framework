#ifndef XTASKKEYS_H
#define XTASKKEYS_H
#include "string"

const std::string TASK_KEY_ID = "testId";
const std::string TASK_KEY_STATUS = "status";

const std::string TASK_STATUS_OPEN = "open";
const std::string TASK_STATUS_PROCESSING = "processing";
const std::string TASK_STATUS_CLOSED = "closed";
const std::string TASK_STATUS_CANCELED = "canceled";

const std::string UPDATE_TIME = "updateTime";

const std::string TASK_ID_CLOSE_PATH = "/test/task/json_close";
const std::string TASK_ID_UPDATE_PATH = "/test/task/json_update";
const std::string TASK_ID_START_PATH = "/test/task/start";

#endif // XTASKKEYS_H

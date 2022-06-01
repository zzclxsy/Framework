#ifndef XHTTPKEYS_DEF
#define XHTTPKEYS_DEF


#include <string>

// HTTP DATA PADDING
const std::string HTTP_DATA_PADDING = "&vendor=phypower";
const std::string HTTP_MULTIPART_BOUNDARY_BASE = "1+2=vEn3Dor4Ph5YpO6We8R79D0aVx>Mc<jKz*X";
// CONFIG KEYS
// BASE KEYS
const std::string COLOR_KEY = "color";
const std::string SLOT_ID = "slot";
const std::string BIB_ID = "bibId";
// DATA RECORD PATH KEYS
const std::string DUT_LOG_PATH = "/log/dut/commit";
const std::string DUT_JSON_LOG_PATH = "/log/dut/json_commit";
const std::string STATUS_LOG_PATH = "/slot/state/update";
const std::string TYPED_LOG_PATH = "/log/typed/commit";
const std::string TYPED_JSON_LOG_PATH = "/log/typed/json_commit";
const std::string TEST_ITEM_LOG_PATH = "/slot/test_items/update";
// data format def info
const std::string DUT_INDEX_SIZE = "dutIndexSize";

// http rsp type info keys
// task deal info path
const std::string SLOT_TASKS_QUERY_PATH = "/test/task/query2";
const std::string TASK_ID_QUERY_PATH = "/test/task/query1";
const std::string TASK_ID_CREATE_PATH = "/test/task/create";
const std::string TASK_ID_UPDATE_PATH = "/test/task/update";
const std::string TASK_ID_START_PATH = "/test/task/start";
const std::string TASK_ID_CLOSE_PATH = "/test/task/close";
const std::string TASK_TEST_ID_KEY = "testId";
const std::string TASK_NEW_TESTID_VALUE = "open";
//
const std::string TASK_RESLUT_KEY = "result";

namespace TEST_BIB_STATUS
{
	const std::string ONLINE_KEY = "online";
	const std::string TESTING_KEY = "testing";
	const std::string BIB_READY_KEY = "ready";
} // namespace TEST_BIB_STATUS


#endif // !  JHHTTPKEYS_DEF

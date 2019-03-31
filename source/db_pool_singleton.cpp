#include "db_pool_singleton.hpp"

std::unique_ptr<DbPoolSingleton> DbPoolSingleton::_instance;
std::once_flag DbPoolSingleton::_onceFlag;
ConnPoolConfig DbPoolSingleton::_config;
#include "db_pool_singleton.hpp"

std::unique_ptr<DbPoolSingleton> DbPoolSingleton::_instance = 0;
std::unique_ptr<mongocxx::pool> DbPoolSingleton::_conn_pool;
ConnPoolConfig DbPoolSingleton::_config;

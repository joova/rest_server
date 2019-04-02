#include "db_pool_singleton.hpp"

std::unique_ptr<DbPoolSingleton> DbPoolSingleton::_instance = nullptr;
std::unique_ptr<mongocxx::pool> DbPoolSingleton::_conn_pool;
ConnPoolConfig DbPoolSingleton::_config;

DbPoolSingleton::DbPoolSingleton() {
    mongocxx::instance instance{};
    _config = ConnPoolConfig("../conf/store.json");
    _conn_pool = std::unique_ptr<mongocxx::pool> (
            new mongocxx::pool(mongocxx::uri{_config.conn_str()})
        );
}

DbPoolSingleton & DbPoolSingleton::GetInstance() {
    if(!_instance)
        _instance.reset(new DbPoolSingleton());

    return *_instance; 
}

mongocxx::pool & DbPoolSingleton::connection() const {
    return *_conn_pool;
}

ConnPoolConfig & DbPoolSingleton::config(){
    return _config;
}

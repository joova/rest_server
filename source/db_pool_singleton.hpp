#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>

#include "conn_pool_config.hpp"

class DbPoolSingleton {
    static std::unique_ptr<DbPoolSingleton> _instance;
    static std::once_flag _onceFlag;

    std::unique_ptr<mongocxx::pool> _connPool;

    DbPoolSingleton(const ConnPoolConfig & config) {
        mongocxx::instance instance{};
        _connPool = std::unique_ptr<mongocxx::pool> (new mongocxx::pool(mongocxx::uri{_config.connstr()}));
    }

public:
    static ConnPoolConfig _config;
    static DbPoolSingleton & shared() {
        std::call_once(_onceFlag,[]{
            _instance.reset(new DbPoolSingleton(_config));
        });

        return *_instance; 
    }

    mongocxx::pool & connection() const {
        return *_connPool;
    }
};
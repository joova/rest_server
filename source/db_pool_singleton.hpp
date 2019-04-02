#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>

#include "conn_pool_config.hpp"

class DbPoolSingleton {
private:
    static std::unique_ptr<DbPoolSingleton> _instance;
    static std::unique_ptr<mongocxx::pool> _conn_pool;
    static ConnPoolConfig _config;

    DbPoolSingleton() {
        mongocxx::instance instance{};
        _config = ConnPoolConfig("../conf/store.json");
        _conn_pool = std::unique_ptr<mongocxx::pool> (
                new mongocxx::pool(mongocxx::uri{_config.conn_str()})
            );
    }

public:
    static DbPoolSingleton & GetInstance() {
        if(_instance == 0)
            _instance.reset(new DbPoolSingleton());

        return *_instance; 
    }

    mongocxx::pool & connection() const {
        return *_conn_pool;
    }

    ConnPoolConfig & config(){
        return _config;
    }
};
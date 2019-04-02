#ifndef REST_SERVER_DB_POOL_SINGLETON_H_
#define REST_SERVER_DB_POOL_SINGLETON_H_

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>

#include "conn_pool_config.hpp"

class DbPoolSingleton {

public:
    static DbPoolSingleton & GetInstance();
    mongocxx::pool & connection() const;
    ConnPoolConfig & config();

private:
    static std::unique_ptr<DbPoolSingleton> _instance;
    static std::unique_ptr<mongocxx::pool> _conn_pool;
    static ConnPoolConfig _config;

    DbPoolSingleton();
};

#endif // REST_SERVER_DB_POOL_SINGLETON_H_
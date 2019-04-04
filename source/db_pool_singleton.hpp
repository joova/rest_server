#ifndef REST_SERVER_DB_POOL_SINGLETON_H_
#define REST_SERVER_DB_POOL_SINGLETON_H_

#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>

#include "pool_config.hpp"

using connection = mongocxx::pool::entry;

class DbPoolSingleton {

public:
    static DbPoolSingleton & instance();
    connection connection();

    void configure(const PoolConfig & config);
    PoolConfig config();

private:
    std::unique_ptr<mongocxx::instance> _instance = nullptr;
    std::unique_ptr<mongocxx::pool> _pool = nullptr;
    PoolConfig _config;

    DbPoolSingleton();
};

class NoopLogger : public mongocxx::logger {
    public:
    virtual void operator()(mongocxx::log_level,
                            bsoncxx::stdx::string_view,
                            bsoncxx::stdx::string_view) noexcept {}
};

#endif // REST_SERVER_DB_POOL_SINGLETON_H_
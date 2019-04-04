#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include "db_pool_singleton.hpp"


using connection = mongocxx::pool::entry;

DbPoolSingleton::DbPoolSingleton(){
    PoolConfig config = PoolConfig("../conf/store.json");
    configure(config);
}

void DbPoolSingleton::configure(const PoolConfig & config) {
    BOOST_LOG_TRIVIAL(debug) << "DbPoolSingleton::configure : config.conn_str() = " << config.conn_str();
    

    auto instance = bsoncxx::stdx::make_unique<mongocxx::instance>(
        bsoncxx::stdx::make_unique<NoopLogger>());
    auto pool = bsoncxx::stdx::make_unique<mongocxx::pool>(
        std::move(mongocxx::uri{config.conn_str()}));

    _config = config;
    _instance = std::move(instance);
    _pool = std::move(pool);
}

DbPoolSingleton & DbPoolSingleton::instance() {
    BOOST_LOG_TRIVIAL(debug) << "DbPoolSingleton::instance : ... ";
    static DbPoolSingleton instance;
    return instance;
}

connection DbPoolSingleton::connection() {
    BOOST_LOG_TRIVIAL(debug) << "DbPoolSingleton::connection : ... ";
    return _pool->acquire();
}

PoolConfig DbPoolSingleton::config() {
    BOOST_LOG_TRIVIAL(debug) << "DbPoolSingleton::config : ... ";
    return _config;
}

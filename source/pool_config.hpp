#ifndef REST_SERVER_CONN_POOL_CONFIG_H_
#define REST_SERVER_CONN_POOL_CONFIG_H_


#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class PoolConfig {
    std::string _conn_str;
    std::string _database;

public:
    PoolConfig() : _conn_str(""), _database("") {}
    PoolConfig(const std::string &confPath) {
        using boost::property_tree::ptree;
        
        ptree conf;
        read_json(confPath, conf);
        
        _conn_str = conf.get<std::string>("store.connstr");
        _database = conf.get<std::string>("store.database");
    }

    std::string conn_str() const { return _conn_str; }
    std::string database() const { return _database; }
};

#endif // REST_SERVER_CONN_POOL_CONFIG_H_
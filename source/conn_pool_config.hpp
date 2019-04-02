#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class ConnPoolConfig {
    std::string _conn_str;
    std::string _database;

public:
    ConnPoolConfig() : _conn_str(""), _database("") {}
    ConnPoolConfig(const std::string &confPath) {
        using boost::property_tree::ptree;
        
        ptree conf;
        read_json(confPath, conf);
        
        _conn_str = conf.get<std::string>("store.connstr");
        _database = conf.get<std::string>("store.database");
    }

    std::string conn_str() const { return _conn_str; }
    std::string database() const { return _database; }
};
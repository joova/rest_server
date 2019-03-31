#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class ConnPoolConfig {
    std::string _connStr;
    std::string _database;

public:
    ConnPoolConfig() : _connStr(""), _database("") {}
    ConnPoolConfig(const std::string &confPath) {
        using boost::property_tree::ptree;
        
        ptree conf;
        read_json(confPath, conf);
        
        _connStr = conf.get<std::string>("store.connstr");
        _database = conf.get<std::string>("store.database");
    }

    std::string connstr() const { return _connStr; }
    std::string database() const { return _database; }
};
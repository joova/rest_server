#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <cpprest/http_client.h>

#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>

#include "pool_config.hpp"
#include "db_pool_singleton.hpp"
#include "microsvc_controller.hpp"
#include "interupt_handler.hpp"

using namespace web; 

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

// fungsi untuk inisialisasi log
void init_logging() 
{
   logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
   logging::add_file_log(
       "../logs/server.log",
       keywords::auto_flush = true,
       keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
   );
   logging::core::get()->set_filter(
       logging::trivial::severity >= logging::trivial::debug
   );
   logging::add_common_attributes();
}

// config database connection pool
void configure() {
    BOOST_LOG_TRIVIAL(debug) << "config connection pool : ../conf/store.json";
    PoolConfig config = PoolConfig("../conf/store.json");
    DbPoolSingleton::instance().configure(config);
}

// fungsi main
int main()
{
    //Inisialisasi logging
    init_logging();

    //Config database
    //configure();

    InterruptHandler::hookSIGINT();

    MicroServiceController server;
    server.SetEndpoint("http://localhost:6502/api");

    try {
        server.Accept().wait();

        BOOST_LOG_TRIVIAL(info) << "Microservice listening at: " << server.endpoint();
        std::cout << "Microservice listening at: " << server.endpoint() << "\n";

        InterruptHandler::WaitForUserInterrupt();
        server.Shutdown().wait();

        BOOST_LOG_TRIVIAL(info) << "Microservice shutdown";
        std::cout << "Microservice shutdown" << "\n";
    } 
    catch (std::exception & e) {
        BOOST_LOG_TRIVIAL(fatal) << "Server errror: " << e.what();
        std::cerr << "Server errror: " << e.what() << "\n";
    }

    return 0;
}
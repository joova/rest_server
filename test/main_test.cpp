#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <gtest/gtest.h>

#include "pool_config.hpp"
#include "db_pool_singleton.hpp"

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


int main(int argc, char ** argv) {
    init_logging();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
cmake_minimum_required(VERSION 3.7)
project(rest-server)

find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
find_package(Threads REQUIRED)

file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/*)
list(REVERSE OPENSSL_ROOT_DIR)
find_package(OpenSSL 1.0.2 REQUIRED)
find_package(cpprestsdk REQUIRED)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DBOOST_LOG_DYN_LINK")

add_executable( ${PROJECT_NAME} source/db_pool_singleton.cpp
                                source/user_manager.cpp
                                source/basic_controller.cpp
                                source/microsvc_controller.cpp
                                source/main.cpp)

# message(BOOST_LIBS " ${Boost_LIBRARIES}")
# message(OPENSSL_LIBS " ${OPENSSL_LIBRARIES}")
# message(CPPRESTSDK_INCLUDE_DIR " ${CPPRESTSDK_INCLUDE_DIR}")

set(MICROSERVICE_INCLUDE_DIR "./source")

file(GLOB COMMON_LIBRARIES ${LIBMONGOCXX_LIBRARIES} 
                            ${LIBBSONCXX_LIBRARIES}
                            ${OPENSSL_LIBRARIES}
                            ${Boost_LIBRARIES}
                            ${ZIP_LIBRARY})

file(GLOB COMMON_INCLUDES ${LIBMONGOCXX_INCLUDE_DIRS} 
                            ${LIBBSONCXX_INCLUDE_DIRS} 
                            ${Boost_INCLUDE_DIR}
                            ${OPENSSL_INCLUDE_DIR}
                            ${MICROSERVICE_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${COMMON_INCLUDES})
# target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
# target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
# target_include_directories(${PROJECT_NAME} PRIVATE ${MICROSERVICE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${COMMON_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENSSL_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PUBLIC ${ZIP_LIBRARY})

target_link_libraries(${PROJECT_NAME} PRIVATE cpprestsdk::cpprest)

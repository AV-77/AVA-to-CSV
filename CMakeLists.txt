cmake_minimum_required (VERSION 3.8)

project ("AVA-CSV")

set(BOOST_ROOT "C:/local/boost_1_72_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_72_0/lib64-msvc-14.2")

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.72.0 COMPONENTS system filesystem REQUIRED) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable (AVA-CSV "AVA-CSV.cpp" "AVA-CSV.h")
    target_link_libraries(AVA-CSV ${Boost_LIBRARIES}
                         ${Boost_FILESYSTEM_LIBRARY}
                         ${Boost_SYSTEM_LIBRARY})
endif()
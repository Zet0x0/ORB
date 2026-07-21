add_library(common STATIC)

target_sources(common PRIVATE singleton.h utilities.h utilities.cpp)

target_link_libraries(common PUBLIC Qt6::Qml)

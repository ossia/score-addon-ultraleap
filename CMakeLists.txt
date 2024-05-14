cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
    include(ScoreExternalAddon)
endif()

if(NOT TARGET score_plugin_avnd)
    return()
endif()

if(NOT LEAPSDK_PATH)
    return()
endif()

project(score_addon_ultraleap LANGUAGES CXX)
score_common_setup()
include("${CMAKE_CURRENT_SOURCE_DIR}/ultraleap.cmake")

add_library(score_addon_ultraleap
    "${CMAKE_CURRENT_SOURCE_DIR}/score_addon_ultraleap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/Protocol.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionDevice.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionProtocolFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionProtocolSettingsWidget.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionSpecificSettings.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/score_addon_ultraleap.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/Protocol.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionDevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionProtocolFactory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionProtocolSettingsWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UltraLeap/LeapmotionSpecificSettingsSerialization.cpp"
)


target_include_directories(score_addon_ultraleap
  PRIVATE
    3rdparty/ultraleap/src
    "${LEAPC_HEADERS}"
)

target_link_libraries(score_addon_ultraleap
  PUBLIC
    score_plugin_engine
)

setup_score_plugin(score_addon_ultraleap)

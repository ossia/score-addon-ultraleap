cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(EMSCRIPTEN)
  return()
endif()

if(NOT TARGET score_lib_base)
    include(ScoreExternalAddon)
endif()

project(score_addon_ultraleap LANGUAGES CXX)
score_common_setup()

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
    3rdparty/LeapSDK/include
)

target_link_libraries(score_addon_ultraleap
  PUBLIC
    score_plugin_engine
)

if(COMMAND score_register_license)
  score_register_license("Ultraleap LeapC headers" URL "https://central.leapmotion.com/agreements/SdkAgreement"
    NOTICES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/LeapSDK/include/LeapC.h")
endif()

setup_score_plugin(score_addon_ultraleap)

if(SCORE_TESTING AND COMMAND score_add_test)
  # With static plug-ins, APP links score_addon_ultraleap, which already has the
  # serializers: compiling them in as well defines them twice.
  set(_ultraleap_test_sources tests/LeapmotionSettingsTest.cpp)
  if(NOT SCORE_STATIC_PLUGINS)
    list(APPEND _ultraleap_test_sources UltraLeap/LeapmotionSpecificSettingsSerialization.cpp)
  endif()
  score_add_test(score_addon_ultraleap_settings_test
    SOURCES ${_ultraleap_test_sources}
    APP
  )
  target_include_directories(score_addon_ultraleap_settings_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}")
  # score's test fixtures are defined after the add-ons
  target_link_libraries(score_addon_ultraleap_settings_test PRIVATE
    $<TARGET_NAME_IF_EXISTS:score_test_fixtures>)
  # The serializer specialisations are defined in a .cpp: in a unity build the
  # test, which instantiates them, can come first in the same translation unit.
  set_target_properties(score_addon_ultraleap_settings_test PROPERTIES UNITY_BUILD OFF)
endif()

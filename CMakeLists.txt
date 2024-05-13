cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include(ScoreExternalAddon)
endif()

if(NOT TARGET score_plugin_avnd)
  return()
endif()

project(score_addon_ultraleap LANGUAGES CXX)

avnd_score_plugin_init(
  BASE_TARGET score_addon_ultraleap
)

avnd_score_plugin_add(
  BASE_TARGET score_addon_ultraleap
  SOURCES
    UltraLeap/UltraLeap.hpp
    UltraLeap/UltraLeapModel.hpp
    UltraLeap/UltraLeapModel.cpp
    UltraLeap/UltraLeapUi.hpp
  TARGET ultraleap
  MAIN_CLASS UltraLeap
  NAMESPACE Example
)

avnd_score_plugin_finalize(
  BASE_TARGET score_addon_ultraleap
  PLUGIN_VERSION 1
  PLUGIN_UUID "d6ac02a7-a9a8-4b6d-a419-c18a63f65a12"
)

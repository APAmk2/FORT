# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("FORT")

set(IMGUI_PATH "3rdparty/imgui")
set(IMGUI_INCLUDE
  ${IMGUI_PATH}/imgui.cpp
  ${IMGUI_PATH}/imgui_draw.cpp
  ${IMGUI_PATH}/imgui_widgets.cpp
  ${IMGUI_PATH}/imgui_demo.cpp
  ${IMGUI_PATH}/imgui_tables.cpp
  ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
  ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer2.cpp
  ${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
)

set(SDL2_DIR "3rdparty/SDL2-2.30.6/cmake")
set(ZLIB_ROOT "3rdparty/zlib-1.2.3")
find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)
set(SDL2_LIBRARY "${SDL2_LIBDIR}/SDL2.lib;${SDL2_LIBDIR}/SDL2main.lib")

set(NEW_FONLINE_FORMATS src/Fonline/Fo2D.cpp
					src/Fonline/FoTxtb.cpp)
set(NEW_FONLINE_WINDOWS src/Fonline/Fo2D_win.cpp
					src/Fonline/FoTxtb_win.cpp)

set(FALLOUT_FORMATS src/Fallout/FallShared.cpp
					src/Fallout/FallProto.cpp
					src/Fallout/FallMap.cpp
					src/Fallout/FallFRM.cpp
					src/Fallout/FallRIX.cpp
					src/Fallout/FallMSK.cpp
					)
set(FALLOUT_WINDOWS src/Fallout/FallProto_win.cpp
					src/Fallout/FallMap_win.cpp
					src/Fallout/FallFRM_win.cpp
					src/Fallout/FallRIX_win.cpp
					src/Fallout/FallMSK_win.cpp
					)

set(FT_FORMATS src/FT/FTZar.cpp
			   src/FT/FTTil.cpp)
set(FT_WINDOWS src/FT/FTZar_win.cpp
			   src/FT/FTTil_win.cpp)

add_executable (FORT "src/main.cpp" 
					 "3rdparty/lodepng/lodepng.cpp"
					 "src/Utils/ByteReader.cpp"
					 "src/Utils/MemoryReader.cpp"
					 ${NEW_FONLINE_FORMATS}
					 ${NEW_FONLINE_WINDOWS}
					 ${FALLOUT_FORMATS}
					 ${FALLOUT_WINDOWS}
					 ${FT_FORMATS}
					 ${FT_WINDOWS}
					 ${IMGUI_INCLUDE}
)
set_property(TARGET FORT PROPERTY CXX_STANDARD 20)
set_target_properties(FORT PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/binaries/debug"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/binaries/release")

add_custom_command(TARGET FORT POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_LIBDIR}/SDL2.dll" $<TARGET_FILE_DIR:FORT>)

include_directories(FORT ${SDL2_INCLUDE_DIR} ${IMGUI_PATH} ${IMGUI_PATH}/backends ${IMGUI_PATH}/misc/cpp 3rdparty/lodepng ${ZLIB_INCLUDE_DIRS})
target_link_libraries(FORT ${SDL2_LIBRARY} ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib-1.2.3/lib/zlib.lib)
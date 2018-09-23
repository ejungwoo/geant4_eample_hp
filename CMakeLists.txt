#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(human_phantom)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

# Setup of ROOT ANALYSIS : optional. 
option(WITH_ANALYSIS_USE "Build example with analysis objects" ON)
if(WITH_ANALYSIS_USE)
add_definitions(-DANALYSIS_USE)
else()
endif()

#----------------------------------------------------------------------------
# Setup of GDML : optional. 
option(WITH_GDML_USE "Build example with GDML" OFF)
if(WITH_GDML_USE)
add_definitions(-DG4LIB_USE_GDML)
else()
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${GDML_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(phantom phantom.cc ${sources} ${headers})
EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
target_link_libraries(phantom ${Geant4_LIBRARIES} ${GDML_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build human_phantom. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(human_phantom_SCRIPTS
    adultHead.mac adultMIRDFemale.mac adultMIRDMale.mac adultMIXFemale.mac adultORNLFemale.mac adultORNLMale.mac batch.mac dawnVis.mac default.mac openGLVis.mac vrmlVis.mac primary.mac macro.C 
    h_flux.dat
  )

foreach(_script ${human_phantom_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(human_phantom DEPENDS phantom)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS phantom DESTINATION bin)


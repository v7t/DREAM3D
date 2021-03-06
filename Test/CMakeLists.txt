#--////////////////////////////////////////////////////////////////////////////
#--
#--  Copyright (c) 2015, BlueQuartz Software
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--
#-- This code was partly written under US Air Force Contract FA8650-07-D-5800
#--
#--////////////////////////////////////////////////////////////////////////////

set(DREAM3DTest_SOURCE_DIR ${DREAM3DProj_SOURCE_DIR}/Test)
set(DREAM3DTest_BINARY_DIR ${DREAM3DProj_BINARY_DIR}/Test)

#------------------------------------------------------------------------------
# Include this file from the CMP Project
include(${CMP_SOURCE_DIR}/cmpCMakeMacros.cmake)
include(${SIMPLProj_SOURCE_DIR}/Source/SIMPLib/SIMPLibMacros.cmake)

set(TEST_TEMP_DIR ${DREAM3DTest_BINARY_DIR}/Temp)
# Make sure the directory is created during CMake time
file(MAKE_DIRECTORY ${TEST_TEMP_DIR})


configure_file(${DREAM3DTest_SOURCE_DIR}/TestFileLocations.h.in
               ${DREAM3DTest_BINARY_DIR}/DREAM3DTestFileLocations.h @ONLY IMMEDIATE)


configure_file(${SIMPLProj_SOURCE_DIR}/Resources/UnitTestSupport.hpp
               ${DREAM3DTest_BINARY_DIR}/UnitTestSupport.hpp COPYONLY IMMEDIATE)


## QT5_WRAP_CPP( PreflightVerify_MOC  "${DREAM3DTest_SOURCE_DIR}/PreflightVerify.h")
#set_source_files_properties(${DREAM3DTest_SOURCE_DIR}/PreflightVerify.h PROPERTIES HEADER_FILE_ONLY TRUE)
set(PreFlightTest_SOURCES ${DREAM3DTest_SOURCE_DIR}/PreFlightTest.cpp
                          ${DREAM3DTest_SOURCE_DIR}/PreflightVerify.h
                          ${DREAM3DTest_SOURCE_DIR}/PreflightVerify.cpp
                          )
AddSIMPLUnitTest(TESTNAME PreFlightTest
                  SOURCES ${PreFlightTest_SOURCES}
                  FOLDER 
                    "DREAM3D UnitTests"
                  LINK_LIBRARIES 
                    Qt5::Core H5Support SIMPLib  
                  INCLUDE_DIRS
                    ${DREAM3DTest_BINARY_DIR}
                    ${SIMPLProj_SOURCE_DIR}/Source
                    ${SIMPLProj_BINARY_DIR}
                   )

#----------------------------------------------------------------------------
# Here we are trying to get something together that will run all the PrebuiltPipelines
# pipelines as a sanity check
# Reconstruction Pipelines First
set(PREBUILT_PIPELINES_DIR "PrebuiltPipelines/Workshop")
set(TEST_PIPELINE_LIST_FILE ${DREAM3DTest_BINARY_DIR}/PipelineRunnerTest.txt)
set(TEST_DATA_DIR ${DREAM3D_DATA_DIR})

set(PREBUILT_PIPELINE_NAMES
"EBSD Reconstruction/(01) SmallIN100 Archive"
"EBSD Reconstruction/(02) SmallIN100 Initial Visualization"
"EBSD Reconstruction/(03) SmallIN100 Alignment"
"EBSD Reconstruction/(04) SmallIN100 Presegmentation Processing"
"EBSD Reconstruction/(05) SmallIN100 Segmentation"
"EBSD Reconstruction/(06) SmallIN100 Postsegmentation Processing"
"EBSD Reconstruction/(07) SmallIN100 Final Processing"
"EBSD Reconstruction/(08) SmallIN100 Full Reconstruction"
"EBSD Statistics/(01) SmallIN100 Morphological Statistics"
"EBSD Statistics/(02) SmallIN100 Export Statistics"
"EBSD Statistics/(03) SmallIN100 Transfer Field Data"
"EBSD Statistics/(04) SmallIN100 Field Threshold"
"EBSD Statistics/(05) SmallIN100 Crystallographic Statistics"
"EBSD SurfaceMeshing/(01) SmallIN100 Quick Mesh"
"EBSD SurfaceMeshing/(02) SmallIN100 Smooth Mesh"
"EBSD SurfaceMeshing/(03) SmallIN100 Mesh Statistics"
"EBSD SurfaceMeshing/(04) SmallIN100 GBCD"
"Synthetic/(01) Single Cubic Phase Equiaxed"
"Synthetic/(02) Single Hexagonal Phase Equiaxed"
"Synthetic/(03) Single Cubic Phase Rolled"
"Synthetic/(04) Two Phase Cubic Hexagonal Particles Equiaxed"
"Synthetic/(05) Composite"
"Synthetic/(06) SmallIN100 Synthetic"
)

if(SIMPL_USE_ITK)
  set(PREBUILT_PIPELINE_NAMES
    ${PREBUILT_PIPELINE_NAMES}
    "Image Reconstruction/(01) Image Initial Visualization"
    "Image Reconstruction/(02) Image Segmentation"
    )
endif()


configure_file(${DREAM3DTest_SOURCE_DIR}/PipelineRunnerTest.h.in
               ${DREAM3DTest_BINARY_DIR}/PipelineRunnerTest.h)

add_executable(PipelineRunnerTest
                ${DREAM3DTest_SOURCE_DIR}/PipelineRunnerTest.cpp ${DREAM3DTest_BINARY_DIR}/PipelineRunnerTest.h)
target_include_directories(PipelineRunnerTest 
                            PUBLIC
                              ${DREAM3DTest_BINARY_DIR}
                              ${SIMPLProj_SOURCE_DIR}/Source
                              ${SIMPLProj_BINARY_DIR})
target_link_libraries(PipelineRunnerTest Qt5::Core EbsdLib SIMPLib)
set_target_properties(PipelineRunnerTest PROPERTIES FOLDER "DREAM3D UnitTests")
add_test(NAME PipelineRunnerTest
          COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PipelineRunnerTest
          WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

FILE(WRITE ${TEST_PIPELINE_LIST_FILE} )

foreach(f ${PREBUILT_PIPELINE_NAMES} )
  set(DREAM3D_PIPELINE_FILE "${DREAM3D_SUPPORT_DIR}/${PREBUILT_PIPELINES_DIR}/${f}.json")
  FILE(APPEND ${TEST_PIPELINE_LIST_FILE} "${DREAM3D_PIPELINE_FILE}\n")
endforeach()

# Some more Testing Only pipelines that can be put here
set(PREBUILT_PIPELINES_DIR "TestPipelines")
set(TEST_PIPELINE_LIST_FILE ${DREAM3DTest_BINARY_DIR}/PipelineRunnerTest.txt)
set(TEST_DATA_DIR ${DREAM3D_DATA_DIR})
set(PREBUILT_PIPELINE_NAMES

)

foreach(f ${PREBUILT_PIPELINE_NAMES} )
  set(DREAM3D_PIPELINE_FILE "${DREAM3D_SUPPORT_DIR}/${PREBUILT_PIPELINES_DIR}/${f}")
  FILE(APPEND ${TEST_PIPELINE_LIST_FILE} "${DREAM3D_PIPELINE_FILE}\n")
endforeach()


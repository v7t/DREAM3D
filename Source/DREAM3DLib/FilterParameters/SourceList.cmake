#--////////////////////////////////////////////////////////////////////////////
#--
#--  Copyright (c) 2011, Michael A. Jackson. BlueQuartz Software
#--  Copyright (c) 2011, Michael Groeber, US Air Force Research Laboratory
#--  All rights reserved.
#--  BSD License: http://www.opensource.org/licenses/bsd-license.html
#--
#-- This code was partly written under US Air Force Contract FA8650-07-D-5800
#--
#--////////////////////////////////////////////////////////////////////////////

set(DREAM3DLib_FilterParameters_HDRS
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/AbstractFilterParametersReader.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/AbstractFilterParametersWriter.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/FilterParameter.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/H5FilterParametersConstants.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/H5FilterParametersReader.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/H5FilterParametersWriter.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/QFilterParametersReader.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/QFilterParametersWriter.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/JsonFilterParametersWriter.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/JsonFilterParametersReader.h
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/DynamicTableData.h
)

set(DREAM3DLib_FilterParameters_SRCS
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/AbstractFilterParametersReader.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/AbstractFilterParametersWriter.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/FilterParameter.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/H5FilterParametersReader.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/H5FilterParametersWriter.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/QFilterParametersReader.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/QFilterParametersWriter.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/JsonFilterParametersWriter.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/JsonFilterParametersReader.cpp
  ${DREAM3DLib_SOURCE_DIR}/FilterParameters/DynamicTableData.cpp
)
cmp_IDE_SOURCE_PROPERTIES( "DREAM3DLib/FilterParameters" "${DREAM3DLib_FilterParameters_HDRS}" "${DREAM3DLib_FilterParameters_SRCS}" "0")
if( ${PROJECT_INSTALL_HEADERS} EQUAL 1 )
    INSTALL (FILES ${DREAM3DLib_FilterParameters_HDRS}
            DESTINATION include/DREAM3D/FilterParameters
            COMPONENT Headers   )
endif()

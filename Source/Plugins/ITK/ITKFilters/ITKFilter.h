/*
 * Your License or Copyright Information can go here
 */

#ifndef _ITKFilter_H_
#define _ITKFilter_H_

#include <string>

#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/Common/AbstractFilter.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

/**
 * @class ITKFilter ITKFilter.h ITK/Code/ITKFilters/ITKFilter.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ITKFilter : public AbstractFilter
{
  public:
    DREAM3D_SHARED_POINTERS(ITKFilter);
    DREAM3D_STATIC_NEW_MACRO(ITKFilter);
    DREAM3D_TYPE_MACRO_SUPER(ITKFilter, AbstractFilter);

    virtual ~ITKFilter();

    /* Place your input parameters here. You can use some of the DREAM3D Macros if you want to
    * DREAM3D_INSTANCE_PROPERTY(float, XRes)
    * DREAM3D_INSTANCE_STRING_PROPERTY(OutputFile)
    */


    /**
    * @brief This returns the group that the filter belonds to. You can select
    * a different group if you want. The string returned here will be displayed
    * in the GUI for the filter
    */
    virtual const std::string getGroupName() { return "ITK"; }

    /**
    * @brief This returns a string that is displayed in the GUI. It should be readable
    * and understandable by humans.
    */
    virtual const std::string getHumanLabel() { return "ITKFilter"; }

    /**
    * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
    * a subgroup. It should be readable and understandable by humans.
    */
    virtual const std::string getSubGroupName() { return "Misc"; }

    /**
    * @brief This method will instantiate all the end user settable options/parameters
    * for this filter
    */
    virtual void setupFilterParameters();

    /**
    * @brief This method will write the options to a file
    * @param writer The writer that is used to write the options to a file
    */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
    * @brief This method will read the options from a file
    * @param reader The reader that is used to read the options from a file
    */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

   /**
    * @brief Reimplemented from @see AbstractFilter class
    */
    virtual void execute();

    /**
    * @brief This function runs some sanity checks on the DataContainer and inputs
    * in an attempt to ensure the filter can process the inputs.
    */
    virtual void preflight();

  protected:
    ITKFilter();

    /**
    * @brief Checks for the appropriate parameter values and availability of
    * arrays in the data container
    * @param preflight
    * @param voxels The number of voxels
    * @param fields The number of fields
    * @param ensembles The number of ensembles
    */
    void dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles);

  private:

    ITKFilter(const ITKFilter&); // Copy Constructor Not Implemented
    void operator=(const ITKFilter&); // Operator '=' Not Implemented
};

#endif /* _ITKFilter_H_ */

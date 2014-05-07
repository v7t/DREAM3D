/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "EbsdTransform.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdTransform::EbsdTransform()

{

}


Ebsd::EbsdToSampleCoordinateMapping EbsdTransform::IdentifyStandardTransformation(AxisAngleInput_t sampleTransformation, AxisAngleInput_t eulerTransformation)
{



    // TSL/EDAX
    if (      sampleTransformation.angle == 180.0f
              && sampleTransformation.h == 0.0f
              && sampleTransformation.k == 1.0f
              && sampleTransformation.l == 0.0f


              && eulerTransformation.angle == 90.0f
              && eulerTransformation.h == 0.0f
              && eulerTransformation.k == 0.0f
              && eulerTransformation.l == 1.0f
              )


    {
        return Ebsd::TSLdefault;
    }

    else if (       sampleTransformation.angle == 180.0f    // HKL
                      && sampleTransformation.h == 0.0f
                      && sampleTransformation.k == 1.0f
                      && sampleTransformation.l == 0.0f

                      && eulerTransformation.angle == 0.0f
                      && eulerTransformation.h == 0.0f
                      && eulerTransformation.k == 0.0f
                      && eulerTransformation.l == 1.0f
                      )

    {
        return Ebsd::HKLdefault;
    }


    else if (       sampleTransformation.angle == 0.0f     // HEDM
                    && sampleTransformation.h == 0.0f
                    && sampleTransformation.k == 0.0f
                    && sampleTransformation.l == 1.0f

                    && eulerTransformation.angle == 0.0f
                    && eulerTransformation.h == 0.0f
                    && eulerTransformation.k == 0.0f
                    && eulerTransformation.l == 1.0f
                    )

    {
       return Ebsd::HEDMdefault;
    }


    else {return Ebsd::UnknownCoordinateMapping;}


}




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdTransform::~EbsdTransform()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//void EbsdTransform::appendOriginalHeader(const QString& more)
//{
//  m_OriginalHeader.append(more);
//}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

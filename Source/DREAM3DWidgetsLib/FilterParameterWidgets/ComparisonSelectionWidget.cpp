/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
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

#include "ComparisonSelectionWidget.h"


#include "DREAM3DWidgetsLib/FilterParameterWidgets/moc_ComparisonSelectionWidget.cxx"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ComparisonSelectionWidget::ComparisonSelectionWidget(FilterParameter* parameter, AbstractFilter* filter, QWidget* parent) :
  QWidget(parent),
  m_ShowOperators(true),
  m_Filter(filter),
  m_FilterParameter(parameter),
  m_DidCausePreflight(false),
  m_ComparisonSelectionTableModel(NULL)
{
  setupUi(this);
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ComparisonSelectionWidget::~ComparisonSelectionWidget()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QVector<ComparisonInput_t> ComparisonSelectionWidget::getComparisonInputs()
{
  QVector<ComparisonInput_t> comps;
  if (m_ComparisonSelectionTableModel == NULL) { return comps; }

  int filterCount = m_ComparisonSelectionTableModel->rowCount();
  QVector<QString> featureNames;
  QVector<float> featureValues;
  QVector<int> featureOperators;
  m_ComparisonSelectionTableModel->getTableData(featureNames, featureValues, featureOperators);

  for(int i = 0; i < filterCount; ++i)
  {
    ComparisonInput_t comp;
    comp.arrayName = featureNames[i];
    comp.compOperator = featureOperators[i];
    comp.compValue = featureValues[i];
    comps.push_back(comp);
  }
  return comps;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::setupGui()
{
    qRegisterMetaType<ComparisonInput_t>();
     qRegisterMetaType<QVector<ComparisonInput_t> >();

  // Catch when the filter is about to execute the preflight
  connect(m_Filter, SIGNAL(preflightAboutToExecute()),
    this, SLOT(beforePreflight()));

  // Catch when the filter is finished running the preflight
  connect(m_Filter, SIGNAL(preflightExecuted()),
    this, SLOT(afterPreflight()));

  // Catch when the filter wants its values updated
  connect(m_Filter, SIGNAL(updateFilterParameters(AbstractFilter*)),
    this, SLOT(filterNeedsInputParameters(AbstractFilter*)));

  if (m_FilterParameter == NULL)
  {
    return;
  }

  m_ComparisonSelectionTableModel = new ComparisonSelectionTableModel(m_ShowOperators);
  QAbstractItemModel* model = m_ComparisonSelectionTableView->model();
  if(NULL != model)
  {
    delete model;
  }
  m_ComparisonSelectionTableView->setModel(m_ComparisonSelectionTableModel);
  m_ComparisonSelectionTableModel->setNumberOfPhases(1);

  // Set the ItemDelegate for the table.
  QAbstractItemDelegate* aid = m_ComparisonSelectionTableModel->getItemDelegate();
  m_ComparisonSelectionTableView->setItemDelegate(aid);



  dataContainerList->blockSignals(true);
  attributeMatrixList->blockSignals(true);

  dataContainerList->clear();
  attributeMatrixList->clear();

  // Now let the gui send signals like normal
  dataContainerList->blockSignals(false);
  attributeMatrixList->blockSignals(false);


  populateComboBoxes();


}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateComboBoxes()
{
  //  std::cout << "void SingleArraySelectionWidget::populateComboBoxesWithSelection()" << std::endl;


  // Now get the DataContainerArray from the Filter instance
  // We are going to use this to get all the current DataContainers
  DataContainerArray::Pointer dca = m_Filter->getDataContainerArray();
  if(NULL == dca.get()) { return; }

  // Check to see if we have any DataContainers to actually populate drop downs with.
  if(dca->getDataContainerArray().size() == 0)
  {
    return;
  }
  // Cache the DataContainerArray Structure for our use during all the selections
  m_DcaProxy = DataContainerArrayProxy(dca.get());

  // Populate the DataContainerArray Combo Box with all the DataContainers
  QList<DataContainerProxy> dcList = m_DcaProxy.list;
  QListIterator<DataContainerProxy> iter(dcList);

  while(iter.hasNext() )
  {
    DataContainerProxy dc = iter.next();
    if(dataContainerList->findText(dc.name) == -1 ) {
      dataContainerList->addItem(dc.name);
    }
  }

  // Grab what is currently selected
  QString curDcName = dataContainerList->currentText();
  QString curAmName = attributeMatrixList->currentText();
//  QString curDaName = attributeArrayList->currentText();

  // Get what is in the filter
  QString selectedPath = m_Filter->property(PROPERTY_NAME_AS_CHAR).toString();
  // Split the path up to make sure we have a valid path separated by the "|" character

  QString filtDcName;
  QString filtAmName;
//  QString filtDaName;
  QStringList tokens = selectedPath.split(DREAM3D::PathSep);
  if(tokens.size() == 3) {
    filtDcName = tokens.at(0);
    filtAmName = tokens.at(1);
//    filtDaName = tokens.at(2);
  }

  // Now to figure out which one of these to use. If this is the first time through then what we picked up from the
  // gui will be empty strings because nothing is there. If there is something in the filter then we should use that.
  // If there is something in both of them and they are NOT equal then we have a problem. Use the flag m_DidCausePreflight
  // to determine if the change from the GUI should over ride the filter or vice versa. there is a potential that in future
  // versions that something else is driving DREAM3D and pushing the changes to the filter and we need to reflect those
  // changes in the GUI, like a testing script?

  QString dcName = checkStringValues(curDcName, filtDcName);
  QString amName = checkStringValues(curAmName, filtAmName);
 // QString daName = checkStringValues(curDaName, filtDaName);

  bool didBlock = false;

  if (!dataContainerList->signalsBlocked()) { didBlock = true; }
  dataContainerList->blockSignals(true);
  int dcIndex = dataContainerList->findText(dcName);
  if(dcIndex < 0 && dcName.isEmpty() == false) {
    dataContainerList->addItem(dcName);
  } // the string was not found so just set it to the first index
  else {
    if(dcIndex < 0) { dcIndex = 0; } // Just set it to the first DataContainer in the list
    dataContainerList->setCurrentIndex(dcIndex);
    populateAttributeMatrixList();
  }
  if(didBlock) { dataContainerList->blockSignals(false); didBlock = false; }


  if(!attributeMatrixList->signalsBlocked()) { didBlock = true; }
  attributeMatrixList->blockSignals(true);
  int amIndex = attributeMatrixList->findText(amName);
  if(amIndex < 0 && amName.isEmpty() == false) { attributeMatrixList->addItem(amName); } // The name of the attributeMatrix was not found so just set the first one
  else {
    if(amIndex < 0) { amIndex = 0; }
    attributeMatrixList->setCurrentIndex(amIndex);
    QStringList possibleArrays = generateAttributeArrayList();
    m_ComparisonSelectionTableModel->setPossibleFeatures(possibleArrays);
  }
  if(didBlock) { attributeMatrixList->blockSignals(false); didBlock = false; }


}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateAttributeMatrixList()
{
  QString dcName = dataContainerList->currentText();

  // Clear the AttributeMatrix List
  attributeMatrixList->blockSignals(true);
  attributeMatrixList->clear();

  // Loop over the data containers until we find the proper data container
  QList<DataContainerProxy> containers = m_DcaProxy.list;
  QListIterator<DataContainerProxy> containerIter(containers);
  while(containerIter.hasNext())
  {
    DataContainerProxy dc = containerIter.next();

    if(dc.name.compare(dcName) == 0 )
    {
      // We found the proper Data Container, now populate the AttributeMatrix List
      QMap<QString, AttributeMatrixProxy> attrMats = dc.attributeMatricies;
      QMapIterator<QString, AttributeMatrixProxy> attrMatsIter(attrMats);
      while(attrMatsIter.hasNext() )
      {
        attrMatsIter.next();
        QString amName = attrMatsIter.key();
        attributeMatrixList->addItem(amName);
      }
    }
  }

  attributeMatrixList->blockSignals(false);
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QStringList ComparisonSelectionWidget::generateAttributeArrayList()
{

  QStringList attributeArrayList;
  // Get the selected Data Container Name from the DataContainerList Widget
  QString currentDCName = dataContainerList->currentText();
  QString currentAttrMatName = attributeMatrixList->currentText();

  // Loop over the data containers until we find the proper data container
  QList<DataContainerProxy> containers = m_DcaProxy.list;
  QListIterator<DataContainerProxy> containerIter(containers);
  while(containerIter.hasNext())
  {
    DataContainerProxy dc = containerIter.next();
    if(dc.name.compare(currentDCName) == 0 )
    {
      // We found the proper Data Container, now populate the AttributeMatrix List
      QMap<QString, AttributeMatrixProxy> attrMats = dc.attributeMatricies;
      QMapIterator<QString, AttributeMatrixProxy> attrMatsIter(attrMats);
      while(attrMatsIter.hasNext() )
      {
        attrMatsIter.next();
        QString amName = attrMatsIter.key();
        if(amName.compare(currentAttrMatName) == 0 )
        {

          // We found the selected AttributeMatrix, so loop over this attribute matrix arrays and populate the list widget
          AttributeMatrixProxy amProxy = attrMatsIter.value();
          QMap<QString, DataArrayProxy> dataArrays = amProxy.dataArrays;
          QMapIterator<QString, DataArrayProxy> dataArraysIter(dataArrays);
          while(dataArraysIter.hasNext() )
          {
            dataArraysIter.next();
            //DataArrayProxy daProxy = dataArraysIter.value();
            QString daName = dataArraysIter.key();
            attributeArrayList << daName;
          }
        }
      }
    }
  }

  return attributeArrayList;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString ComparisonSelectionWidget::checkStringValues(QString curDcName, QString filtDcName)
{
  if(curDcName.isEmpty() == true && filtDcName.isEmpty() == false)
  {return filtDcName;}
  else if(curDcName.isEmpty() == false && filtDcName.isEmpty() == true)
  {return curDcName;}
  else if(curDcName.isEmpty() == false && filtDcName.isEmpty() == false && m_DidCausePreflight == true)
  { return curDcName;}

  return filtDcName;

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::on_addComparison_clicked()
{
  if (!m_ComparisonSelectionTableModel->insertRow(m_ComparisonSelectionTableModel->rowCount())) { return; }

  QModelIndex index = m_ComparisonSelectionTableModel->index(m_ComparisonSelectionTableModel->rowCount() - 1, 0);
  m_ComparisonSelectionTableView->setCurrentIndex(index);
  m_ComparisonSelectionTableView->resizeColumnsToContents();
  m_ComparisonSelectionTableView->scrollToBottom();
  m_ComparisonSelectionTableView->setFocus();
  emit parametersChanged();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::on_removeComparison_clicked()
{
  //qDebug() << "on_removeComparisonSelection_clicked" << "\n";
  QItemSelectionModel* selectionModel = m_ComparisonSelectionTableView->selectionModel();
  if (!selectionModel->hasSelection()) { return; }
  QModelIndex index = selectionModel->currentIndex();
  if (!index.isValid()) { return; }
  m_ComparisonSelectionTableModel->removeRow(index.row(), index.parent());
  if (m_ComparisonSelectionTableModel->rowCount() > 0)
  {
    m_ComparisonSelectionTableView->resizeColumnsToContents();
  }
  emit parametersChanged();
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::setComparisons(QVector<ComparisonInput_t> comparisons)
{
  qint32 count = comparisons.size();

  QVector<QString> arrayNames(count);
  QVector<int>   compOperators(count);
  QVector<float> compValues(count);
  //bool ok = false;
  for(int i = 0; i < count; ++i)
  {
    arrayNames[i] = (comparisons[i].arrayName);
    compOperators[i] = comparisons[i].compOperator;
    compValues[i] = comparisons[i].compValue;
  }
  m_ComparisonSelectionTableModel->setTableData(arrayNames, compValues, compOperators);
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::filterNeedsInputParameters(AbstractFilter* filter)
{
 // qDebug() << "DataContainerArrayProxyWidget::filterNeedsInputParameters(AbstractFilter* filter)";

  QVariant var;
  //var.setValue(m_DcaProxy);
  bool ok = false;
  // Set the value into the Filter
  ok = filter->setProperty(PROPERTY_NAME_AS_CHAR, var);
  if(false == ok)
  {
    QString ss = QObject::tr("Filter '%1': Error occurred setting Filter Parameter '%2'").arg(m_Filter->getNameOfClass()).arg(m_FilterParameter->getPropertyName() );
    emit errorSettingFilterParameter(ss);
    qDebug() << ss;
  }

}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::beforePreflight()
{
  if (NULL == m_Filter) { return; }
  if(m_DidCausePreflight == true)
  {
    std::cout << "***  ComparisonSelectionWidget already caused a preflight, just returning" << std::endl;
    return;
  }

  dataContainerList->blockSignals(true);
  attributeMatrixList->blockSignals(true);

  // Reset all the combo box widgets to have the default selection of the first index in the list
  populateComboBoxes();

  dataContainerList->blockSignals(false);
  attributeMatrixList->blockSignals(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::afterPreflight()
{
 // qDebug() << m_Filter->getNameOfClass() << " DataContainerArrayProxyWidget::afterPreflight()";
}


#if 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateArrayNames(DataContainerArray::Pointer dca)
{
#if __APPLE__
#warning This needs to be fixed
#endif
  if (m_ArrayListType >= CellListType && m_ArrayListType <= FaceListType )
  {
//    populateVolumeArrayNames(vldc);
  }
  else if (m_ArrayListType >= FeatureListType && m_ArrayListType <= FaceListType)
  {
    //populateSurfaceArrayNames(sdc);
  }
  else if (m_ArrayListType >= FeatureListType && m_ArrayListType <= EdgeListType)
  {
    //  populateEdgeArrayNames(edc);
  }
  else if (m_ArrayListType >= FeatureListType && m_ArrayListType <= VertexListType)
  {
    //  populateVertexArrayNames(vdc);
  }


  // We need to do this each time the possible arrays names are changed upstream in the
  // pipeline so that we get a new/updated array list.
  // Set the ItemDelegate for the table.
  QAbstractItemDelegate* aid = m_ComparisonSelectionTableModel->getItemDelegate();
  m_ComparisonSelectionTableView->setItemDelegate(aid);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateVolumeArrayNames(VolumeDataContainer::Pointer vldc)
{

  QList<QString> cellNames;
  if (m_ArrayListType == VertexListType)
  {
    cellNames = vldc->getVertexArrayNameList();
  }
  else if (m_ArrayListType == EdgeListType)
  {
    cellNames = vldc->getEdgeArrayNameList();
  }
  else if (m_ArrayListType == FaceListType)
  {
    cellNames = vldc->getFaceArrayNameList();
  }
  if (m_ArrayListType == CellListType)
  {
    cellNames = vldc->getCellArrayNameList();
  }
  else if (m_ArrayListType == FeatureListType)
  {
    cellNames = vldc->getCellFeatureArrayNameList();
  }
  else if (m_ArrayListType == EnsembleListType)
  {
    cellNames = vldc->getCellEnsembleArrayNameList();
  }
  m_ComparisonSelectionTableModel->setPossibleFeatures(cellNames);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateSurfaceArrayNames(SurfaceDataContainer::Pointer sdc)
{
  QList<QString> cellNames;
  if (m_ArrayListType == VertexListType)
  {
    cellNames = sdc->getVertexArrayNameList();
  }
  else if (m_ArrayListType == EdgeListType)
  {
    cellNames = sdc->getEdgeArrayNameList();
  }
  else if (m_ArrayListType == FaceListType)
  {
    cellNames = sdc->getFaceArrayNameList();
  }
  else if (m_ArrayListType == FeatureListType)
  {
    cellNames = sdc->getFaceFeatureArrayNameList();
  }
  else if (m_ArrayListType == EnsembleListType)
  {
    cellNames = sdc->getFaceEnsembleArrayNameList();
  }
  m_ComparisonSelectionTableModel->setPossibleFeatures(cellNames);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateEdgeArrayNames(EdgeDataContainer::Pointer edc)
{
  QList<QString> cellNames;
  if (m_ArrayListType == VertexListType)
  {
    cellNames = edc->getVertexArrayNameList();
  }
  else if (m_ArrayListType == EdgeListType)
  {
    cellNames = edc->getEdgeArrayNameList();
  }
  else if (m_ArrayListType == FeatureListType)
  {
    cellNames = edc->getEdgeFeatureArrayNameList();
  }
  else if (m_ArrayListType == EnsembleListType)
  {
    cellNames = edc->getEdgeEnsembleArrayNameList();
  }
  m_ComparisonSelectionTableModel->setPossibleFeatures(cellNames);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ComparisonSelectionWidget::populateVertexArrayNames(VertexDataContainer::Pointer vdc)
{
  QList<QString> cellNames;
  if (m_ArrayListType == VertexListType)
  {
    cellNames = vdc->getVertexArrayNameList();
  }
  else if (m_ArrayListType == FeatureListType)
  {
    cellNames = vdc->getVertexFeatureArrayNameList();
  }
  else if (m_ArrayListType == EnsembleListType)
  {
    cellNames = vdc->getVertexEnsembleArrayNameList();
  }
  m_ComparisonSelectionTableModel->setPossibleFeatures(cellNames);
}
#endif
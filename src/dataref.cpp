/*****************************************************************************
* Copyright 2015-2017 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "dataref.h"

#include "atools.h"

#include <QDebug>

DataRef::DataRef(DataRefPtrVector& refs, const QString& dataRefName)
  : name(dataRefName)
{
  // Add to array but do not find it yet (still invalid)
  refs.append(this);
}

DataRef::DataRef(const QString& dataRefName)
  : name(dataRefName)
{
}

DataRef::DataRef()
{
}

bool DataRef::find()
{
  qDebug() << Q_FUNC_INFO << name;
  dataRef = XPLMFindDataRef(name.toLatin1().constData());
  if(dataRef == NULL)
    qWarning() << Q_FUNC_INFO << "Cannot find" << name;
  else
  {
    dataRefType = XPLMGetDataRefTypes(dataRef);
    if(dataRefType == xplmType_Unknown)
      qWarning() << Q_FUNC_INFO << name << "has unexpected type xplmType_Unknown";
  }

  return isValid();
}

QVector<int> DataRef::valueIntArr() const
{
#ifdef DATAREF_VALIDATION
  checkType(xplmType_IntArray);
#endif

  // Get size first by calling with null pointer
  int size = XPLMGetDatavi(dataRef, nullptr, 0, 0);

  // Get the array contents
  IntVector retval(size);
  XPLMGetDatavi(dataRef, retval.data(), 0, size);
  return retval;
}

QVector<float> DataRef::valueFloatArr() const
{
#ifdef DATAREF_VALIDATION
  checkType(xplmType_FloatArray);
#endif

  // Get size first by calling with null pointer
  int size = XPLMGetDatavf(dataRef, nullptr, 0, 0);

  // Get the array contents
  FloatVector retval(size);
  XPLMGetDatavf(dataRef, retval.data(), 0, size);
  return retval;
}

QByteArray DataRef::valueByteArr() const
{
#ifdef DATAREF_VALIDATION
  checkType(xplmType_Data);
#endif

  // Get size first by calling with null pointer
  int size = XPLMGetDatab(dataRef, nullptr, 0, 0);

  // Get the array contents
  QByteArray retval(size, 0);
  XPLMGetDatab(dataRef, retval.data(), 0, size);
  return retval;
}

void DataRef::valueIntArr(IntVector& array) const
{
#ifdef DATAREF_VALIDATION
  checkType(xplmType_IntArray);
#endif

  int size = XPLMGetDatavi(dataRef, nullptr, 0, 0);
  array.resize(size);
  XPLMGetDatavi(dataRef, array.data(), 0, size);
}

void DataRef::valueFloatArr(FloatVector& array) const
{
#ifdef DATAREF_VALIDATION
  checkType(xplmType_FloatArray);
#endif

  int size = XPLMGetDatavf(dataRef, nullptr, 0, 0);
  array.resize(size);
  XPLMGetDatavf(dataRef, array.data(), 0, size);
}

void DataRef::valueByteArr(QByteArray& bytes) const
{
  int size = XPLMGetDatab(dataRef, nullptr, 0, 0);
  bytes.resize(size);
  XPLMGetDatab(dataRef, bytes.data(), 0, size);
}

#ifdef DATAREF_VALIDATION
void DataRef::checkType(int type) const
{
  if((dataRefType & type) == 0)
    qWarning() << Q_FUNC_INFO << "Type mismatch" << dataRefType << "does not cover" << type;
}

#endif

int DataRef::valueIntArrSum() const
{
  int sumValue = 0.f;
  for(int val : valueIntArr())
    sumValue += val;
  return sumValue;
}

float DataRef::valueFloatArrSum() const
{
  float sumValue = 0.f;
  for(float val : valueFloatArr())
    sumValue += val;
  return sumValue;
}

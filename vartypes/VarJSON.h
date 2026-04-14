//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    VarJSON.h
  \brief   C++ Interface: VarJSON
  \author  Modernization pass, 2026

  JSON serialization backend for the VarTypes system. Provides the same
  read/write semantics as VarXML but using QJsonDocument/QJsonObject.
*/
//========================================================================
#ifndef VARJSON_H_
#define VARJSON_H_

#include "VarTypes.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <string>
#include <vector>

namespace VarTypes {

  /*!
    \class  VarJSON
    \brief  JSON helper functions for the VarType system.

    Provides static functions to serialize/deserialize a VarTypes tree
    to/from JSON files, mirroring the VarXML interface.
  */
  class VarJSON
  {
  public:
    VarJSON();
    virtual ~VarJSON();

    /// Write a VarType node (and all of its children) to a JSON file.
    static void write(VarPtr rootVar, const std::string & filename);

    /// Write a vector of VarType nodes to a JSON file.
    static void write(const std::vector<VarPtr> & rootVars, const std::string & filename);

    /// Read a VarType-tree from a JSON file, merging with existing nodes.
    static std::vector<VarPtr> read(std::vector<VarPtr> existing_nodes, const std::string & filename);

    /// Get the JSON representation as a string.
    static std::string getJSON(const std::vector<VarPtr> & rootVars);

    /// Convert a VarType tree to a QJsonObject.
    static QJsonObject varToJson(VarPtr var);

    /// Read a VarType from a QJsonObject, updating existing or creating new.
    static VarPtr jsonToVar(const QJsonObject & obj, const std::vector<VarPtr> & existing, bool blind_append = false);

  private:
    /// Recursively serialize children.
    static QJsonArray childrenToJson(const std::vector<VarPtr> & children);

    /// Recursively deserialize children.
    static std::vector<VarPtr> jsonToChildren(const QJsonArray & arr, const std::vector<VarPtr> & existing, bool blind_append);
  };

} // namespace VarTypes

#endif /* VARJSON_H_ */

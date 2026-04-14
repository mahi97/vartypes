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
  \file    VarJSON.cpp
  \brief   C++ Implementation: VarJSON
  \author  Modernization pass, 2026

  JSON serialization backend for VarTypes. Mirrors VarXML behavior:
  - Each VarType node becomes a JSON object with "type", "name", "value", and optionally "children"
  - Attribute metadata (min, max) is stored as string values in an "attributes" object
  - The root document is { "VarJSON": [ ...nodes... ] }
*/
//========================================================================

#include "VarJSON.h"
#include "VarTypesInstance.h"
#include "VarList.h"
#include <QFile>
#include <QByteArray>
#include <cstdio>

namespace VarTypes {

VarJSON::VarJSON() {}
VarJSON::~VarJSON() {}

QJsonObject VarJSON::varToJson(VarPtr var) {
    QJsonObject obj;
    obj["type"] = QString::fromStdString(var->getTypeName());
    obj["name"] = QString::fromStdString(var->getName());

    // Store the serialized value
    std::string serialVal;
    var->getSerialString(serialVal);
    obj["value"] = QString::fromStdString(serialVal);

    // Store attributes (min/max) as numeric JSON values
    QJsonObject attrs;
    if (var->hasMinValue()) {
        attrs["minval"] = var->getMinValue();
    }
    if (var->hasMaxValue()) {
        attrs["maxval"] = var->getMaxValue();
    }
    if (!attrs.isEmpty()) {
        obj["attributes"] = attrs;
    }

    // Recurse into children
    std::vector<VarPtr> children = var->getChildren();
    if (!children.empty()) {
        obj["children"] = childrenToJson(children);
    }

    return obj;
}

QJsonArray VarJSON::childrenToJson(const std::vector<VarPtr> & children) {
    QJsonArray arr;
    for (const auto & child : children) {
        if (child && !child->areFlagsSet(VARTYPE_FLAG_NOSAVE)) {
            arr.append(varToJson(child));
        }
    }
    return arr;
}

void VarJSON::write(VarPtr rootVar, const std::string & filename) {
    std::vector<VarPtr> v;
    v.push_back(rootVar);
    write(v, filename);
}

void VarJSON::write(const std::vector<VarPtr> & rootVars, const std::string & filename) {
    QJsonObject root;
    QJsonArray nodesArray;
    for (const auto & var : rootVars) {
        if (var) {
            nodesArray.append(varToJson(var));
        }
    }
    root["VarJSON"] = nodesArray;

    QJsonDocument doc(root);
    QFile file(QString::fromStdString(filename));
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    } else {
        fprintf(stderr, "VarJSON::write: Unable to open file '%s' for writing.\n", filename.c_str());
    }
}

std::string VarJSON::getJSON(const std::vector<VarPtr> & rootVars) {
    QJsonObject root;
    QJsonArray nodesArray;
    for (const auto & var : rootVars) {
        if (var) {
            nodesArray.append(varToJson(var));
        }
    }
    root["VarJSON"] = nodesArray;

    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Indented).toStdString();
}

void VarJSON::applyAttributes(VarPtr /* target */, const QJsonObject & /* attrs */) {
    // Attributes (min/max) are stored in JSON for informational round-trip.
    // However, full attribute restoration requires type-specific downcasting
    // (VarInt::setMin, VarDouble::setMax, etc.) which the generic VarPtr
    // interface doesn't expose. The XML backend handles this via virtual
    // readAttributes() methods tied to the XML parser.
    //
    // In practice, applications define constraints at tree construction time,
    // and serialization only restores values. Min/max attributes are preserved
    // in JSON output for documentation and potential future use.
}

void VarJSON::applyChildren(VarPtr target, const QJsonArray & childArr, bool blind_append) {
    std::vector<VarPtr> existingChildren = target->getChildren();
    std::vector<VarPtr> merged = jsonToChildren(childArr, existingChildren, blind_append);

    // If the target is a VarList (or subclass), update its children.
    // We use dynamic_cast to check, since only list-like types have children.
    VarList * listNode = dynamic_cast<VarList *>(target.get());
    if (listNode) {
        // Add any new children that weren't in the original list
        for (const auto & child : merged) {
            bool isNew = true;
            for (const auto & existing : existingChildren) {
                if (existing == child) {
                    isNew = false;
                    break;
                }
            }
            if (isNew) {
                listNode->addChild(child);
            }
        }
    }
}

VarPtr VarJSON::jsonToVar(const QJsonObject & obj, const std::vector<VarPtr> & existing, bool blind_append) {
    std::string stype = obj["type"].toString().toStdString();
    std::string sname = obj["name"].toString().toStdString();
    std::string svalue = obj["value"].toString().toStdString();

    if (!VarTypesInstance::getFactory()->isRegisteredType(stype)) {
        fprintf(stderr, "VarJSON: Unknown type '%s' in JSON\n", stype.c_str());
        return VarPtr();
    }

    // Try to find existing node by name and type
    VarPtr target;
    if (!blind_append && !sname.empty()) {
        for (const auto & e : existing) {
            if (e && e->getName() == sname && e->getTypeName() == stype) {
                target = e;
                break;
            }
        }
    }

    if (!target) {
        target = VarTypesInstance::getFactory()->newVarType(stype);
        target->setName(sname);
    }

    // Set the value
    target->setSerialString(svalue);

    // Apply attributes
    if (obj.contains("attributes")) {
        applyAttributes(target, obj["attributes"].toObject());
    }

    // Process children
    if (obj.contains("children")) {
        applyChildren(target, obj["children"].toArray(), blind_append);
    }

    return target;
}

std::vector<VarPtr> VarJSON::jsonToChildren(const QJsonArray & arr, const std::vector<VarPtr> & existing, bool blind_append) {
    std::vector<VarPtr> result = existing;

    for (int i = 0; i < arr.size(); i++) {
        QJsonObject childObj = arr[i].toObject();
        std::string stype = childObj["type"].toString().toStdString();
        std::string sname = childObj["name"].toString().toStdString();

        if (!VarTypesInstance::getFactory()->isRegisteredType(stype)) {
            fprintf(stderr, "VarJSON: Unknown type '%s' in JSON\n", stype.c_str());
            continue;
        }

        bool found = false;
        if (!blind_append && !sname.empty()) {
            for (auto & e : result) {
                if (e && e->getName() == sname && e->getTypeName() == stype) {
                    // Update existing node's value
                    std::string svalue = childObj["value"].toString().toStdString();
                    e->setSerialString(svalue);

                    // Apply attributes
                    if (childObj.contains("attributes")) {
                        applyAttributes(e, childObj["attributes"].toObject());
                    }

                    // Recurse into children
                    if (childObj.contains("children")) {
                        applyChildren(e, childObj["children"].toArray(), blind_append);
                    }
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            VarPtr newNode = VarTypesInstance::getFactory()->newVarType(stype);
            if (!newNode) continue;
            newNode->setName(sname);
            std::string svalue = childObj["value"].toString().toStdString();
            newNode->setSerialString(svalue);

            // Apply attributes to new node
            if (childObj.contains("attributes")) {
                applyAttributes(newNode, childObj["attributes"].toObject());
            }

            // Recurse children for new nodes
            if (childObj.contains("children")) {
                applyChildren(newNode, childObj["children"].toArray(), true);
            }
            result.push_back(newNode);
        }
    }

    return result;
}

std::vector<VarPtr> VarJSON::read(std::vector<VarPtr> existing_nodes, const std::string & filename) {
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        fprintf(stderr, "VarJSON::read: Unable to open file '%s'\n", filename.c_str());
        return existing_nodes;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        fprintf(stderr, "VarJSON::read: JSON parse error in '%s': %s\n",
                filename.c_str(), parseError.errorString().toStdString().c_str());
        return existing_nodes;
    }

    QJsonObject root = doc.object();
    if (!root.contains("VarJSON")) {
        fprintf(stderr, "VarJSON::read: Missing 'VarJSON' root key in '%s'\n", filename.c_str());
        return existing_nodes;
    }

    QJsonArray nodesArray = root["VarJSON"].toArray();
    return jsonToChildren(nodesArray, existing_nodes, false);
}

} // namespace VarTypes

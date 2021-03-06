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
#ifndef VARVAL_H
#define VARVAL_H
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <limits.h>
#include <float.h>
#include <typeinfo>

#ifdef WIN32
  typedef unsigned char uint8_t;
#endif

using namespace std;

namespace VarTypes {

  /**
    @author Stefan Zickler
  */
  
  
  class VarVal {
  
      protected:
      inline virtual void lock() const {};
      
      inline virtual void unlock() const {};
      
      inline virtual void changed() {};
      public:
  
      VarVal();
  
      virtual ~VarVal();
  
      /// Creates a clone of the item.
      /// Note, that for containers that include VarType pointers (e.g. a list consisting of child-elements),
      /// this will perform a SHALLOW-COPY (e.g. the pointer *values* will be copied, but not the actual children
      /// references by them.), To perform a fully recursive deep-copy clone, run "deepClone()".
      virtual VarVal * clone() const;
  
      /// Creates a clone of this item (similar to clone()), except that this will produce a DEEP-COPY,
      /// which recursively clones the entire data-hierarchy.
      virtual VarVal * deepClone() const;
      
      /// Print out debugging information of this node to console.
      /// Usually this means the actually data of the node will be printed.
      virtual void printdebug() const;
    
      /// Get a human-readable string representation of this node's data.
      virtual string getString() const;
    
      /// Set this node's data to some string representation.
      virtual bool setString(const string & val);
    
      /// Get a full canonical binary representation of this node.
      /// This will be used to store the node to XML.
      virtual void getBinarySerialString(string & val) const;

      /// and its less efficient, deprecated non-virtual wrapper:
      /// DO NOT OVERLOAD THIS:
      string getBinarySerialString() const;
    
      /// Set the node to some full canonical binary representation.
      /// This will be used to load the node from XML.
      virtual void setBinarySerialString(const string & val);

      /// Get a full canonical binary representation of this node.
      /// This will be used to store the node to XML.
      virtual void getSerialString(string & val) const;

      /// Serialize contents from istream
      virtual bool serializeContentsFromStream(std::istream & stream, const std::string & extension=".bin");
      
      /// Serialize contents to ostream and return appropriate filename extension
      virtual void serializeContentsToStream(std::ostream & stream) const;
      
      /// Get desired filename extension for serialized contents
      virtual string getSerializedContentsFilenameExtension() const {
        return ".bin";
      }
      
      /// and its less efficient, deprecated non-virtual wrapper:
      /// DO NOT OVERLOAD THIS:
      string getSerialString() const;
    
      /// Set the node to some full canonical string representation.
      /// This will be used to load the node from XML.
      virtual void setSerialString(const string & val);
    
      /// This function indicates whether this nodes represents a numeric value.
      /// This is useful for e.g. determining whether this item is plottable.
      /// (e.g. whether this node has some kind of y-value)
      virtual bool hasValue() const;
    
      /// This function indicates whether this node's value range has a pre-specified
      /// maximum
      /// This is useful for plotting.
      /// \see getMaxValue()
      virtual bool hasMaxValue() const; //indicates whether the value range has
      // a pre-specified maximum
    
      /// This function indicates whether this node's value range has a pre-specified
      /// minimum.
      /// This is useful for plotting.
      /// \see getMinValue()
      virtual bool hasMinValue() const; //indicates whether the value range has
      // a pre-specified minimum
    
      /// This function returns the pre-specified minimum of this node's value range.
      /// To first check whether this node has such a minimum, please see \c hasMinValue() .
      ///
      /// \see hasMinValue()
      virtual double getMinValue() const; // returns the pre-specified minimum
    
      /// This function returns the pre-specified maximum of this node's value range.
      /// To first check whether this node has such a maximum, please see \c hasMaxValue() .
      ///
      /// \see hasMaxValue()
      virtual double getMaxValue() const; // returns the pre-specified maximum
    
      /// Returns a single, numeric value of this node.
      /// To first check whether this node has a numeric value, please see \c hasValue() .
      ///
      /// \see hasValue()
      virtual double getValue() const; //returns the numeric value
  
  };

};

#endif

/*
    Copyright 2002-2008 Tampere University of Technology.  All Rights
    Reserved.

    This file is part of TTA-Based Codesign Environment (TCE).

    TCE is free software; you can redistribute it and/or modify it under the
    terms of the GNU General Public License version 2 as published by the Free
    Software Foundation.

    TCE is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
    details.

    You should have received a copy of the GNU General Public License along
    with TCE; if not, write to the Free Software Foundation, Inc., 51 Franklin
    St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
*/
/**
 * @file DataDefinition.hh
 *
 * Declaration of DataDefinition class.
 *
 * @author Mikael Lepist� 2006 (mikael.lepisto-no.spam-tut.fi)
 * @author Pekka J��skel�inen 2006 (pekka.jaaskelainen-no.spam-tut.fi)
 * @note rating: red
 */

#ifndef TTA_DATA_DEFINITION_HH
#define TTA_DATA_DEFINITION_HH

#include <vector>
#include "BaseType.hh"
#include "Exception.hh"
#include "Address.hh"

namespace TTAMachine {
    class AddressSpace;
}

namespace TTAProgram {
    
/**
 * Describes characteristics of an initialized or uninitialized 
 * memory area.
 */
class DataDefinition {
public:
    DataDefinition(
        Address start, 
        int size, MinimumAddressableUnit *initData = NULL)
        throw (OutOfRange);

    DataDefinition(
        Address start, 
        const std::vector<MinimumAddressableUnit>& initData)
        throw (OutOfRange);

    virtual ~DataDefinition();

    virtual Address startAddress() const;
    virtual void setStartAddress(Address start);
    
    virtual bool isInitialized() const;
    virtual MinimumAddressableUnit MAU(int index) const;
    virtual int size() const;

    virtual bool isAddress() const;
    virtual bool isInstructionAddress() const;
    virtual Address destinationAddress() const;
    virtual void setDestinationAddress(Address dest);
    
    virtual DataDefinition* copy() const;

private:
    /// Start address of definition.
    Address start_;

    /// Size of uninitialized data definition.
    int size_;
    
    /// Init data of definition.
    std::vector<MinimumAddressableUnit>* data_;

};

}

#endif

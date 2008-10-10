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
 * @file OperationBuilder.cc
 *
 * Declaration of OperationBuilder class.
 *
 * @author Jussi Nyk�nen 2004 (nykanen-no.spam-cs.tut.fi)
 * @note rating: red
 */

#ifndef TTA_OPERATION_BUILDER_HH
#define TTA_OPERATION_BUILDER_HH

#include <string>
#include <vector>

/**
 * Class that handles building the operation behavior.
 */
class OperationBuilder {
public:
    static OperationBuilder& instance();

    std::string xmlFilePath(const std::string& xmlFile);
    std::string behaviorFile(
        const std::string& baseName,
        std::string& path);

    bool buildObject(const std::string& baseName,
                     const std::string& behaviorFile,
                     const std::string& path,
                     std::vector<std::string>& output);

    bool installDataFile(
        const std::string& path,
        const std::string& xmlFile,
        const std::string& destination);

    bool verifyXML(const std::string file);

private:
    OperationBuilder();
    virtual ~OperationBuilder();

    std::string makeIncludeString(const std::vector<std::string>& paths);

    /// Static unique instance.
    static OperationBuilder* instance_;
};

#endif

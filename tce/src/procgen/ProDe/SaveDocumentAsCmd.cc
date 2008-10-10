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
 * @file SaveDocumentAsCmd.cc
 *
 * Definition of SaveDocumentAsCmd class.
 *
 * @author Veli-Pekka Jääskeläinen 2004 (vjaaskel-no.spam-cs.tut.fi)
 */

#include <wx/docview.h>

#include "SaveDocumentAsCmd.hh"
#include "ProDeConstants.hh"
#include "ProDe.hh"

using std::string;

/**
 * The Constructor.
 */
SaveDocumentAsCmd::SaveDocumentAsCmd():
    EditorCommand(ProDeConstants::CMD_NAME_SAVEAS_DOC) {

}


/**
 * The Destructor.
 */
SaveDocumentAsCmd::~SaveDocumentAsCmd() {}


/**
 * Executes the command.
 *
 * @return Always false. Returning true here would cause problems with the
 *         wxCommandProcessor, and the command is never undoable so it's
 *         safe to return false even if the command was succesfully executed.
 */
bool
SaveDocumentAsCmd::Do() {
    wxCommandEvent dummy(wxID_SAVEAS, 0);
    wxGetApp().docManager()->OnFileSaveAs(dummy);
    return false;
}


/**
 * Returns id of this command.
 *
 * @return ID for this command to be used in menus and toolbars.
 */
int
SaveDocumentAsCmd::id() const {
    return ProDeConstants::COMMAND_SAVEAS_DOC;
}


/**
 * Creates and returns a new instance of this command.
 *
 * @return Newly created instance of this command.
 */
SaveDocumentAsCmd*
SaveDocumentAsCmd::create() const {
    return new SaveDocumentAsCmd();
}


/**
 * Returns short version of the command name.
 *
 * @return Short name of the command to be used in the toolbar.
 */
string
SaveDocumentAsCmd::shortName() const {
    return ProDeConstants::CMD_SNAME_SAVEAS_DOC;
}

/**
 * Returns path to the command's icon file.
 *
 * @return Full path to the command's icon file.
 */
string
SaveDocumentAsCmd::icon() const {
    return ProDeConstants::CMD_ICON_SAVEAS_DOC;
}


/**
 * Returns true when the command is executable, false when not.
 *
 * This command is executable when a document is open.
 *
 * @return True, if a document is open.
 */
bool
SaveDocumentAsCmd::isEnabled() {
    wxDocManager* manager = wxGetApp().docManager();
    if (manager->GetCurrentView() != NULL) {
	return true;
    }
    return false;
}

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
 * @file AddBusCmd.cc
 *
 * Definition of AddBusCmd class.
 *
 * @author Veli-Pekka Jääskeläinen 2004 (vjaaskel-no.spam-cs.tut.fi)
 * @note rating: red
 */

#include <wx/wx.h>
#include <wx/docview.h>

#include "AddBusCmd.hh"
#include "Bus.hh"
#include "Segment.hh"
#include "BusDialog.hh"
#include "Model.hh"
#include "ModelConstants.hh"
#include "MDFDocument.hh"
#include "ProDeConstants.hh"
#include "ErrorDialog.hh"
#include "ProDe.hh"
#include "Machine.hh"
#include "Guard.hh"

using std::string;
using namespace TTAMachine;

/**
 * The Constructor.
 */
AddBusCmd::AddBusCmd() :
    EditorCommand(ProDeConstants::CMD_NAME_ADD_BUS) {
}



/**
 * Executes the command.
 *
 * @return true, if the command was succesfully executed, false otherwise.
 */
bool
AddBusCmd::Do() {

    assert(parentWindow() != NULL);
    assert(view() != NULL);

    Model* model = dynamic_cast<MDFDocument*>(
        view()->GetDocument())->getModel();

    model->pushToStack();

    // Generate name for the new bus.
    Machine::BusNavigator navigator =
        model->getMachine()->busNavigator();
    int i = 1;
    string newName = ProDeConstants::COMP_NEW_NAME_PREFIX_BUS +
        Conversion::toString(i);
    while (navigator.hasItem(newName)) {
        newName = ProDeConstants::COMP_NEW_NAME_PREFIX_BUS +
            Conversion::toString(i);
        i++;
    }
    Bus* bus =
        new Bus(newName, ModelConstants::DEFAULT_WIDTH,
                ModelConstants::DEFAULT_WIDTH, Machine::ZERO);
    bus->setMachine(*(model->getMachine()));

    // Create an always-true guard for the bus by default.
    new UnconditionalGuard(false, *bus);

    // Add a default segment to the bus.
    new Segment(ProDeConstants::COMP_DEFAULT_NAME_SEGMENT, *bus);

    BusDialog dialog(parentWindow(), bus);

    if (dialog.ShowModal() == wxID_OK) {
	model->notifyObservers();
	return true;
    } else {
	// bus creation was cancelled
	model->popFromStack();
	return false;
    }
}


/**
 * Returns id of this command.
 *
 * @return ID for this command to be used in menus and toolbars.
 */
int
AddBusCmd::id() const {
    return ProDeConstants::COMMAND_ADD_BUS;
}


/**
 * Creates and returns a new instance of this command.
 *
 * @return Newly created instance of this command.
 */
AddBusCmd*
AddBusCmd::create() const {
    return new AddBusCmd();
}


/**
 * Returns short version of the command name.
 *
 * @return Short name of the command to be used in the toolbar.
 */
string
AddBusCmd::shortName() const {
    return ProDeConstants::CMD_SNAME_ADD_BUS;
}


/**
 * Returns true when the command is executable, false when not.
 *
 * This command is executable when a document is open.
 *
 * @return True, if a document is open.
 */
bool
AddBusCmd::isEnabled() {
    wxDocManager* manager = wxGetApp().docManager();
    if (manager->GetCurrentView() != NULL) {
	return true;
    }
    return false;
}

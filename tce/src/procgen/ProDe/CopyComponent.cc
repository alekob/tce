/*
    Copyright (c) 2002-2010 Tampere University.

    This file is part of TTA-Based Codesign Environment (TCE).

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
 */
/**
 * @file CopyComponent.cc
 *
 * Definition of CopyComponent class.
 *
 * @author Veli-Pekka Jääskeläinen 2004 
 * @author Pekka Jääskeläinen 2010 (pjaaskel-no.spam-cs.tut.fi)
 */

#include "Application.hh"
#include "CopyComponent.hh"
#include "FunctionUnit.hh"
#include "EditPart.hh"
#include "ProDeClipboard.hh"

using namespace TTAMachine;

/**
 * The Constructor.
 *
 * @param editPart Function unit to delete.
 */
CopyComponent::CopyComponent(EditPart* editPart): editPart_(editPart) {
}


/**
 * The Destructor.
 */
CopyComponent::~CopyComponent() {
}


/**
 * Executes the command.
 *
 * @return true, if the command was succesfully executed, false otherwise.
 */
bool
CopyComponent::Do() {
    Component* component = dynamic_cast<Component*>(editPart_->model());
    assert (component != NULL);
    ProDeClipboard* clipboard = ProDeClipboard::instance();
    clipboard->setContents(component->saveState(), component->machine());
    return true;
}


/**
 * Undoing this command is not possible, returns always false.
 *
 * @return Always false.
 */
bool
CopyComponent::Undo() {
    return false;
}


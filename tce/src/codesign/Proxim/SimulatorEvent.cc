/*
    Copyright (c) 2002-2009 Tampere University.

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
 * @file SimulatorEvent.cc
 *
 * Implementation of SimulatorEvent class.
 *
 * @author Veli-Pekka Jääskeläinen 2005 (vjaaskel-no.spam-cs.tut.fi)
 * @note rating: red
 */

#include "SimulatorEvent.hh"

const wxEventType SimulatorEvent::EVT_SIMULATOR_TERMINATED = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_START = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_STOP = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_RUN = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_OUTPUT = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_ERROR = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_COMMAND = wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_COMMAND_DONE =
    wxNewEventType();

const wxEventType SimulatorEvent::EVT_SIMULATOR_RUNTIME_ERROR =
    wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_RUNTIME_WARNING =
    wxNewEventType();

const wxEventType SimulatorEvent::EVT_SIMULATOR_LOADING_MACHINE =
    wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_MACHINE_LOADED =
    wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_LOADING_PROGRAM =
    wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_PROGRAM_LOADED =
    wxNewEventType();
const wxEventType SimulatorEvent::EVT_SIMULATOR_RESET = wxNewEventType();

/**
 * The Constructor.
 *
 * @param eventType Event type ID.
 * @param data String data associated with the event.
 */
SimulatorEvent::SimulatorEvent(WXTYPE eventType, std::string data) :
    wxEvent(0, eventType), data_(data) {

}


/**
 * The Destructor.
 */
SimulatorEvent::~SimulatorEvent() {
}


/**
 * Returns string data of the event.
 *
 * @return String data associated with the event.
 */
std::string
SimulatorEvent::data() const {
    return data_;
}

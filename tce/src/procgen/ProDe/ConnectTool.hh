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
 * @file ConnectTool.hh
 *
 * Declaration of ConnectTool class.
 *
 * @author Veli-Pekka Jääskeläinen (vjaaskel-no.spam-cs.tut.fi)
 * @note rating: red
 */

#ifndef TTA_CONNECT_TOOL_HH
#define TTA_CONNECT_TOOL_HH

#include "MachineCanvasTool.hh"

class MDFView;
class EditPart;
class Figure;
class ChildFrame;

/**
 * Mouse tool for creating and removing connections between ports, sockets
 * and bus segments.
 */
class ConnectTool : public MachineCanvasTool {
public:
    ConnectTool(ChildFrame* frame, MDFView* view);
    virtual ~ConnectTool();
    virtual void activate();
    virtual void deactivate();
    virtual void onMouseEvent(wxMouseEvent& event, wxDC& dc);
    virtual Figure* figure();
private:
    void updateStatusline(EditPart* part);
    void leftClick(EditPart* part);
    void rightClick(wxMouseEvent& event);

    /// Parent frame of the canvas.
    ChildFrame* frame_;
    /// View displayed on the Canvas.
    MDFView* view_;
    /// Tells if the tool is active or not.
    bool active_;
    /// Target EditPart of the connection.
    EditPart* target_;
    /// Connection figure.
    Figure* figure_;
};

#endif

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
 * @file ControlFlowGraphPass.hh
 *
 * Declaration of ControlFlowGraphPass interface.
 *
 * @author Pekka Jääskeläinen 2007 (pjaaskel-no.spam-cs.tut.fi)
 * @note rating: red
 */

#ifndef TTA_CFG_PASS_HH
#define TTA_CFG_PASS_HH

#include "Exception.hh"
#include "SchedulerPass.hh"

class BasicBlockPass;
class ControlFlowGraph;
class BasicBlockNode;

namespace TTAMachine {
    class Machine;
}

/**
 * Interface for scheduler passes that handle control flow graphs.
 */
class ControlFlowGraphPass : public SchedulerPass {
public:
    ControlFlowGraphPass(InterPassData& data);
    virtual ~ControlFlowGraphPass();

    virtual void handleControlFlowGraph(
        ControlFlowGraph& cfg, const TTAMachine::Machine& targetMachine);

    void executeBasicBlockPass(
        ControlFlowGraph& cfg, const TTAMachine::Machine& targetMachine,
        BasicBlockPass& bbPass);
};
#endif

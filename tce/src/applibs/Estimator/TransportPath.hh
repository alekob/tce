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
 * @file TransportPath.hh
 *
 * Declaration of TransportPath class
 *
 * @author Pekka Jääskeläinen 2005 (pjaaskel-no.spam-cs.tut.fi)
 * @note rating: red
 */

#ifndef TTA_TRANSPORT_PATH_HH
#define TTA_TRANSPORT_PATH_HH

#include <list>

namespace TTAMachine {
    class Port;
    class Socket;
    class Bus;
}

namespace CostEstimator {
/**
 * Represents a single transport path in a machine.
 *
 * The class is simply a storage for source port, source socket, bus, 
 * destination socket, destination port components that make a path for
 * data transports in a machine. 
 */
class TransportPath {
public:
    TransportPath(
        const TTAMachine::Port& sourcePort, 
        const TTAMachine::Socket& sourceSocket,
        const TTAMachine::Bus& bus,
        const TTAMachine::Socket& destinationSocket,
        const TTAMachine::Port& destinationPort);
    virtual ~TransportPath();

    const TTAMachine::Port& sourcePort() const;
    const TTAMachine::Socket& sourceSocket() const;
    const TTAMachine::Bus& bus() const;
    const TTAMachine::Socket& destinationSocket() const;
    const TTAMachine::Port& destinationPort() const;

private:
    const TTAMachine::Port& sourcePort_;
    const TTAMachine::Socket& sourceSocket_;
    const TTAMachine::Bus& bus_;
    const TTAMachine::Socket& destinationSocket_;
    const TTAMachine::Port& destinationPort_;    
};

typedef std::list<TransportPath> TransportPathList;
}

#endif

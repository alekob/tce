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
 * Implementation of SimControlLanguageCommand class
 *
 * @author Pekka Jääskeläinen 2005 (pjaaskel-no.spam-cs.tut.fi)
 * @note rating: red
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include "SimControlLanguageCommand.hh"
#include "SimulatorInterpreterContext.hh"
#include "SimulatorFrontend.hh"
#include "SimulatorToolbox.hh"
#include "StringTools.hh"
#include "Program.hh"
#include "Procedure.hh"
#include "Breakpoint.hh"
#include "StopPointManager.hh"
#include "Conversion.hh"
#include "TclConditionScript.hh"
#include "ExpressionScript.hh"
#include "TclInterpreter.hh"
#include "GlobalScope.hh"
#include "CodeLabel.hh"
#include "DataLabel.hh"
#include "AddressSpace.hh"
#include "NullAddressSpace.hh"

using namespace TTAProgram;

///////////////////////////////////////////////////////////////////////////////
// SimControlLanguageCommand
///////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 *
 * Sets the name of the command to the base class.
 *
 * @param name Name of the command to add.
 */
SimControlLanguageCommand::SimControlLanguageCommand(
    const std::string& name) : 
    CustomCommand(name) {
}

/**
 * Destructor.
 *
 * Does nothing.
 */
SimControlLanguageCommand::~SimControlLanguageCommand() {
}

/**
 * Checks that the simulation is initialized and ready to run.
 *
 * Sets errors message and returns false if simulation is not initialized.
 *
 * @return True if simulation is initialized.
 */
bool
SimControlLanguageCommand::checkSimulationInitialized() {

    DataObject* result = new DataObject();        
    if (!simulatorFrontend().isSimulationInitialized()) {
        result->setString(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_INTERP_SIMULATION_NOT_INITIALIZED).str());
        interpreter()->setResult(result);
        return false;
    }

    interpreter()->setResult(result);
    return true;
}

/**
 * Checks that the simulation is not already running.
 *
 * Sets errors message and returns false if simulation is running. 
 *
 * @return True if simulation is not running.
 */
bool
SimControlLanguageCommand::checkSimulationNotAlreadyRunning() {

    DataObject* result = new DataObject();
    if (simulatorFrontend().isSimulationRunning() || 
        simulatorFrontend().isSimulationStopped()) {
        result->setString(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_INTERP_SIMULATION_ALREDY_RUNNING).str());
        interpreter()->setResult(result);
        return false;
    }    
    interpreter()->setResult(result);        
    return true;
}

/**
 * Checks that the simulation is stopped.
 *
 * Sets errors message and returns false if simulation is not stopped by 
 * the user.
 *
 * @return True if simulation is stopped.
 */
bool
SimControlLanguageCommand::checkSimulationStopped() {

    DataObject* result = new DataObject();
    if (!simulatorFrontend().isSimulationStopped()) {
        result->setString(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_INTERP_SIMULATION_NOT_RUNNING).str());
        interpreter()->setResult(result);
        return false;
    }    

    interpreter()->setResult(result);
    return true;
}

/**
 * Checks that the simulation has ended.
 *
 * Sets errors message and returns false if simulation has not ended.
 *
 * @return True if simulation has ended.
 */
bool
SimControlLanguageCommand::checkSimulationEnded() {

    DataObject* result = new DataObject();
    if (!simulatorFrontend().hasSimulationEnded()) {
        result->setString(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_INTERP_SIMULATION_NOT_ENDED).str());
        interpreter()->setResult(result);
        return false;
    }    
    
    interpreter()->setResult(result);
    return true;
}

/**
 * Checks that the simulation is running a parallel simulation.
 *
 * Sets errors message and returns false if no parallel simulation is
 * running.
 *
 * @return True if parallel simulation is running.
 */
bool
SimControlLanguageCommand::checkParallelSimulation() {

    if (!checkSimulationInitialized() &&
        !checkSimulationStopped() &&
        !simulatorFrontend().isSimulationRunning()) {
        return false;
    }

    if (simulatorFrontend().isSequentialSimulation()) {
        interpreter()->setError(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_NOT_RUNNING_PARALLEL_SIMULATION).str());
        return false;
    }        
    return true;
}

/**
 * Checks that the simulated program has been loaded successfully.
 *
 * @return True if program has been loaded.
 */
bool
SimControlLanguageCommand::checkProgramLoaded() {

    if (!simulatorFrontend().isProgramLoaded()) {
        interpreter()->setError(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_NO_PROGRAM_LOADED).str());

        return false;
    }
    return true;
}


/**
 * Returns the SimulatorFrontend instance stored in the context.
 *
 * @return The SimulatorFrontend instance.
 */
SimulatorFrontend& 
SimControlLanguageCommand::simulatorFrontend() {

    assert(interpreter() != NULL);
    
    SimulatorInterpreterContext& interpreterContext = 
        dynamic_cast<SimulatorInterpreterContext&>(interpreter()->context());
    
    return interpreterContext.simulatorFrontend();    
}

/**
 * Prints the next simulated instruction to the simulator console.
 */
void
SimControlLanguageCommand::printNextInstruction() {

    if (simulatorFrontend().nextInstructionPrinting())    
        outputStream() << simulatorFrontend().programLocationDescription() 
                       << std::endl;
}

/**
 * Prints information that should be printed after simulation is stopped.
 *
 * These should be printed when the control is returned back to user
 * after running simulation with "run" or "resume" commands and being
 * stopped due to simulation finish or a breakpoint.
 */
void
SimControlLanguageCommand::printStopInformation() {

    printNextInstruction();
    printStopReasons();
    printSimulationTime();
}

/**
 * Prints the reasons why simulation has been stopped.
 *
 * @todo the stop reason code to string conversion should be externalized
 * to another function.
 */
void
SimControlLanguageCommand::printStopReasons() {
    for (unsigned int i = 0; i < simulatorFrontend().stopReasonCount(); ++i) {
        switch (simulatorFrontend().stopReason(i)) {
        case SRE_AFTER_STEPPING:
            outputStream() 
                << SimulatorToolbox::textGenerator().text(
                    Texts::TXT_STOPREASON_STEPPING).str() << std::endl;
            break;
        case SRE_AFTER_UNTIL:
            outputStream() 
                << SimulatorToolbox::textGenerator().text(
                    Texts::TXT_STOPREASON_UNTIL).str() << std::endl;
            break;
        case SRE_AFTER_TIMEOUT:
            outputStream()
                << SimulatorToolbox::textGenerator().text(
                    Texts::TXT_STOPREASON_TIMEOUT).str() << std::endl;
            break;
        case SRE_USER_REQUESTED:
            outputStream() 
                << SimulatorToolbox::textGenerator().text(
                    Texts::TXT_STOPREASON_USERREQUEST).str() << std::endl;
            break;
        case SRE_RUNTIME_ERROR:
            outputStream() 
                << SimulatorToolbox::textGenerator().text(
                    Texts::TXT_STOPREASON_RUNTIME_ERROR).str() << std::endl;
            break;
        case SRE_BREAKPOINT: {
             StopPointManager& stopPointManager = 
                simulatorFrontend().stopPointManager();
            for (unsigned int i = 0; 
                 i < stopPointManager.stopCausingStopPointCount(); ++i) {
                try {
                    // test that the breakpoint is not deleted
                    stopPointManager.stopPointWithHandleConst(
                        stopPointManager.stopCausingStopPoint(i));
                    outputStream() 
                        << (SimulatorToolbox::textGenerator().text(
                                Texts::TXT_STOPREASON_BREAKPOINT) % 
                            stopPointManager.stopCausingStopPoint(i)).str()
                        << std::endl;
                } catch (const InstanceNotFound&) {
                    outputStream() 
                        << SimulatorToolbox::textGenerator().text(
                            Texts::TXT_STOPREASON_DELETED_BREAKPOINT).str()
                        << std::endl;
                }
            }
            break;
        }
        default:
            outputStream() 
                << "Stopped because of an unknown reason." << std::endl;
            break;
        }
    }
}

/**
 * Prints the simulation time in minutes and seconds in case it's enabled.
 *
 * Also prints out the frequency in MHz
 */
void
SimControlLanguageCommand::printSimulationTime() {        

    if (!simulatorFrontend().simulationTimeStatistics())
        return;

    ClockCycleCount cycleCount = simulatorFrontend().lastRunCycleCount();    
    double time = simulatorFrontend().lastRunTime();

    double minutes = 0.0;
    double seconds = modf(time / 60.0 , &minutes) * 60.0;
    double frequency = (static_cast<double>(cycleCount) / time) / 1000000;

        
    outputStream() 
        << minutes << " min "
        << seconds << " s "
        << frequency << " MHz" << std::endl;
}

/**
 * Returns the output stream which can be used to print information to
 * the user.
 *
 * Uses the same output stream as the linereader to make the information
 * texts printed to the same stream as confirmations of the linereader.
 *
 * @return The output stream.
 */
std::ostream&
SimControlLanguageCommand::outputStream() {
    return interpreter()->lineReader()->outputStream();
}

/**
 * Parses an instruction address expression string.
 *
 * Expression string can be given by the user to, for example, 'break' or
 * 'until' commands. This function also does range checking, when the address
 * is lower than the first address of the program, it sets the address to the
 * lowest address, when the address is bigger than the last address of the
 * program, it sets the address to the last address.
 *
 * Supported address expression strings are currently:
 *
 * [empty]              Address of the instruction following the current
 *                      instruction.
 * *address             An absolute instruction address reference, can be
 *                      an integer or a label string.
 *
 *
 * @param expression The expression to parse.
 * @return The parsed instruction address.
 * @exception IllegalParameters when expression could not be parsed to 
 *                              an address.
 */
InstructionAddress
SimControlLanguageCommand::parseInstructionAddressExpression(
    const std::string& expression)         
    throw (IllegalParameters) {

    std::string expr = StringTools::trim(expression);
    const std::string errorMessage = "Illegal argument.";

    InstructionAddress theAddress = 0;
    if (expr.size() == 0) {
        theAddress = simulatorFrontend().programCounter() + 1;
    } else {
        // the argument is an absolute address, try to evaluate it to an
        // instruction address 

        // check if it's an integer, that is, a direct instruction address
        // reference
        const std::string address = expression;
        try {
            theAddress = Conversion::toUnsignedInt(address);
        } catch (const NumberFormatException&) {
            // check if a label with given string is found in the
            // global scope
            const GlobalScope& globalScope = 
                simulatorFrontend().program().globalScopeConst();

            try {
                theAddress = 
                    globalScope.codeLabel(address).address().location();
            } catch (const KeyNotFound&) {
                try {
                    // check if it's a reference to a function
                    theAddress = 
                        simulatorFrontend().program().procedure(address).
                        startAddress().location();
                } catch (const KeyNotFound&) {
                    try {
                        // check if user forgot the leading "_"
                        theAddress = 
                            simulatorFrontend().program().procedure(
                                std::string("_") + address).
                            startAddress().location();
                    } catch (const KeyNotFound&) {
                        // couldn't evaluate the label 
                        interpreter()->setError(
                            SimulatorToolbox::textGenerator().text(
                            Texts::TXT_LABEL_NOT_FOUND).str());
                        
                        throw IllegalParameters(
                            __FILE__, __LINE__, __func__, errorMessage);
                    }
                }
            }
        }
    }

    // check the boundaries of the instruction address and fix it, if necessary
    const InstructionAddress programLastAddress = 
        simulatorFrontend().program().lastProcedure().endAddress().location() 
        - 1;

    const InstructionAddress programFirstAddress = 
        simulatorFrontend().program().firstProcedure().startAddress().
        location();
    
    if (theAddress > programLastAddress) {
        theAddress = programLastAddress;
    } else if (theAddress < programFirstAddress) {
        theAddress = programFirstAddress;
    }

    return theAddress;
}

/**
 * Parses a data address expression string.
 *
 * Expression string can be given by the user to, for example, 'x' command.
 * Resolves symbol.
 *
 * Supported address expression strings are currently:
 *
 * *address             An absolute address reference, can be
 *                      an integer or a label string. In case of a label
 *                      string, the return value contains also the address
 *                      space of the address expression.
 *
 *
 * @param expression The expression to parse.
 * @return The parsed instruction address and its address space, if given.
 *         In case address space could not be found, the address space
 *         field points to NullAddressSpace.
 * @exception IllegalParameters when expression could not be parsed to 
 *                              an address.
 */
TTAProgram::Address
SimControlLanguageCommand::parseDataAddressExpression(
    const std::string& expression)         
    throw (IllegalParameters) {

    std::string expr = StringTools::trim(expression);
    const std::string errorMessage = "Illegal argument.";

    InstructionAddress theAddress = 0;

    // check if it's an integer, that is, a direct address reference
    const std::string addressString = expression;
    try {
        theAddress = Conversion::toUnsignedInt(expression);
    } catch (const NumberFormatException&) {
        // it was not an integer, check if a label with given string 
        // is found in the global scope
        const GlobalScope& globalScope = 
            simulatorFrontend().program().globalScopeConst();
        
        try {
            return globalScope.dataLabel(expression).address();
        } catch (const KeyNotFound&) {
            // couldn't evaluate the label 
            interpreter()->setError(
                SimulatorToolbox::textGenerator().text(
                        Texts::TXT_LABEL_NOT_FOUND).str());
            throw IllegalParameters(
                __FILE__, __LINE__, __func__, errorMessage);
        }
    }
    return Address(theAddress, TTAMachine::NullAddressSpace::instance());
}

/**
 * Prompts for a condition script from the user.
 *
 * Also test runs the condition script. Returns false and sets interpreter 
 * error if the entered condition is illegal. If an empty condition script
 * is entered, the condition script is set to always true "1" condition.
 *
 * @param target The target to put the condition script in.
 * @return false in case the script produces error when executed.
 */
bool
SimControlLanguageCommand::askConditionFromUser(TclConditionScript& target) {

    std::string conditionScript =  
        StringTools::trim(
            interpreter()->lineReader()->readLine(
                SimulatorToolbox::textGenerator().text(
                    Texts::TXT_INTERP_ENTER_CONDITION_PROMPT).str()));

    if (StringTools::trim(conditionScript) == "") {
        conditionScript = "1";
    }

    TclConditionScript testCondition(
        dynamic_cast<TclInterpreter*>(interpreter()), conditionScript);

    // try the condition
    try {
        testCondition.execute();
        interpreter()->setResult("");
        target = testCondition;
    } catch (const ScriptExecutionFailure& sef) {
        interpreter()->setError(true);            
        return false;
    }   
    return true;
}

/**
 * Prompts for an expression script from the user.
 *
 * Also test runs the expression script. Returns false and sets interpreter 
 * error if the entered expression is illegal. If an empty expression script
 * is entered, returns false and sets interpreter error.
 *
 * @param target The target to put the expression script in.
 * @return false in case the script produces error when executed or it's empty.
 */
bool
SimControlLanguageCommand::askExpressionFromUser(ExpressionScript& target) {

    std::string expressionScript =  
        StringTools::trim(
            interpreter()->lineReader()->readLine(
                SimulatorToolbox::textGenerator().text(
                    Texts::TXT_INTERP_ENTER_EXPRESSION_PROMPT).str()));

    if (StringTools::trim(expressionScript) == "") {
        interpreter()->setError(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_EXPRESSION_EMPTY).str());
        return false;
    }

    ExpressionScript testExpression(
        dynamic_cast<TclInterpreter*>(interpreter()), expressionScript);

    // try the condition
    try {
        testExpression.execute();
        interpreter()->setResult("");
        target = testExpression;
        return true;
    } catch (const ScriptExecutionFailure& sef) {
        interpreter()->setError(true);            
        return false;
    }   
}

/**
 * Parses breakpoint data from the given arguments.
 *
 * This is a refactoring of common functionality used in 'bp' and 'tbp' 
 * commands. Sets the error message to the interpreter and returns false
 * if there was an error while parsing or simulation is in wrong state.
 *
 * @param arguments as passed to the execute() of the command.
 * @param target the breakpoint instance to put the parsed data to.
 * @return true if the breakpoint was parsed succesfully.
 */
bool 
SimControlLanguageCommand::parseBreakpoint(
    const std::vector<DataObject>& arguments, Breakpoint& target) {

    if (!checkSimulationInitialized() && !checkSimulationStopped()) {
        return false;
    }

    // how many arguments there are in the condition part
    size_t conditionArgumentCount = 0;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (StringTools::stringToLower(arguments[i].stringValue()) ==
            "if") {

            if (i != arguments.size() - 1) {
                interpreter()->setError(
                    SimulatorToolbox::textGenerator().text(
                        Texts::TXT_ILLEGAL_ARGUMENTS).str());
                return false;
            }
            conditionArgumentCount = 1;
            break;
        }
    }

    if (conditionArgumentCount > 0) {
        TclConditionScript condition(NULL, "");
        if (!askConditionFromUser(condition)) {
            return false;
        }
        assert(condition.script().size() > 0);
        if (condition.script().at(0) == "1") {
            target.removeCondition();
        } else {
            target.setCondition(condition);
        }
    }

    const int argumentsBeforeCondition = 
        arguments.size() - 1 - conditionArgumentCount;

    InstructionAddress theAddress = 0;

    if (argumentsBeforeCondition == 0 || argumentsBeforeCondition == 1) {

        const std::string argument = 
            ((argumentsBeforeCondition == 0)?
             (""):(arguments[1].stringValue()));
        try {
            theAddress = 
                parseInstructionAddressExpression(argument);
        } catch (const IllegalParameters&) {
            // the helper function has set the error string to 
            // the interpreter
            return false;
        }
        
    } else {
        interpreter()->setError(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_ILLEGAL_ARGUMENTS).str());
        return false;
    }

    /// @todo set the condition, if any
    
    target.setAddress(theAddress);
    return true;
}


/// widths of the columns of the break point info table
/// widths don't include the spaces between columns
/// the handle id column width
const int BPINFO_HANDLE_COL_WIDTH = 6;
/// the breakpoint address column width
const int BPINFO_ADDRESS_COL_WIDTH = 7;
/// the disposition (del/dis/keep) column width
const int BPINFO_DISPOSITION_COL_WIDTH = 11;
/// enabled (y/n) column width
const int BPINFO_ENABLED_COL_WIDTH = 7;
/// ignore count column width 
const int BPINFO_IGNORE_COUNT_COL_WIDTH = 6;
/// the breakpoint condition column width
const int BPINFO_CONDITION_COL_WIDTH = 40;

/**
 * Prints information of a breakpoint.
 *
 * If breakpoint with the given handle could not be found, sets error 
 * to the interpreter and returns false.
 *
 * @param breakpointHandle The handle of the breakpoint to be printed.
 * @return true If breakpoint could be printed.
 */
bool
SimControlLanguageCommand::printBreakpointInfo(
    unsigned int breakpointHandle) {

    try {
        const StopPoint& stopPoint = 
            simulatorFrontend().stopPointManager().stopPointWithHandleConst(
                breakpointHandle);

        outputStream() 
            << std::right << std::setw(BPINFO_HANDLE_COL_WIDTH) 
            << breakpointHandle << ": " << stopPoint.description() 
            << std::endl;

        return true;
    } catch (const InstanceNotFound&) {
        interpreter()->setError(
            SimulatorToolbox::textGenerator().text(
                Texts::TXT_BREAKPOINT_NOT_FOUND).str());
        return false;
    }
}

/**
 * Verifies that the list of breakpoint ids are valid breakpoint handles.
 *
 * A refactoring for error checking of the {enable,disable,delete}bp 
 * commands. Sets interpreter error in case invalid handles are found
 * and returns false.
 *
 * @param arguments The arguments as given to the execute() of the command.
 * @param startIndex The first argument to check.
 * @return true if all handles are valid.
 */
bool 
SimControlLanguageCommand::verifyBreakpointHandles(
    const std::vector<DataObject>& arguments,
    std::size_t startIndex) {
    
    // verify that the breakpoints are found
    for (size_t i = startIndex; i < arguments.size(); ++i) {
        if (!checkPositiveIntegerArgument(arguments[i])) {
            return false;
        }
        const unsigned int breakpointHandle = arguments[i].integerValue();
        try {
            StopPointManager& stopPointManager =
                simulatorFrontend().stopPointManager();
            // just check that breakpoint is found with the handle
            stopPointManager.stopPointWithHandleConst(breakpointHandle);
        } catch (const InstanceNotFound&) {
            interpreter()->setError(
                SimulatorToolbox::textGenerator().text(
                    Texts::TXT_BREAKPOINT_NOT_FOUND).str());
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// SimControlLanguageSubCommand
///////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 *
 * @param parentCommand The main command this is a subcommand for.
 * @param minArgs The minimum count of arguments this command should receive.
 * @param maxArgs The maximum count of arguments this command should receive.
 */
SimControlLanguageSubCommand::SimControlLanguageSubCommand(
    SimControlLanguageCommand& parentCommand) : 
    parentCommand_(parentCommand) {
}

/**
 * Destructor.
 *
 */
SimControlLanguageSubCommand::~SimControlLanguageSubCommand() {
}

/**
 * Returns the parent command.
 *
 * @return the parent command.
 */
SimControlLanguageCommand& 
SimControlLanguageSubCommand::parent() {
    return parentCommand_;
}


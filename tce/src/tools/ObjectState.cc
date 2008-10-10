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
 * @file ObjectState.cc
 *
 * Implementation of class ObjectState.
 *
 * @author Lasse Laasonen 2003 (lasse.laasonen-no.spam-tut.fi)
 * @note reviewed 8 Jun 2004 by tr, jm, am, ll
 * @note rating: red
 */


#include "ObjectState.hh"
#include "Conversion.hh"
#include "SequenceTools.hh"
#include "ContainerTools.hh"
#include "Application.hh"

using std::string;

/**
 * Constructor.
 *
 * @param name Name of the ObjectState.
 */
ObjectState::ObjectState(const std::string& name) :
    name_(name), value_(""), parent_(NULL) {
}

/**
 * Copy constructor.
 *
 * Makes a deep copy.
 *
 * @param old The ObjectState tree to copy.
 */
ObjectState::ObjectState(const ObjectState& old) :
    name_(old.name_), value_(old.value_), parent_(NULL),
    attributes_(old.attributes_) {

    for (int i = 0; i < old.childCount(); i++) {
        ObjectState* newChild = new ObjectState(*old.child(i));
        addChild(newChild);
    }
}

/**
 * Destructor.
 *
 * Deletes all the children as well and removes itself from the list of child
 * objects of the parent object. That is, the parent object will stay valid.
 */
ObjectState::~ObjectState() {
    if (parent_ != NULL) {
        bool removed = ContainerTools::removeValueIfExists(
            parent_->children_, this);
        assert(removed);
    }
    SequenceTools::deleteAllItems(children_);
}

/**
 * Sets an attribute.
 *
 * If there is an attribute called the given name, its value is changed
 * according to the given value. Otherwise a new attribute is created.
 *
 * @param name Name of the attribute.
 * @param value Value of the attribute.
 */
void
ObjectState::setAttribute(
    const std::string& name,
    const std::string& value) {

    AttributeTable::iterator iter = attributes_.begin();
    while (iter != attributes_.end()) {
        if ((*iter).name == name) {
            (*iter).value = value;
            return;
        }
        iter++;
    }
    Attribute newAttribute;
    newAttribute.name = name;
    newAttribute.value = value;
    attributes_.push_back(newAttribute);
}


/**
 * Sets an attribute.
 *
 * If there is an attribute called the given name, its value is changed
 * according to the given value. Otherwise a new attribute is created.
 *
 * @param name Name of the attribute.
 * @param value Value of the attribute.
 */
void
ObjectState::setAttribute(const std::string& name, int value) {
    string valueAsString = Conversion::toString(value);
    setAttribute(name, valueAsString);
}


/**
 * Sets an attribute.
 *
 * If there is an attribute called the given name, its value is changed
 * according to the given value. Otherwise a new attribute is created.
 *
 * @param name Name of the attribute.
 * @param value Value of the attribute.
 */
void
ObjectState::setAttribute(const std::string& name, unsigned int value) {
    string valueAsString = Conversion::toString(value);
    setAttribute(name, valueAsString);
}


/**
 * Sets an attribute.
 *
 * If there is an attribute called the given name, its value is changed
 * according to the given value. Otherwise a new attribute is created.
 *
 * @param name Name of the attribute.
 * @param value Value of the attribute.
 */
void
ObjectState::setAttribute(const std::string& name, double value) {
    string valueAsString = Conversion::toString(value);
    setAttribute(name, valueAsString);
}


/**
 * Sets an attribute.
 *
 * If there is an attribute called the given name, its value is changed
 * according to the given value. Otherwise a new attribute is created.
 *
 * @param name Name of the attribute.
 * @param value Value of the attribute.
 */
void
ObjectState::setAttribute(const std::string& name, bool value) {
    string valueAsString = Conversion::toString(value);
    setAttribute(name, valueAsString);
}


/**
 * Returns true if the ObjectState has an attribute with the given name.
 *
 * @param name Name of the attribute.
 * @return True if the ObjectState has an attribute with the given name.
 */
bool
ObjectState::hasAttribute(const std::string& name) const {
    AttributeTable::const_iterator iter = attributes_.begin();
    while (iter != attributes_.end()) {
        if ((*iter).name == name) {
            return true;
        }
        iter++;
    }
    return false;
}


/**
 * Returns an attribute by the given index.
 *
 * The value of index must be greater or equal to 0 and less than the
 * number of attributes.
 *
 * @param index The index of the requested attribute.
 * @return Attribute by the given index.
 * @exception OutOfRange If the given index is out of range.
 */
ObjectState::Attribute*
ObjectState::attribute(int index) const
    throw (OutOfRange) {

    if (index < 0 || index >= attributeCount()) {
        const string procName = "ObjectState::attribute";
        string errorMsg = commonErrorMessage();
        errorMsg += "Requested attribute by index '";
        errorMsg += Conversion::toString(index);
        errorMsg += "' which is out of range.";
        throw OutOfRange(__FILE__, __LINE__, procName, errorMsg);
    } else {
        return const_cast<Attribute*>(&attributes_[index]);
    }
}


/**
 * Returns the value of the requested attribute.
 *
 * @param name Name of the attribute.
 * @return Value of the requested attribute.
 * @exception KeyNotFound If no attribute called the given name is found.
 */
std::string
ObjectState::stringAttribute(const std::string& name) const
    throw (KeyNotFound) {

    AttributeTable::const_iterator iter = attributes_.begin();
    while (iter != attributes_.end()) {
        if ((*iter).name == name) {
            return (*iter).value;
        }
        iter++;
    }

    string procName = "ObjectState::stringAttribute";
    string errorMsg = commonErrorMessage();
    errorMsg += "Requested attribute by name '";
    errorMsg += name;
    errorMsg += "' which doesn't exist.";
    throw KeyNotFound(__FILE__, __LINE__, procName, errorMsg);
}


/**
 * Returns the value of the requested attribute.
 *
 * @param name Name of the attribute.
 * @return Value of the requested attribute.
 * @exception KeyNotFound If no attribute called the given name is found.
 * @exception NumberFormatException If the value of the requested attribute
 *                                  can't be converted to int.
 */
int
ObjectState::intAttribute(const std::string& name) const
    throw (KeyNotFound, NumberFormatException) {

    string value = stringAttribute(name);
    return Conversion::toInt(value);
}


/**
 * Returns the value of the requested attribute.
 *
 * @param name Name of the attribute.
 * @return Value of the requested attribute.
 * @exception KeyNotFound If no attribute called the given name is found.
 * @exception NumberFormatException If the value of the requested attribute
 *                                  can't be converted to unsigned int.
 */
unsigned int
ObjectState::unsignedIntAttribute(const std::string& name) const
    throw (KeyNotFound, NumberFormatException) {

    string value = stringAttribute(name);
    return Conversion::toUnsignedInt(value);
}


/**
 * Returns the value of the requested attribute.
 *
 * @param name Name of the attribute.
 * @return Value of the requested attribute.
 * @exception KeyNotFound If no attribute called the given name is found.
 * @exception NumberFormatException If the value of the requested attribute
 *                                  can't be converted to double.
 */
double
ObjectState::doubleAttribute(const std::string& name) const
    throw (KeyNotFound, NumberFormatException) {

    string value = stringAttribute(name);
    return Conversion::toDouble(value);
}


/**
 * Returns the value of the requested attribute.
 *
 * @param name Name of the attribute.
 * @return Value of the requested attribute.
 * @exception KeyNotFound If no attribute called the given name is found.
 * @exception TypeMismatch If the value of the requested attribute can't be
 *                         converted to bool.
 */
bool
ObjectState::boolAttribute(const std::string& name) const
    throw (KeyNotFound, TypeMismatch) {

    string value = stringAttribute(name);

    int intValue;
    try {
        intValue = Conversion::toInt(value);
    } catch (const NumberFormatException&) {
        const string procName = "ObjectState::boolAttribute";
        throw TypeMismatch(__FILE__, __LINE__, procName);
    }

    return intValue;
}


/**
 * Returns true if there is a child element called the given name.
 *
 * @return True if there is a child element called the given name.
 */
bool
ObjectState::hasChild(const std::string& name) const {
    ChildTable::const_iterator iter = children_.begin();
    while (iter != children_.end()) {
        if ((*iter)->name() == name) {
            return true;
        }
        iter++;
    }
    return false;
}


/**
 * Adds a child object.
 *
 * @param child ObjectState object to be added as a child.
 */
void
ObjectState::addChild(ObjectState* child) {
    children_.push_back(child);
    child->parent_ = this;
}


/**
 * Removes the given child object.
 *
 * @param child The child object to be removed.
 * @exception InstanceNotFound If the object does not have the given child
 *                             object.
 */
void
ObjectState::removeChild(ObjectState* child)
    throw (InstanceNotFound) {

    bool removed = ContainerTools::removeValueIfExists(children_, child);
    if (removed) {
        child->parent_ = NULL;
    } else {
        const string procName = "ObjectState::removeChild";
        throw InstanceNotFound(__FILE__, __LINE__, procName);
    }
}


/**
 * Replaces a child with a new child.
 *
 * @param old Child to be replaced.
 * @param newChild Child to be added in the place of the old child.
 * @exception InvalidData If the new child given already has a parent
 *                          object.
 */
void
ObjectState::replaceChild(ObjectState* old, ObjectState* newChild)
    throw (InvalidData) {

    if (newChild->parent() != NULL) {
        const string procName = "ObjectState::replaceChild";
        throw InvalidData(__FILE__, __LINE__, procName);
    }

    ChildTable::iterator it = children_.begin();
    while (it != children_.end()) {
        if (*it == old) {
            ObjectState* toBeRemoved = *it;
            ChildTable::iterator pt = it;
            pt++;
            if (pt == children_.end()) {
                children_.push_back(newChild);
            } else {
                children_.insert(pt, newChild);
            }
            newChild->parent_ = this;
            delete toBeRemoved;
            break;
        }
        it++;
    }
}

/**
 * Returns the child of the requested name.
 *
 * If there are many children of that name, returns the first of them.
 *
 * @param name Name of the child.
 * @return First child of the requested name.
 * @exception InstanceNotFound If the requested child is not found.
 */
ObjectState*
ObjectState::childByName(const std::string& name) const
    throw (InstanceNotFound) {

    ChildTable::const_iterator iter = children_.begin();
    while (iter != children_.end()) {
        if ((*iter)->name() == name) {
            return *iter;
        }
        iter++;
    }

    string procName = "ObjectState::childByName";
    string errorMsg = commonErrorMessage();
    errorMsg += "Requested child by name '";
    errorMsg += name;
    errorMsg += "' which doesn't exist.";
    throw InstanceNotFound(__FILE__, __LINE__, procName, errorMsg);
}


/**
 * Returns a child by the given index.
 *
 * The index must be greater or equal to 0 and less than the number of
 * children.
 *
 * @param index The index.
 * @return Child by the given index.
 * @exception OutOfRange If the given index is out of range.
 */
ObjectState*
ObjectState::child(int index) const
    throw (OutOfRange) {

    if (index < 0 || index >= childCount()) {
        const string procName = "ObjectState::child";
        string errorMsg = commonErrorMessage();
        errorMsg += "Requested child by index '";
        errorMsg += Conversion::toString(index);
        errorMsg += "' which is out of range.";
        throw OutOfRange(__FILE__, __LINE__, procName, errorMsg);
    } else {
        return children_[index];
    }
}


/**
 * Inequality comparison operator.
 *
 * @param object Object in which this object is compared to.
 * @return True if objects are inequal, false otherwise.
 */
bool
ObjectState::operator!=(const ObjectState& object) {

    if (name_ != object.name() ||
        value_ != object.stringValue() ||
        children_.size() != static_cast<unsigned int>(object.childCount()) ||
        static_cast<int>(attributes_.size()) != object.attributeCount()) {

        return true;
    }

    for (size_t i = 0; i < attributes_.size(); i++) {
        Attribute attr = attributes_[i];
        if (!object.hasAttribute(attr.name)) {
            return true;
        } else {
            if (attr.value != object.stringAttribute(attr.name)) {
                return true;
            }
        }
    }

    for (size_t i = 0; i < children_.size(); i++) {
        ObjectState& child = *children_[i];
        ObjectState& compare = *object.child(i);
        if (child != compare) {
            return true;
        }
    }

    return false;
}

/**
 * Generates a common beginning of error messages.
 *
 * @return The generated error message.
 */
std::string
ObjectState::commonErrorMessage() const {
    string errorMsg = "ObjectState ";
    errorMsg += name();
    errorMsg += ": ";
    return errorMsg;
}

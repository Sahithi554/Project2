/**
 * @file Component.cpp
 * @author sahithi
 * brief Implementation for base Component class.
 */

#include "pch.h"
#include "Component.h"
#include "Machine.h"



/**
 * @brief Component constructor.
 * @param machine The machine this component belongs to.
 *
 * Initializes the component attached to the specified machine.
 */
Component::Component(Machine* machine) : mMachine(machine)
{
}

/**
 * @brief Default rotation setter.
 * @param rotation Rotation in turns (0-1).
 *
 * Default implementation does nothing. Components that need to respond
 * to rotation should override this method.
 */
void Component::SetRotation(double rotation)
{

}

/**
 * @brief Set the rotation source for this component.
 * @param source The rotation source.
 *
 * Stores the pointer to the rotation source; concrete components may
 * subscribe to the source or use its values in Update/Draw as needed.
 */
void Component::SetSource(RotationSource* source)
{
    mRotationSource = source;
}

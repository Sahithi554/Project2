/**
* @file Component.cpp
 * @author Sahithi Nalamalpu
 */

#include "pch.h"
#include "Component.h"
#include "Machine.h"

/**
 * Constructor
 */
Component::Component(Machine* machine) : mMachine(machine)
{
}

/**
 * Default rotation setter
 */
void Component::SetRotation(double rotation)
{
    // Default implementation does nothing
}

/**
 * Set rotation source
 */
void Component::SetSource(RotationSource* source)
{
    mRotationSource = source;
}

/**
 * Rotate with speed
 */
void Component::Rotate(double rotation, double speed)
{
    SetRotation(rotation);
}
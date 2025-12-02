/**
* @file RotationSource.cpp
 * @author Sahithi Nalamalpu
 * @version 1.0
 *
 * Provides the implementation of RotationSource for distributing rotation
 * to registered components.
 */

#include "pch.h"
#include "RotationSource.h"

/**
 * Assign a new rotation value and propagate it to all connected sinks.
 * @param angle Rotation value in turns (0 to 1)
 */
void RotationSource::SetRotation(double angle)
{
    mRotation = angle;
    // Update all registered sinks with the current rotation
    for (auto sink : mSinks)
    {
        sink->SetRotation(angle);
    }
}

/**
 * Assign rotation and speed values and propagate them to all connected sinks.
 * @param angle Rotation value in turns (0 to 1)
 * @param speed Rotation speed in turns per second
 */
void RotationSource::SetRotation(double angle, double speed)
{
    mRotation = angle;
    mSpeed = speed;
    // Update all registered sinks with both rotation and speed
    for (auto sink : mSinks)
    {
        sink->Rotate(angle, speed);
    }
}

/**
 * Register a new rotation sink to receive updates.
 * @param sink Pointer to a component implementing IRotationSink
 */
void RotationSource::AddSink(IRotationSink* sink)
{
    mSinks.push_back(sink);
    sink->SetSource(this);  // Link the sink back to this source
}

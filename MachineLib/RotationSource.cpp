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
    // Record internal state
    mRotation = angle;

    if (mSinks.empty()) {
        return;    // Nothing to update, bail early
    }

    // Broadcast using functional style
    std::for_each(mSinks.begin(), mSinks.end(),
        [angle](IRotationSink* s)
        {
            if (s)
            {
                s->SetRotation(angle);
            }
        }
    );
}

/**
 * Assign rotation and speed values and propagate them to all connected sinks.
 * @param angle Rotation value in turns (0 to 1)
 * @param speed Rotation speed in turns per second
 */
void RotationSource::SetRotation(double angle, double speed)
{
    // Store new state
    mRotation = angle;
    mSpeed    = speed;

    if (mSinks.empty()) {
        return;
    }

    // Again, functional broadcasting
    std::for_each(mSinks.begin(), mSinks.end(),
        [angle, speed](IRotationSink* s)
        {
            if (s)
            {
                s->Rotate(angle, speed);
            }
        }
    );
}

/**
 * Register a new rotation sink to receive updates.
 * @param sink Pointer to a component implementing IRotationSink
 */
void RotationSource::AddSink(IRotationSink* sink)
{
    if (!sink) {
        return;
    }

    // Register the sink
    mSinks.emplace_back(sink);

    // Connect sink → source link
    sink->SetSource(this);

}

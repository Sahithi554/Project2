/**
* @file RotationSource.h
 * @author Sahithi Nalamalpu
 * @version 1.0
 *
 * Class that provides rotation to rotation sinks.
 */

#ifndef CANADIANEXPERIENCE_ROTATIONSOURCE_H
#define CANADIANEXPERIENCE_ROTATIONSOURCE_H

#include "IRotationSink.h"
#include <vector>

/**
 * Class that provides rotation to rotation sinks.
 */
class RotationSource {
private:
    /// Current rotation value in turns (0-1)
    double mRotation = 0;

    /// Current rotation speed in turns per second
    double mSpeed = 0;

    /// List of rotation sinks that receive rotation from this source
    std::vector<IRotationSink*> mSinks;

public:
    RotationSource() = default;

    /// Copy constructor (disabled)
    RotationSource(const RotationSource &) = delete;

    /// Assignment operator (disabled)
    void operator=(const RotationSource &) = delete;

    /**
     * Get the current rotation
     * @return Rotation in turns (0-1)
     */
    double GetRotation() const { return mRotation; }

    /**
     * Get the current rotation speed
     * @return Speed in turns per second
     */
    double GetSpeed() const { return mSpeed; }

    /**
     * Set the rotation and notify all sinks
     * @param rotation Rotation in turns (0-1)
     */
    void SetRotation(double rotation);

    /**
     * Set the rotation and speed, then notify all sinks
     * @param rotation Rotation in turns (0-1)
     * @param speed Rotation speed in turns per second
     */
    void SetRotation(double rotation, double speed);

    /**
     * Add a rotation sink to receive rotation updates
     * @param sink The rotation sink to add
     */
    void AddSink(IRotationSink* sink);
};

#endif // CANADIANEXPERIENCE_ROTATIONSOURCE_H

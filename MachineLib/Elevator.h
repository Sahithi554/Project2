/**
 * @file Elevator.h
 * @author Sahithi Nalamalpu
 * @brief An elevator component that moves vertically.
 *
 * The elevator platform itself moves up and down.
 * Elevators are rotation sinks (driven by pulleys).
 * Objects on the platform automatically move with it via physics.
 */

#ifndef CANADIANEXPERIENCE_ELEVATOR_H
#define CANADIANEXPERIENCE_ELEVATOR_H

#include "Component.h"
#include "PhysicsPolygon.h"
#include <memory>

class Machine;

/**
 * Elevator component that moves vertically.
 *
 * An elevator is driven by a rotation source (pulley) and the platform
 * itself moves up and down. Objects on the platform move with it
 * automatically through physics contact.
 */
class Elevator : public Component
{
private:
    /// The polygon representing the elevator platform
    std::shared_ptr<cse335::PhysicsPolygon> mPolygon;

    /// Current rotation speed in turns per second (from rotation source)
    double mSpeed = 0;

    /// Elevator speed multiplier (converts rotation speed to linear velocity)
    /// This controls how fast the elevator moves per turn of the pulley
    double mSpeedMultiplier = 200.0; // cm/s per turn/s

    /// Initial position (to remember where elevator starts)
    wxPoint2DDouble mInitialPosition;

public:
    /**
     * Constructor
     * @param machine The machine this component belongs to
     */
    Elevator(Machine* machine);

    /// Destructor
    virtual ~Elevator() = default;

    /// Copy constructor (disabled)
    Elevator(const Elevator &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Elevator &) = delete;

    /**
     * Set the image for this elevator
     * @param path Path to the image file
     */
    void SetImage(const std::wstring& path);

    /**
     * Set the initial position of this elevator
     * @param x X-coordinate in centimeters
     * @param y Y-coordinate in centimeters
     */
    void SetPosition(double x, double y);

    /**
     * Set the size of the elevator platform
     * @param width Width in centimeters
     * @param height Height in centimeters
     */
    void SetSize(double width, double height);

    /**
     * Draw the elevator
     * @param graphics Graphics context to draw on
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    /**
     * Set the rotation for this elevator (from rotation source)
     * Not used for elevators - they move linearly, not rotationally
     * @param rotation Rotation in turns (0-1)
     */
    void SetRotation(double rotation) override;

    /**
     * Rotate this elevator with both rotation angle and speed
     * Converts rotational motion to linear vertical motion
     * @param rotation Rotation in turns (0-1) - not used
     * @param speed Rotation speed in turns per second
     */
    void Rotate(double rotation, double speed) override;

    /**
     * Update the elevator based on elapsed time
     * Moves the platform vertically
     * @param time Elapsed time in seconds
     */
    void Update(double time) override;

    /**
     * Get the polygon representing this elevator
     * @return Shared pointer to PhysicsPolygon
     */
    std::shared_ptr<cse335::PhysicsPolygon> GetPolygon() const { return mPolygon; }

    /**
     * Set the speed multiplier (how fast the platform moves per rotation speed)
     * @param multiplier Speed multiplier in cm/s per turn/s
     */
    void SetSpeedMultiplier(double multiplier) { mSpeedMultiplier = multiplier; }
};

#endif // CANADIANEXPERIENCE_ELEVATOR_H
/**
 * @file Conveyor.h
 * @author Sahithi Nalamalpu
 * @brief A conveyor component that moves objects horizontally.
 *
 * Conveyors are rotation sinks (can be driven by motors or pulleys).
 * When driven, they apply horizontal velocity to objects on their surface.
 */

#ifndef CANADIANEXPERIENCE_CONVEYOR_H
#define CANADIANEXPERIENCE_CONVEYOR_H

#include "Component.h"
#include "PhysicsPolygon.h"
#include <memory>

class Machine;

/**
 * Conveyor component that moves objects horizontally.
 *
 * A conveyor is driven by a rotation source and applies horizontal
 * velocity to objects that come into contact with its surface.
 */
class Conveyor : public Component
{
private:
    /// The polygon representing the conveyor surface
    std::shared_ptr<cse335::PhysicsPolygon> mPolygon;

    /// Current rotation speed in turns per second (from rotation source)
    double mSpeed = 0;

    /// Conveyor speed multiplier (converts rotation speed to linear velocity)
    double mSpeedMultiplier = 50.0; // cm/s per turn/s

public:
    /**
     * Constructor
     * @param machine The machine this component belongs to
     */
    Conveyor(Machine* machine);

    /// Destructor
    virtual ~Conveyor() = default;

    /// Copy constructor (disabled)
    Conveyor(const Conveyor &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Conveyor &) = delete;

    /**
     * Set the image for this conveyor
     * @param path Path to the image file
     */
    void SetImage(const std::wstring& path);

    /**
     * Set the position of this conveyor
     * @param x X-coordinate in centimeters
     * @param y Y-coordinate in centimeters
     */
    void SetPosition(double x, double y);

    /**
     * Set the size of the conveyor
     * @param width Width in centimeters
     * @param height Height in centimeters
     */
    void SetSize(double width, double height);

    /**
     * Draw the conveyor
     * @param graphics Graphics context to draw on
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    /**
     * Set the rotation for this conveyor (from rotation source)
     * @param rotation Rotation in turns (0-1)
     */
    void SetRotation(double rotation) override;

    /**
     * Rotate this conveyor with both rotation angle and speed
     * @param rotation Rotation in turns (0-1)
     * @param speed Rotation speed in turns per second
     */
    void Rotate(double rotation, double speed) override;

    /**
     * Update the conveyor based on elapsed time
     * @param time Elapsed time in seconds
     */
    void Update(double time) override;

    /**
     * Get the polygon representing this conveyor
     * @return Shared pointer to PhysicsPolygon
     */
    std::shared_ptr<cse335::PhysicsPolygon> GetPolygon() const { return mPolygon; }

    /**
     * Set the speed multiplier (how fast objects move per rotation speed)
     * @param multiplier Speed multiplier in cm/s per turn/s
     */
    void SetSpeedMultiplier(double multiplier) { mSpeedMultiplier = multiplier; }
};

#endif // CANADIANEXPERIENCE_CONVEYOR_H


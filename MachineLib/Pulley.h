/**
 * @file Pulley.h
 * @author Sahithi Nalamalpu
 * @brief A pulley component that can receive and transmit rotation.
 *
 * Pulleys are both rotation sinks (can be driven by motors or other pulleys)
 * and rotation sources (can drive other pulleys, conveyors, elevators, or shapes).
 * The speed ratio between pulleys is determined by their relative radii.
 */

#ifndef CANADIANEXPERIENCE_PULLEY_H
#define CANADIANEXPERIENCE_PULLEY_H

#include "Component.h"
#include "RotationSource.h"
#include "Polygon.h"
#include <memory>
#include <vector>

class Machine;

/**
 * Pulley component that transfers rotation between components.
 *
 * A pulley has a radius that determines its rotational speed when
 * driven by another pulley. Larger pulleys turn slower than smaller
 * pulleys when connected by a belt.
 */
class Pulley : public Component
{
private:
    /// The polygon used to draw this pulley
    std::shared_ptr<cse335::Polygon> mPolygon;

    /// Rotation source for driving other components
    std::unique_ptr<RotationSource> mRotationSource;

    /// Current rotation angle in turns (0-1)
    double mRotation = 0;

    /// Radius of this pulley in centimeters
    double mRadius = 10;

    /// Current rotational speed in turns per second
    double mSpeed = 0;

    /// List of pulleys this pulley drives
    std::vector<Pulley*> mDrivenPulleys;

public:
    /**
     * Constructor
     * @param machine The machine this component belongs to
     * @param radius Radius of the pulley in centimeters
     */
    Pulley(Machine* machine, double radius);

    /// Destructor
    virtual ~Pulley() = default;

    /// Copy constructor (disabled)
    Pulley(const Pulley &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Pulley &) = delete;

    /**
     * Set the image for this pulley
     * @param path Path to the image file
     */
    void SetImage(const std::wstring& path);

    /**
     * Draw the belts connecting this pulley to driven pulleys
     * @param graphics Graphics context to draw on
     */
    void DrawBelts(std::shared_ptr<wxGraphicsContext> graphics);

    /**
     * Draw the pulley
     * @param graphics Graphics context to draw on
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    /**
     * Set the rotation for this pulley (from rotation source)
     * @param rotation Rotation in turns (0-1)
     */
    void SetRotation(double rotation) override;

    /**
     * Rotate this pulley with both rotation angle and speed
     * @param rotation Rotation in turns (0-1)
     * @param speed Rotation speed in turns per second
     */
    void Rotate(double rotation, double speed) override;

    /**
     * Update the pulley based on elapsed time
     * @param time Elapsed time in seconds
     */
    void Update(double time) override;

    /**
     * Set the position of this pulley
     * @param x X-coordinate in centimeters
     * @param y Y-coordinate in centimeters
     */
    void SetPosition(double x, double y);

    /**
     * Drive another pulley with this pulley
     * @param driven The pulley to be driven by this one
     */
    void Drive(Pulley* driven);

    /**
     * Get the rotation source for this pulley
     * @return Pointer to the rotation source
     */
    RotationSource* GetRotationSource() const { return mRotationSource.get(); }

    /**
     * Get the radius of this pulley
     * @return Radius in centimeters
     */
    double GetRadius() const { return mRadius; }

    /**
     * Get the current rotation angle
     * @return Rotation in turns (0-1)
     */
    double GetRotation() const { return mRotation; }

    /**
     * Get the current rotational speed
     * @return Speed in turns per second
     */
    double GetSpeed() const { return mSpeed; }

    /**
     * Set the rotational speed directly
     * @param speed Speed in turns per second
     */
    void SetSpeed(double speed) { mSpeed = speed; }
};

#endif // CANADIANEXPERIENCE_PULLEY_H
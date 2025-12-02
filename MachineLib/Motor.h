/**
 * @file Motor.h
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * A motor component that can drive other components and propagate rotation.
 */

#ifndef CANADIANEXPERIENCE_MOTOR_H
#define CANADIANEXPERIENCE_MOTOR_H

#include "Component.h"
#include "RotationSource.h"
#include "PhysicsPolygon.h"
#include "Polygon.h"
#include <memory>
#include <vector>
#include <b2_world_callbacks.h>

class Machine;
class b2Contact;

/**
 * Motor component that can rotate and drive other components.
 */
class Motor : public Component, public b2ContactListener {
private:
    double mSpeed = 0;                     ///< Motor speed in turns per second
    bool mIsActive = false;                ///< True if motor is active
    double mRotation = 0;                  ///< Current rotation (0-1)
    bool mIsOscillating = false;           ///< True if motor oscillates

    std::shared_ptr<cse335::PhysicsPolygon> mBox; ///< Box polygon of motor
    std::shared_ptr<cse335::Polygon> mWheel;      ///< Wheel polygon of motor
    std::vector<std::shared_ptr<cse335::Polygon>> mImages;      ///< Motor animation images
    std::vector<std::shared_ptr<cse335::Polygon>> mImagesHuman; ///< Human animation images (optional)
    bool mIsHumanActive = false;           ///< True if human animation active

    std::unique_ptr<RotationSource> mRotationSource; ///< Rotation source for driving other components
    std::wstring mImagesDir;               ///< Directory of motor images

public:
    Motor(Machine* machine, std::wstring imagesDir); ///< Constructor
    virtual ~Motor() = default;                       ///< Destructor

    Motor(const Motor &) = delete;                   ///< Disable copy constructor
    void operator=(const Motor &) = delete;          ///< Disable assignment

    void SetSpeed(double speed) { mSpeed = speed; }  ///< Set motor speed
    void AddRotationSink(Component* sink);           ///< Add a rotation sink
    void SetOscillating(bool oscillation) { mIsOscillating = oscillation; } ///< Enable oscillation
    void SetOscillation(bool oscillation) { mIsOscillating = oscillation; } ///< Enable oscillation (alias)
    void BeginContact(b2Contact* contact) override;  ///< Handle contact events
    void SetInitiallyRunning(bool running) { mIsActive = running; } ///< Set initial active state
    void SetInitiallyActive(bool active) { mIsActive = active; } ///< Set initial active state (alias)
    void SetSpread(double spread); ///< Set oscillation spread
    void SetWindupToReleaseRatio(double ratio); ///< Set windup to release ratio
    void Update(double time) override;               ///< Update motor based on time
    void SetPosition(wxPoint2DDouble position);      ///< Set motor position
    void SetPosition(double x, double y);           ///< Set motor position (overload)
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override; ///< Draw motor

    RotationSource* GetRotationSource() const { return mRotationSource.get(); } ///< Get rotation source
    RotationSource* GetSource() const { return mRotationSource.get(); } ///< Get rotation source (alias)
    bool IsActive() const { return mIsActive; }                                  ///< Check if active
    std::shared_ptr<cse335::PhysicsPolygon> GetBox() const { return mBox; }      ///< Get motor box polygon
    std::shared_ptr<cse335::Polygon> GetWheel() const { return mWheel; }         ///< Get motor wheel polygon
    wxPoint2DDouble GetShaftPosition() const; ///< Get the position of the motor shaft
};

#endif //CANADIANEXPERIENCE_MOTOR_H

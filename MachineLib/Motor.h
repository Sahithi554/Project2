/**
 * @file Motor.h
 * @author Sahithi Nalamalpu
 * @brief Motor component with state machine
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
 * @brief Motor component with state machine
 */
class Motor : public Component, public b2ContactListener {
private:
    /// Motor state enumeration
    enum class State {
        Idle,
        Active
    };

    /// Motor box
    std::shared_ptr<cse335::PhysicsPolygon> mBox;

    /// Motor wheel
    std::shared_ptr<cse335::Polygon> mWheel;

    /// Animation frames
    std::vector<std::shared_ptr<cse335::Polygon>> mImages;

    /// Rotation source
    std::unique_ptr<RotationSource> mRotationSource;

    /// Resources directory
    std::wstring mImagesDir;

    /// Current state
    State mState = State::Idle;

    /// Motor speed (turns per second)
    double mSpeed = 1.0;

    /// Current rotation
    double mRotation = 0.0;

    /// Is oscillating
    bool mIsOscillating = false;

    /// Oscillation spread
    double mSpread = 0.1;

    /// Windup to release ratio
    double mWindupToReleaseRatio = 5.0;

    void LoadAnimationFrames();
    double CalculateRotation(double time);

public:
    Motor(Machine* machine, std::wstring imagesDir);
    virtual ~Motor() = default;

    Motor(const Motor &) = delete;
    void operator=(const Motor &) = delete;

    // Configuration
    void SetSpeed(double speed) { mSpeed = speed; }
    void SetOscillating(bool oscillating) { mIsOscillating = oscillating; }
    void SetOscillation(bool oscillation) { mIsOscillating = oscillation; }
    void SetInitiallyRunning(bool running) { if (running) mState = State::Active; }
    void SetInitiallyActive(bool active) { if (active) mState = State::Active; }
    void SetSpread(double spread) { mSpread = spread; }
    void SetWindupToReleaseRatio(double ratio) { mWindupToReleaseRatio = ratio; }

    // Component interface
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void Update(double time) override;

    // Position management
    void SetPosition(wxPoint2DDouble position);
    void SetPosition(double x, double y);

    // Contact listener
    void BeginContact(b2Contact* contact) override;

    // Accessors
    RotationSource* GetRotationSource() const { return mRotationSource.get(); }
    RotationSource* GetSource() const { return mRotationSource.get(); }
    std::shared_ptr<cse335::PhysicsPolygon> GetBox() const { return mBox; }
    std::shared_ptr<cse335::Polygon> GetWheel() const { return mWheel; }
    wxPoint2DDouble GetShaftPosition() const;
    bool IsActive() const { return mState == State::Active; }
    void AddRotationSink(Component* sink);
};

#endif // CANADIANEXPERIENCE_MOTOR_H
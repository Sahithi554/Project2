/**
 * @file Motor.h
 * @author Sahithi Nalamalpu
 * @brief Motor component with state machine - FIXED VERSION
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
 *
 * The motor can be in two states: Idle or Active
 * - Idle: Shows the idle runner image, no rotation
 * - Active: Shows cycling runner animation, rotates wheel and drives other components
 */
class Motor : public Component, public b2ContactListener {
private:
    /// Motor state enumeration
    enum class State {
        Idle,    ///< Motor is not running
        Active   ///< Motor is running (either hit or initially active)
    };

    /// Motor box (the physics body)
    std::shared_ptr<cse335::PhysicsPolygon> mBox;

    /// Motor wheel (rotates when active)
    std::shared_ptr<cse335::Polygon> mWheel;

    /// Animation frames: [0] = idle, [1-4] = active animation cycle
    std::vector<std::shared_ptr<cse335::Polygon>> mImages;

    /// Rotation source for driving other components (pulleys, shapes)
    std::unique_ptr<RotationSource> mRotationSource;

    /// Resources directory path
    std::wstring mImagesDir;

    /// Current state (Idle or Active)
    State mState = State::Idle;

    /// Motor speed in turns per second (positive = clockwise, negative = counter-clockwise)
    double mSpeed = 1.0;

    /// Current rotation angle in turns (0.0 to 1.0)
    double mRotation = 0.0;

    /// Is this motor oscillating?
    bool mIsOscillating = false;

    /// Oscillation spread (for oscillating motors)
    double mSpread = 0.1;

    /// Windup to release ratio (for oscillating motors)
    double mWindupToReleaseRatio = 5.0;

    /// Helper to load the 5 animation images
    void LoadAnimationFrames();

public:
    /**
     * Constructor
     * @param machine The machine this motor belongs to
     * @param imagesDir Path to the images directory
     */
    Motor(Machine* machine, std::wstring imagesDir);

    /// Destructor
    virtual ~Motor() = default;

    /// Copy constructor (disabled)
    Motor(const Motor &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Motor &) = delete;

    // ========== Configuration Methods ==========

    /**
     * Set the motor speed
     * @param speed Speed in turns per second (positive = CW, negative = CCW)
     */
    void SetSpeed(double speed) { mSpeed = speed; }

    /**
     * Set whether this motor oscillates
     * @param oscillating True if motor should oscillate
     */
    void SetOscillating(bool oscillating) { mIsOscillating = oscillating; }

    /**
     * Alias for SetOscillating
     * @param oscillation True if motor should oscillate
     */
    void SetOscillation(bool oscillation) { mIsOscillating = oscillation; }

    /**
     * Set whether motor starts running immediately
     * @param running True if motor should start active
     */
    void SetInitiallyRunning(bool running) {
        if (running) mState = State::Active;
    }

    /**
     * Alias for SetInitiallyRunning
     * @param active True if motor should start active
     */
    void SetInitiallyActive(bool active) {
        if (active) mState = State::Active;
    }

    /**
     * Set oscillation spread (for oscillating motors)
     * @param spread Oscillation spread value
     */
    void SetSpread(double spread) { mSpread = spread; }

    /**
     * Set windup to release ratio (for oscillating motors)
     * @param ratio Windup to release ratio
     */
    void SetWindupToReleaseRatio(double ratio) { mWindupToReleaseRatio = ratio; }

    // ========== Component Interface ==========

    /**
     * Draw the motor
     * @param graphics Graphics context to draw on
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    /**
     * Update the motor based on elapsed time
     * @param elapsed Elapsed time in seconds
     */
    void Update(double elapsed) override;

    // ========== Position Management ==========

    /**
     * Set the position of this motor
     * @param position Position in centimeters
     */
    void SetPosition(wxPoint2DDouble position);

    /**
     * Set the position of this motor
     * @param x X-coordinate in centimeters
     * @param y Y-coordinate in centimeters
     */
    void SetPosition(double x, double y);

    // ========== Contact Listener (Box2D) ==========

    /**
     * Called when something collides with the motor box
     * This activates the motor
     * @param contact The Box2D contact object
     */
    void BeginContact(b2Contact* contact) override;

    // ========== Accessors ==========

    /**
     * Get the rotation source for this motor
     * @return Pointer to the rotation source
     */
    RotationSource* GetRotationSource() const { return mRotationSource.get(); }

    /**
     * Alias for GetRotationSource
     * @return Pointer to the rotation source
     */
    RotationSource* GetSource() const { return mRotationSource.get(); }

    /**
     * Get the motor box (physics body)
     * @return Shared pointer to the box PhysicsPolygon
     */
    std::shared_ptr<cse335::PhysicsPolygon> GetBox() const { return mBox; }

    /**
     * Get the motor wheel
     * @return Shared pointer to the wheel Polygon
     */
    std::shared_ptr<cse335::Polygon> GetWheel() const { return mWheel; }

    /**
     * Get the shaft position (where pulleys connect)
     * @return Shaft position in centimeters
     */
    wxPoint2DDouble GetShaftPosition() const;

    /**
     * Check if motor is active
     * @return True if motor is running
     */
    bool IsActive() const { return mState == State::Active; }

    /**
     * Add a rotation sink to be driven by this motor
     * @param sink The component to be driven
     */
    void AddRotationSink(Component* sink);
};

#endif // CANADIANEXPERIENCE_MOTOR_H
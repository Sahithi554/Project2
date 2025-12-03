/**
 * @file Motor.cpp
 * @author Sahithi Nalamalpu
 * FIXED VERSION - Animation cycling corrected
 */

#include "pch.h"
#include "Motor.h"
#include "Machine.h"
#include <b2_contact.h>

/// The center point for drawing the wheel relative to bottom center of box
const auto WheelCenter = wxPoint2DDouble(-12, 24);

/// How fast the motor runs. This is how many cycles of the 4 images
/// we make per revolution of the wheel (as 1, 2, 3, 4, 1, 2, 3, ...)
const double MotorSpeed = 4.0;

/**
 * Constructor
 */
Motor::Motor(Machine* machine, std::wstring imagesDir)
    : Component(machine), mImagesDir(imagesDir)
{
    mBox = std::make_shared<cse335::PhysicsPolygon>();
    mWheel = std::make_shared<cse335::Polygon>();
    mRotationSource = std::make_unique<RotationSource>();

    // Setup box
    mBox->SetImage(imagesDir + L"/motor-box.png");
    mBox->BottomCenteredRectangle(75, 50);

    // Setup wheel
    mWheel->SetImage(imagesDir + L"/wheel.png");
    mWheel->CenteredSquare(45);

    LoadAnimationFrames();
}

/**
 * Load animation frames
 */
void Motor::LoadAnimationFrames()
{
    const std::wstring frames[5] = {
        L"/motor-idle.png",
        L"/motor-active-1.png",
        L"/motor-active-2.png",
        L"/motor-active-3.png",
        L"/motor-active-4.png"
    };

    for (int i = 0; i < 5; i++) {
        auto frame = std::make_shared<cse335::Polygon>();
        frame->SetImage(mImagesDir + frames[i]);
        frame->CenteredSquare(45);
        mImages.push_back(frame);
    }
}

/**
 * Begin contact - activates the motor when something hits it
 */
void Motor::BeginContact(b2Contact* contact)
{
    if (mBox && GetBody()) {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        if (bodyA == GetBody() || bodyB == GetBody()) {
            mState = State::Active;
        }
    }
}

/**
 * Update - handles rotation and animation
 */
void Motor::Update(double elapsed)
{
    // Only update if active (or initially running)
    if (mState == State::Active) {
        // Update rotation based on speed
        mRotation += mSpeed * elapsed;

        // Keep rotation in range [0, 1)
        while (mRotation >= 1.0) mRotation -= 1.0;
        while (mRotation < 0.0) mRotation += 1.0;

        // Notify rotation source so it can drive other components
        if (mRotationSource) {
            mRotationSource->SetRotation(mRotation, mSpeed);
        }
    }
}

/**
 * Set position
 */
void Motor::SetPosition(wxPoint2DDouble position)
{
    Component::SetPosition(position);
    if (mBox) {
        mBox->SetInitialPosition(position.m_x, position.m_y);
    }
}

void Motor::SetPosition(double x, double y)
{
    SetPosition(wxPoint2DDouble(x, y));
}

/**
 * Draw - renders the motor with animation
 */
void Motor::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw box first
    if (mBox) {
        mBox->Draw(graphics);
    }

    // Calculate positions
    wxPoint2DDouble boxPos = mBox ? mBox->GetPosition() : GetPosition();
    wxPoint2DDouble shaftPos(boxPos.m_x + WheelCenter.m_x, boxPos.m_y + WheelCenter.m_y);

    // Draw the runner (animated person) if we have images
    if (!mImages.empty()) {
        graphics->PushState();
        graphics->Translate(shaftPos.m_x, shaftPos.m_y);

        // Mirror if speed is negative (counter-clockwise)
        if (mSpeed < 0) {
            graphics->Scale(-1.0, 1.0);
        }

        if (mState == State::Active) {
            // Active: Cycle through frames 1, 2, 3, 4
            // MotorSpeed = 4.0 means 4 complete cycles per revolution
            // So we go through all 4 frames 4 times per revolution
            // Total of 16 frame changes per revolution (one image every 1/16 of a revolution)

            double framesPerRevolution = MotorSpeed * 4.0; // 16 frames total per revolution
            int frameIndex = (int)(mRotation * framesPerRevolution) % 4; // Gives 0, 1, 2, 3
            frameIndex += 1; // Shift to use images 1, 2, 3, 4 (not 0)

            // Ensure we're in valid range
            if (frameIndex >= 1 && frameIndex <= 4 && frameIndex < (int)mImages.size()) {
                mImages[frameIndex]->DrawPolygon(graphics, 0, 0, 0);
            }
        } else {
            // Idle: Draw frame 0 (idle image)
            mImages[0]->DrawPolygon(graphics, 0, 0, 0);
        }

        graphics->PopState();
    }

    // Draw wheel if active
    if (mState == State::Active && mWheel) {
        mWheel->DrawPolygon(graphics, shaftPos.m_x, shaftPos.m_y, mRotation);
    }
}

/**
 * Get shaft position
 */
wxPoint2DDouble Motor::GetShaftPosition() const
{
    wxPoint2DDouble boxPos = mBox ? mBox->GetPosition() : GetPosition();
    // The shaft is offset from the box position
    // Offset is 25 cm right and 40 cm up from bottom center
    return wxPoint2DDouble(boxPos.m_x + 25, boxPos.m_y + 40);
}

/**
 * Add rotation sink
 */
void Motor::AddRotationSink(Component* sink)
{
    if (sink && mRotationSource) {
        mRotationSource->AddSink(sink);
    }
}
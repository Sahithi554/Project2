/**
 * @file Motor.cpp
 * @author Sahithi Nalamalpu
 */

#include "pch.h"
#include "Motor.h"
#include "Machine.h"
#include <b2_contact.h>

/// The center point for drawing the wheel relative to bottom center of box
const auto WheelCenter = wxPoint2DDouble(-12, 24);

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
 * Calculate rotation
 */
double Motor::CalculateRotation(double time)
{
    if (mIsOscillating) {
        return 0.25 * sin(time * mSpeed * M_PI * 2) + 0.25;
    } else {
        return fmod(time * mSpeed, 1.0);
    }
}

/**
 * Begin contact
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
 * Update
 */
void Motor::Update(double time)
{
    if (mState == State::Active) {
        mRotation = CalculateRotation(time);

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
 * Draw
 */
void Motor::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw box
    if (mBox) {
        mBox->Draw(graphics);
    }

    // Calculate positions
    wxPoint2DDouble boxPos = mBox ? mBox->GetPosition() : GetPosition();
    wxPoint2DDouble shaftPos(boxPos.m_x + WheelCenter.m_x, boxPos.m_y + WheelCenter.m_y);

    // Draw animation
    if (!mImages.empty()) {
        graphics->PushState();
        graphics->Translate(shaftPos.m_x, shaftPos.m_y);

        if (mSpeed < 0) {
            graphics->Scale(-1.0, 1.0);
        }

        if (mState == State::Active) {
            // Cycle through frames 1-4
            int frameIdx = (int)(mRotation * 16.0) % 4 + 1;
            if (frameIdx >= 1 && frameIdx <= 4 && frameIdx < (int)mImages.size()) {
                mImages[frameIdx]->DrawPolygon(graphics, 0, 0, 0);
            }
        } else {
            // Draw idle frame
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
    return wxPoint2DDouble(boxPos.m_x + WheelCenter.m_x, boxPos.m_y + WheelCenter.m_y);
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
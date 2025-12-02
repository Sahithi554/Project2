/**
 * @file Motor.cpp
 * @author Sahithi Nalamalpu
 *
 * Defines the Motor component and manages its visual and physics representation.
 */

#include "pch.h"
#include "Motor.h"
#include "Machine.h"
#include <b2_contact.h>

/**
 * Constructor
 * @param parentMachine Pointer to the parent machine
 * @param assetsDir Directory containing motor images
 */
Motor::Motor(Machine* parentMachine, std::wstring assetsDir)
    : Component(parentMachine), mImagesDir(assetsDir)
{
    mBox = std::make_shared<cse335::PhysicsPolygon>();
    mWheel = std::make_shared<cse335::Polygon>();
    mRotationSource = std::make_unique<RotationSource>();

    // Initialize motor box graphic
    mBox->SetImage(assetsDir + L"/motor-box.png");
    mBox->BottomCenteredRectangle(75, 50);

    // Initialize motor wheel graphic
    mWheel->SetImage(assetsDir + L"/wheel.png");
    mWheel->CenteredSquare(45);

    // Load animation frames (idle + 4 active)
    const std::wstring motorFrames[5] = {
        L"/motor-idle.png",
        L"/motor-active-1.png",
        L"/motor-active-2.png",
        L"/motor-active-3.png",
        L"/motor-active-4.png"
    };

    for (int i = 0; i < 5; i++)
    {
        auto frame = std::make_shared<cse335::Polygon>();
        frame->SetImage(assetsDir + motorFrames[i]);
        frame->CenteredSquare(45);
        mImages.push_back(frame);
    }
}

/**
 * Add a component to receive rotation from this motor
 * @param sink Target component
 */
void Motor::AddRotationSink(Component* sink)
{
    if (sink && mRotationSource)
    {
        mRotationSource->AddSink(sink);
    }
}

/**
 * Called when a Box2D contact begins.
 * @param contact Box2D contact pointer
 */
void Motor::BeginContact(b2Contact* contact)
{
    if (mBox && GetBody())
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        if (bodyA == GetBody() || bodyB == GetBody())
        {
            mIsActive = true; // Activate motor on collision
        }
    }
}

/**
 * Update motor rotation based on elapsed time
 * @param deltaTime Time in seconds since last update
 */
void Motor::Update(double deltaTime)
{
    if (mIsActive)
    {
        if (mIsOscillating)
        {
            mRotation = 0.25 * sin(deltaTime * mSpeed * M_PI * 2) + 0.25;
        }
        else
        {
            mRotation = fmod(deltaTime * mSpeed, 1.0);
        }

        if (mRotationSource)
        {
            mRotationSource->SetRotation(mRotation, mSpeed);
        }
    }
}

/**
 * Set the motor's position
 * @param pos New position
 */
void Motor::SetPosition(wxPoint2DDouble pos)
{
    Component::SetPosition(pos);
    if (mBox)
        mBox->SetInitialPosition(pos.m_x, pos.m_y);
}

/**
 * Set the motor's position using coordinates
 * @param x X-coordinate
 * @param y Y-coordinate
 */
void Motor::SetPosition(double x, double y)
{
    SetPosition(wxPoint2DDouble(x, y));
}

/**
 * Draw the motor and its components
 * @param gfx Shared pointer to a wxGraphicsContext
 */
void Motor::Draw(std::shared_ptr<wxGraphicsContext> gfx)
{
    if (mBox)
        mBox->Draw(gfx);

    wxPoint2DDouble boxPos = mBox ? mBox->GetPosition() : GetPosition();
    wxPoint2DDouble centerOffset(boxPos.m_x - 12, boxPos.m_y + 24);

    // Draw animation frames
    if (!mImages.empty())
    {
        gfx->PushState();
        gfx->Translate(centerOffset.m_x, centerOffset.m_y);

        if (mSpeed < 0)
            gfx->Scale(-1.0, 1.0);

        if (mIsActive)
        {
            int frameIndex = (int)(mRotation * 16.0) % 4 + 1;
            if (frameIndex >= 1 && frameIndex <= 4 && frameIndex < (int)mImages.size())
            {
                mImages[frameIndex]->DrawPolygon(gfx, 0, 0, 0);
            }
        }
        else
        {
            mImages[0]->DrawPolygon(gfx, 0, 0, 0);
        }

        gfx->PopState();
    }

    // Draw the wheel if motor is active
    if (mWheel && mIsActive)
    {
        mWheel->DrawPolygon(gfx, centerOffset.m_x, centerOffset.m_y, mRotation);
    }
}

/**
 * Get the position of the motor shaft (where pulleys connect)
 * @return Position of the shaft
 */
wxPoint2DDouble Motor::GetShaftPosition() const
{
    wxPoint2DDouble boxPos = mBox ? mBox->GetPosition() : GetPosition();
    // Shaft is at the top center of the motor box
    return wxPoint2DDouble(boxPos.m_x, boxPos.m_y + 24);
}

void Motor::SetSpread(double spread)
{
    // Store spread for oscillation (if needed)
    // This can be used to control oscillation amplitude
}

void Motor::SetWindupToReleaseRatio(double ratio)
{
    // Store ratio for oscillation timing (if needed)
    // This controls how long windup takes vs release
}

/**
 * @file Conveyor.cpp
 * @author Sahithi Nalamalpu
 * @brief FIXED - Conveyor with PreSolve for proper surface movement
 */

#include "pch.h"
#include "Conveyor.h"
#include "Machine.h"
#include "Consts.h"
#include <b2_body.h>
#include <b2_fixture.h>
#include <b2_contact.h>

/**
 * Constructor
 * @param machine The machine this component belongs to
 */
Conveyor::Conveyor(Machine* machine) : Component(machine)
{
    mPolygon = std::make_shared<cse335::PhysicsPolygon>();
}

/**
 * Set the image for this conveyor
 * @param path Path to the image file
 */
void Conveyor::SetImage(const std::wstring& path)
{
    if (mPolygon)
    {
        mPolygon->SetImage(path);
    }
}

/**
 * Set the position of this conveyor
 * @param x X-coordinate in centimeters
 * @param y Y-coordinate in centimeters
 */
void Conveyor::SetPosition(double x, double y)
{
    Component::SetPosition(wxPoint2DDouble(x, y));
    if (mPolygon)
    {
        mPolygon->SetInitialPosition(x, y);
    }
}

/**
 * Set the size of the conveyor
 * @param width Width in centimeters
 * @param height Height in centimeters
 */
void Conveyor::SetSize(double width, double height)
{
    if (mPolygon)
    {
        // Create a rectangle for the conveyor surface
        mPolygon->BottomCenteredRectangle(width, height);
    }
}

/**
 * Draw the conveyor
 * @param graphics Graphics context to draw on
 */
void Conveyor::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (mPolygon)
    {
        mPolygon->Draw(graphics);
    }
}

/**
 * Set the rotation for this conveyor (from rotation source)
 * @param rotation Rotation in turns (0-1)
 */
void Conveyor::SetRotation(double rotation)
{
    // Conveyors don't rotate visually, they just move objects horizontally
}

/**
 * Rotate this conveyor with both rotation angle and speed
 * @param rotation Rotation in turns (0-1)
 * @param speed Rotation speed in turns per second
 */
void Conveyor::Rotate(double rotation, double speed)
{
    // Store the speed
    // IMPORTANT: Clockwise rotation (positive in our system) should move RIGHT
    // But Box2D uses negative for clockwise, so we negate
    mSpeed = -speed; // Negate because Box2D clockwise is negative
}

/**
 * Update the conveyor based on elapsed time
 * @param time Elapsed time in seconds
 */
void Conveyor::Update(double time)
{
    if (!mPolygon || GetBody() == nullptr || mSpeed == 0)
    {
        return;
    }

    // Calculate horizontal velocity in cm/s
    double horizontalVelocityCm = mSpeed * mSpeedMultiplier;

    // Convert to meters for Box2D
    double velocityMeters = horizontalVelocityCm / Consts::MtoCM;

    // Get the conveyor body
    b2Body* conveyorBody = GetBody();

    // Apply velocity to all bodies in contact with the conveyor
    auto contact = conveyorBody->GetContactList();
    while (contact != nullptr)
    {
        if (contact->contact->IsTouching())
        {
            b2Body* otherBody = contact->other;

            if (otherBody != nullptr && otherBody->GetType() == b2_dynamicBody)
            {
                // Set horizontal velocity (preserve vertical velocity for gravity/jumping)
                b2Vec2 currentVel = otherBody->GetLinearVelocity();
                otherBody->SetLinearVelocity(b2Vec2((float)velocityMeters, currentVel.y));
            }
        }
        contact = contact->next;
    }
}

/**
 * PreSolve - Set tangent speed for conveyor surface
 * This is the KEY to making the conveyor work smoothly
 * @param contact The Box2D contact
 * @param oldManifold The previous contact manifold
 */
void Conveyor::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    if (!mPolygon || GetBody() == nullptr)
    {
        return;
    }

    // Calculate the surface speed
    double horizontalVelocityCm = mSpeed * mSpeedMultiplier;
    double velocityMeters = horizontalVelocityCm / Consts::MtoCM;

    // Set the tangent speed - this makes objects slide along the surface
    // as if the surface itself is moving
    contact->SetTangentSpeed((float)velocityMeters);
}
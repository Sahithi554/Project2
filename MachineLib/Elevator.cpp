/**
 * @file Elevator.cpp
 * @author Sahithi Nalamalpu
 * @brief FIXED - Elevator that moves UP and carries objects
 */

#include "pch.h"
#include "Elevator.h"
#include "Machine.h"
#include "Consts.h"
#include <b2_body.h>
#include <b2_fixture.h>
#include <b2_contact.h>

/**
 * Constructor
 */
Elevator::Elevator(Machine* machine) : Component(machine)
{
    mPolygon = std::make_shared<cse335::PhysicsPolygon>();
    // CRITICAL: Set as kinematic so it doesn't fall
    mPolygon->SetKinematic();
    // Set physics properties for the platform
    mPolygon->SetPhysics(1.0, 0.5, 0.0);
}

/**
 * Set the image for this elevator
 */
void Elevator::SetImage(const std::wstring& path)
{
    if (mPolygon)
    {
        mPolygon->SetImage(path);
    }
}

/**
 * Set the position of this elevator
 */
void Elevator::SetPosition(double x, double y)
{
    Component::SetPosition(wxPoint2DDouble(x, y));
    mInitialPosition = wxPoint2DDouble(x, y);
    if (mPolygon)
    {
        mPolygon->SetInitialPosition(x, y);
    }
}

/**
 * Set the size of the elevator platform
 */
void Elevator::SetSize(double width, double height)
{
    if (mPolygon)
    {
        mPolygon->BottomCenteredRectangle(width, height);
    }
}

/**
 * Draw the elevator
 */
void Elevator::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (mPolygon)
    {
        mPolygon->Draw(graphics);
    }
}

/**
 * Set the rotation for this elevator (from rotation source)
 */
void Elevator::SetRotation(double rotation)
{
    // Not used for elevators - they move linearly
}

/**
 * THE KEY METHOD - This is called by the rotation source
 * @param rotation Rotation in turns (0-1) - not used for elevators
 * @param speed Rotation speed in turns per second
 */
void Elevator::Rotate(double rotation, double speed)
{
    // Store the speed
    mSpeed = speed;
}

/**
 * Update the elevator - moves it vertically
 */
void Elevator::Update(double time)
{
    if (!mPolygon || GetBody() == nullptr)
    {
        return;
    }

    // Calculate vertical velocity
    // POSITIVE speed should move UP (positive Y in Box2D)
    // The multiplier determines how fast the elevator moves per turn/second
    double verticalVelocityCm = mSpeed * mSpeedMultiplier;

    // Convert to meters for Box2D (divide by 100)
    double velocityMeters = verticalVelocityCm / Consts::MtoCM;

    // Get the elevator body
    b2Body* body = GetBody();

    // CRITICAL: Disable gravity for kinematic bodies
    body->SetGravityScale(0.0f);

    // Box2D coordinate system: Positive Y = UP
    // Set the elevator's velocity (positive = upward)
    body->SetLinearVelocity(b2Vec2(0.0f, (float)velocityMeters));
}

/**
 * PreSolve - helps objects ride on the elevator
 * This ensures objects on top move with the elevator
 */
void Elevator::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    if (!mPolygon || GetBody() == nullptr)
    {
        return;
    }

    // Calculate the vertical velocity
    double verticalVelocityCm = mSpeed * mSpeedMultiplier;
    double velocityMeters = verticalVelocityCm / Consts::MtoCM;

    // Get the bodies in contact
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    b2Body* elevatorBody = GetBody();
    b2Body* otherBody = nullptr;

    // Determine which body is the elevator and which is the object on it
    if (bodyA == elevatorBody)
    {
        otherBody = bodyB;
    }
    else if (bodyB == elevatorBody)
    {
        otherBody = bodyA;
    }

    // If we found the other body and it's dynamic (movable)
    if (otherBody != nullptr && otherBody->GetType() == b2_dynamicBody)
    {
        // Give the object the same vertical velocity as the elevator
        b2Vec2 currentVel = otherBody->GetLinearVelocity();
        otherBody->SetLinearVelocity(b2Vec2(currentVel.x, (float)velocityMeters));
    }
}
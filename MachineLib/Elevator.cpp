/**
 * @file Elevator.cpp
 * @author Sahithi Nalamalpu
 * @brief FIXED - Elevator that actually goes up
 */

#include "pch.h"
#include "Elevator.h"
#include "Machine.h"
#include "Consts.h"
#include <b2_body.h>
#include <b2_fixture.h>

#include "b2_contact.h"

/**
 * Constructor
 */
Elevator::Elevator(Machine* machine) : Component(machine)
{
    mPolygon = std::make_shared<cse335::PhysicsPolygon>();
    // CRITICAL: Set as kinematic so it doesn't fall
    mPolygon->SetKinematic();
    // Also set physics properties for the platform
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
    // Not used for elevators
}

/**
 * THE KEY METHOD - This is called by the rotation source
 * @param rotation Rotation in turns (0-1)
 * @param speed Rotation speed in turns per second
 */
void Elevator::Rotate(double rotation, double speed)
{
    // Store the speed
    mSpeed = speed;

    if (!mPolygon || GetBody() == nullptr)
    {
        return;
    }

    // Calculate vertical velocity
    // The multiplier determines how fast the elevator moves per turn/second
    double verticalVelocityCm = speed * mSpeedMultiplier;

    // Convert to meters for Box2D (divide by 100)
    double velocityMeters = verticalVelocityCm / Consts::MtoCM;

    // Get the elevator body
    b2Body* body = GetBody();

    // CRITICAL: Disable gravity for kinematic bodies
    body->SetGravityScale(0.0f);

    // Box2D coordinate system: Positive Y = UP
    // We want positive speed to move UP, so use velocity as-is
    body->SetLinearVelocity(b2Vec2(0.0f, (float)velocityMeters));
}

/**
 * Update the elevator
 */
void Elevator::Update(double time)
{
    // Make sure velocity is still applied (in case it gets reset)
    if (mPolygon && GetBody() != nullptr && mSpeed != 0)
    {
        double verticalVelocityCm = mSpeed * mSpeedMultiplier;
        double velocityMeters = verticalVelocityCm / Consts::MtoCM;

        b2Body* body = GetBody();
        body->SetGravityScale(0.0f);

        // Positive velocity = UP in Box2D
        body->SetLinearVelocity(b2Vec2(0.0f, (float)velocityMeters));

        // Also push objects that are on the elevator
        b2ContactEdge* edge = body->GetContactList();
        while (edge != nullptr)
        {
            b2Contact* contact = edge->contact;
            if (contact->IsTouching())
            {
                b2Body* otherBody = edge->other;

                if (otherBody != nullptr && otherBody->GetType() == b2_dynamicBody)
                {
                    // Give the object on top the same vertical velocity
                    b2Vec2 currentVel = otherBody->GetLinearVelocity();
                    otherBody->SetLinearVelocity(b2Vec2(currentVel.x, (float)velocityMeters));
                }
            }
            edge = edge->next;
        }
    }
}
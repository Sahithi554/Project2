/**
 * @file Elevator.cpp
 * @author Sahithi Nalamalpu
 * @brief Implementation of the Elevator component.
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
 * @param machine The machine this component belongs to
 */
Elevator::Elevator(Machine* machine) : Component(machine)
{
    mPolygon = std::make_shared<cse335::PhysicsPolygon>();
}

/**
 * Set the image for this elevator
 * @param path Path to the image file
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
 * @param x X-coordinate in centimeters
 * @param y Y-coordinate in centimeters
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
 * @param width Width in centimeters
 * @param height Height in centimeters
 */
void Elevator::SetSize(double width, double height)
{
    if (mPolygon)
    {
        // Create a rectangle for the elevator platform
        mPolygon->Rectangle(-width/2, -height/2, width, height);
    }
}

/**
 * Draw the elevator
 * @param graphics Graphics context to draw on
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
 * @param rotation Rotation in turns (0-1)
 */
void Elevator::SetRotation(double rotation)
{
    // Elevators don't rotate visually, they just move objects vertically
    // The rotation value isn't used directly
}

/**
 * Rotate this elevator with both rotation angle and speed
 * @param rotation Rotation in turns (0-1)
 * @param speed Rotation speed in turns per second
 */
void Elevator::Rotate(double rotation, double speed)
{
    mSpeed = speed;
    
    // Apply vertical velocity to objects on the elevator platform
    // Positive speed moves objects up, negative down
    if (mPolygon && GetBody() != nullptr)
    {
        // The elevator itself doesn't move, but we'll apply velocity to contacting bodies
        // This is handled in the Update method
    }
}

/**
 * Update the elevator based on elapsed time
 * @param time Elapsed time in seconds
 */
void Elevator::Update(double time)
{
    // Apply vertical velocity to objects in contact with the elevator
    if (mPolygon && GetBody() != nullptr && mSpeed != 0)
    {
        b2Body* elevatorBody = GetBody();
        b2ContactEdge* edge = elevatorBody->GetContactList();
        
        // Calculate vertical velocity in cm/s, then convert to m/s for Box2D
        double verticalVelocity = mSpeed * mSpeedMultiplier; // cm/s
        double velocityMeters = verticalVelocity / Consts::MtoCM; // m/s (divide by 100 to convert cm to m)
        
        // Apply velocity to all bodies in contact with the elevator
        while (edge != nullptr)
        {
            b2Contact* contact = edge->contact;
            if (contact->IsTouching())
            {
                // Get the other body (the one touching the elevator)
                b2Body* otherBody = edge->other;
                
                if (otherBody != nullptr && otherBody->GetType() == b2_dynamicBody)
                {
                    // Get current velocity
                    b2Vec2 currentVel = otherBody->GetLinearVelocity();
                    
                    // Set vertical velocity (preserve horizontal velocity)
                    otherBody->SetLinearVelocity(b2Vec2(currentVel.x, (float)velocityMeters));
                }
            }
            edge = edge->next;
        }
    }
}


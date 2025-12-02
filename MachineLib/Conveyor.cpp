/**
 * @file Conveyor.cpp
 * @author Sahithi Nalamalpu
 * @brief Implementation of the Conveyor component.
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
        mPolygon->Rectangle(-width/2, -height/2, width, height);
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
    // The rotation value isn't used directly
}

/**
 * Rotate this conveyor with both rotation angle and speed
 * @param rotation Rotation in turns (0-1)
 * @param speed Rotation speed in turns per second
 */
void Conveyor::Rotate(double rotation, double speed)
{
    mSpeed = speed;
    
    // Apply horizontal velocity to objects on the conveyor surface
    // Positive speed moves objects to the right, negative to the left
    if (mPolygon && GetBody() != nullptr)
    {
        // The conveyor itself doesn't move, but we'll apply velocity to contacting bodies
        // This is handled in the Update method
    }
}

/**
 * Update the conveyor based on elapsed time
 * @param time Elapsed time in seconds
 */
void Conveyor::Update(double time)
{
    // Apply horizontal velocity to objects in contact with the conveyor
    if (mPolygon && GetBody() != nullptr && mSpeed != 0)
    {
        b2Body* conveyorBody = GetBody();
        b2ContactEdge* edge = conveyorBody->GetContactList();
        
        // Calculate horizontal velocity in cm/s, then convert to m/s for Box2D
        double horizontalVelocity = mSpeed * mSpeedMultiplier; // cm/s
        double velocityMeters = horizontalVelocity / Consts::MtoCM; // m/s (divide by 100 to convert cm to m)
        
        // Apply velocity to all bodies in contact with the conveyor
        while (edge != nullptr)
        {
            b2Contact* contact = edge->contact;
            if (contact->IsTouching())
            {
                // Get the other body (the one touching the conveyor)
                b2Body* otherBody = edge->other;
                
                if (otherBody != nullptr && otherBody->GetType() == b2_dynamicBody)
                {
                    // Get current velocity
                    b2Vec2 currentVel = otherBody->GetLinearVelocity();
                    
                    // Set horizontal velocity (preserve vertical velocity for gravity)
                    otherBody->SetLinearVelocity(b2Vec2((float)velocityMeters, currentVel.y));
                }
            }
            edge = edge->next;
        }
    }
}


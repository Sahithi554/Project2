/**
 * @file Pulley.cpp
 * @author Sahithi Nalamalpu
 * @brief Implementation of the Pulley component.
 */

#include "pch.h"
#include "Pulley.h"
#include "Machine.h"
#include <cmath>

/**
 * Constructor
 * @param machine The machine this component belongs to
 * @param radius Radius of the pulley in centimeters
 */
Pulley::Pulley(Machine* machine, double radius)
    : Component(machine), mRadius(radius)
{
    mPolygon = std::make_shared<cse335::Polygon>();
    mRotationSource = std::make_unique<RotationSource>();

    // Create a circular pulley shape
    mPolygon->CenteredSquare(radius * 2);
}

/**
 * Set the image for this pulley
 * @param path Path to the image file
 */
void Pulley::SetImage(const std::wstring& path)
{
    if (mPolygon)
    {
        mPolygon->SetImage(path);
    }
}

/**
 * Set the position of this pulley
 * @param x X-coordinate in centimeters
 * @param y Y-coordinate in centimeters
 */
void Pulley::SetPosition(double x, double y)
{
    Component::SetPosition(wxPoint2DDouble(x, y));
}

/**
 * Draw the belts connecting this pulley to driven pulleys
 * @param graphics Graphics context to draw on
 */
void Pulley::DrawBelts(std::shared_ptr<wxGraphicsContext> graphics)
{
    wxPoint2DDouble p1 = GetPosition();
    double r1 = mRadius - 3; // Inset by 3 pixels for better appearance

    // Set up pen for drawing belts (black lines, 2 pixels wide)
    wxGraphicsPen pen = graphics->CreatePen(wxPen(*wxBLACK, 2));
    graphics->SetPen(pen);

    for (auto* driven : mDrivenPulleys)
    {
        if (driven)
        {
            wxPoint2DDouble p2 = driven->GetPosition();
            double r2 = driven->GetRadius() - 3; // Inset by 3 pixels

            // Calculate theta using atan2 (works in all quadrants)
            double theta = atan2(p2.m_y - p1.m_y, p2.m_x - p1.m_x);

            // Calculate distance between pulley centers
            double distance = sqrt((p2.m_x - p1.m_x) * (p2.m_x - p1.m_x) + 
                                   (p2.m_y - p1.m_y) * (p2.m_y - p1.m_y));

            if (distance > 0.001) // Avoid division by zero
            {
                // Calculate phi: sin(phi) = (r2-r1)/|p2-p1|
                double sinPhi = (r2 - r1) / distance;
                // Clamp to valid range for asin [-1, 1]
                if (sinPhi > 1.0) sinPhi = 1.0;
                if (sinPhi < -1.0) sinPhi = -1.0;
                double phi = asin(sinPhi);

                // Draw first belt line: beta = theta + phi + pi/2
                double beta1 = theta + phi + M_PI / 2.0;
                wxPoint2DDouble point1_1(p1.m_x + r1 * cos(beta1), p1.m_y + r1 * sin(beta1));
                wxPoint2DDouble point1_2(p2.m_x + r2 * cos(beta1), p2.m_y + r2 * sin(beta1));
                graphics->StrokeLine(point1_1.m_x, point1_1.m_y, point1_2.m_x, point1_2.m_y);

                // Draw second belt line: beta = theta - phi - pi/2
                double beta2 = theta - phi - M_PI / 2.0;
                wxPoint2DDouble point2_1(p1.m_x + r1 * cos(beta2), p1.m_y + r1 * sin(beta2));
                wxPoint2DDouble point2_2(p2.m_x + r2 * cos(beta2), p2.m_y + r2 * sin(beta2));
                graphics->StrokeLine(point2_1.m_x, point2_1.m_y, point2_2.m_x, point2_2.m_y);
            }
        }
    }
}

/**
 * Draw the pulley
 * @param graphics Graphics context to draw on
 */
void Pulley::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (mPolygon)
    {
        wxPoint2DDouble pos = GetPosition();
        mPolygon->DrawPolygon(graphics, pos.m_x, pos.m_y, mRotation);
    }
}

/**
 * Set the rotation for this pulley (called by rotation source)
 * @param rotation Rotation in turns (0-1)
 */
void Pulley::SetRotation(double rotation)
{
    mRotation = rotation;

    // Propagate rotation to all driven pulleys
    for (auto* driven : mDrivenPulleys)
    {
        if (driven)
        {
            // Calculate speed ratio based on radius
            // Larger driving pulley = faster driven pulley
            // Formula: driven_speed = driving_speed * (driving_radius / driven_radius)
            double speedRatio = mRadius / driven->GetRadius();
            double drivenSpeed = mSpeed * speedRatio;

            driven->SetSpeed(drivenSpeed);

            // Set the rotation of the driven pulley
            // The rotation angle is also affected by the speed ratio
            double drivenRotation = fmod(rotation * speedRatio, 1.0);
            driven->SetRotation(drivenRotation);
        }
    }

    // Update our rotation source so it can drive other components
    if (mRotationSource)
    {
        mRotationSource->SetRotation(mRotation, mSpeed);
    }
}

/**
 * Rotate this pulley with both rotation angle and speed (called by rotation source)
 * @param rotation Rotation in turns (0-1)
 * @param speed Rotation speed in turns per second
 */
void Pulley::Rotate(double rotation, double speed)
{
    mRotation = rotation;
    mSpeed = speed;

    // Propagate rotation and speed to all driven pulleys via belts
    for (auto* driven : mDrivenPulleys)
    {
        if (driven)
        {
            // Speed ratio: N/M where N is driving radius, M is driven radius
            // Formula: driven_speed = driving_speed * (driving_radius / driven_radius)
            double speedRatio = mRadius / driven->GetRadius();
            double drivenSpeed = mSpeed * speedRatio;

            // Rotation calculation: driven_rotation = driving_rotation * (driving_radius / driven_radius)
            double drivenRotation = fmod(rotation * speedRatio, 1.0);
            driven->Rotate(drivenRotation, drivenSpeed);
        }
    }

    // Update our rotation source so it can drive other components (shapes, conveyors, elevators)
    if (mRotationSource)
    {
        mRotationSource->SetRotation(mRotation, mSpeed);
    }
}

/**
 * Update the pulley based on elapsed time
 * @param time Elapsed time in seconds
 */
void Pulley::Update(double time)
{
    // If we have a speed, update our rotation
    if (mSpeed != 0)
    {
        // Rotation is handled by SetRotation from the driving source
        // This is here for future expansion if needed
    }
}

/**
 * Drive another pulley with this pulley
 * @param driven The pulley to be driven by this one
 */
void Pulley::Drive(Pulley* driven)
{
    if (driven)
    {
        mDrivenPulleys.push_back(driven);

        // Connect our rotation source to the driven pulley
        if (mRotationSource)
        {
            mRotationSource->AddSink(driven);
        }
    }
}
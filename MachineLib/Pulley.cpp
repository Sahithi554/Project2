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
    mPolygon         = std::shared_ptr<cse335::Polygon>(new cse335::Polygon());
    mRotationSource  = std::unique_ptr<RotationSource>(new RotationSource());

    // Represent pulley face with a square of diameter 2R (simple visual stand-in)
    mPolygon->CenteredSquare(mRadius * 2.0);
}

/**
 * Set the image for this pulley
 * @param path Path to the image file
 */
void Pulley::SetImage(const std::wstring& path)
{
    if (!mPolygon) return;
    mPolygon->SetImage(path);
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
void Pulley::DrawBelts(std::shared_ptr<wxGraphicsContext> g)
{
    const auto origin  = GetPosition();
    const double rBase = mRadius - 3.0;

    wxGraphicsPen beltPen = g->CreatePen(wxPen(*wxBLACK, 2));
    g->SetPen(beltPen);

    // Iterate through all driven pulleys using algorithmic style
    std::for_each(mDrivenPulleys.begin(), mDrivenPulleys.end(),
        [&](Pulley* driven)
        {
            if (!driven) return;

            const auto dest = driven->GetPosition();
            const double rOut = driven->GetRadius() - 3.0;

            const double dx = dest.m_x - origin.m_x;
            const double dy = dest.m_y - origin.m_y;

            const double dist = std::hypot(dx, dy);
            if (dist < 0.001) return;

            const double theta = std::atan2(dy, dx);

            // Compute wrapped phi, clamped to asin domain
            double sinPhi = (rOut - rBase) / dist;
            sinPhi = std::clamp(sinPhi, -1.0, 1.0);
            const double phi = std::asin(sinPhi);

            auto drawBeltLine = [&](double baseAngle)
            {
                const double a = theta + baseAngle;
                const double x1 = origin.m_x + rBase * std::cos(a);
                const double y1 = origin.m_y + rBase * std::sin(a);
                const double x2 = dest.m_x + rOut  * std::cos(a);
                const double y2 = dest.m_y + rOut  * std::sin(a);

                g->StrokeLine(x1, y1, x2, y2);
            };

            // Two support belts
            drawBeltLine(phi + M_PI * 0.5);
            drawBeltLine(-phi - M_PI * 0.5);
        }
    );

}

/**
 * Draw the pulley
 * @param graphics Graphics context to draw on
 */
void Pulley::Draw(std::shared_ptr<wxGraphicsContext> g)
{
    if (!mPolygon) return;

    const auto pos = GetPosition();
    mPolygon->DrawPolygon(g, pos.m_x, pos.m_y, mRotation);

}

/**
 * Set the rotation for this pulley (called by rotation source)
 * @param rotation Rotation in turns (0-1)
 */
void Pulley::SetRotation(double angle)
{
    mRotation = angle;

    const auto sendDownstream = [&](Pulley* d)
    {
        if (!d) return;

        const double ratio = mRadius / d->GetRadius();
        const double newSpeed = mSpeed * ratio;
        const double newAngle = std::fmod(angle * ratio, 1.0);

        d->SetSpeed(newSpeed);
        d->SetRotation(newAngle);
    };

    std::for_each(mDrivenPulleys.begin(), mDrivenPulleys.end(), sendDownstream);

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
void Pulley::Rotate(double angle, double speed)
{
    mRotation = angle;
    mSpeed    = speed;

    const auto push = [&](Pulley* d)
    {
        if (!d) return;

        const double ratio = mRadius / d->GetRadius();
        const double childSpeed = speed * ratio;
        const double childAngle = std::fmod(angle * ratio, 1.0);

        d->Rotate(childAngle, childSpeed);
    };

    std::for_each(mDrivenPulleys.begin(), mDrivenPulleys.end(), push);

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
    //if (mSpeed != 0)
    //{
        // Rotation is handled by SetRotation from the driving source
        // This is here for future expansion if needed
    //}
}

/**
 * Drive another pulley with this pulley
 * @param driven The pulley to be driven by this one
 */
void Pulley::Drive(Pulley* other)
{
    if (!other) return;

    mDrivenPulleys.emplace_back(other);

    if (mRotationSource)
    {
        mRotationSource->AddSink(other);
    }
}
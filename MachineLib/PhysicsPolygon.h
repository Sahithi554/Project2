/**
 * @file PhysicsPolygon.h
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * A Polygon object that can install itself into the
 * physics system.
 *
 * Note: Dimensions for the PhysicsPolygon object are in
 * centimeters.
 *
 * Version history:
 * 1.00 Initial version for project 2
 * 1.01 Revised to work prior to physics installation
 * 1.02 Disabled the ability to use DrawPolygon directly
 * 1.03 Added SetPosition function
 */

#pragma once

#include "Polygon.h"
#include <b2_math.h>
#include <b2_body.h>

class b2Body;
class b2World;

namespace cse335
{
/**
 * A Polygon that integrates with Box2D physics.
 */
class PhysicsPolygon : public Polygon
{
private:
    using Polygon::DrawPolygon;

    b2Body* mBody = nullptr;                   ///< Physics body (null until installed)
    double mInitialRotation = 0;               ///< Initial rotation in radians
    wxPoint2DDouble mInitialPosition;          ///< Initial position in machine
    b2BodyType mType = b2_staticBody;          ///< Body type: static, dynamic, or kinematic
    double mDensity = 1.0;                     ///< Density (kg/m^2)
    double mFriction = 0.5;                    ///< Friction coefficient [0,1]
    double mRestitution = 0.5;                 ///< Elasticity [0,1]

public:
    PhysicsPolygon();

    PhysicsPolygon(const PhysicsPolygon&) = delete; ///< Disable copy constructor
    void operator=(const PhysicsPolygon&) = delete; ///< Disable assignment

    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics);

    void SetInitialPosition(double x, double y) { mInitialPosition = wxPoint2DDouble(x, y); }
    void SetInitialRotation(double r) { mInitialRotation = r * M_PI * 2; }

    void SetRotation(double rotation);
    void SetAngularVelocity(double speed);
    virtual double GetRotation();

    void SetPosition(double x, double y);
    wxPoint2DDouble GetPosition();

    /**
     * Install this polygon into the physics world.
     * @param world Box2D world
     */
    void InstallPhysics(std::shared_ptr<b2World> world);

    void SetDynamic();
    void SetKinematic();
    void SetPhysics(double density = 1.0, double friction = 0.5, double restitution = 0.5);

    /**
     * Get the physics body. Only valid after InstallPhysics.
     * @return Pointer to Box2D body
     */
    b2Body* GetBody() { return mBody; }
};

} // namespace cse335

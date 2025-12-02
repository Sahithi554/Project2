/**
 * @file PhysicsPolygon.cpp
 * @author Sahithi Nalamalpu
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * @brief Implementation of the PhysicsPolygon class.
 */

#include "pch.h"
#include "PhysicsPolygon.h"
#include "Consts.h"
#include <b2_polygon_shape.h>
#include <b2_circle_shape.h>
#include <b2_fixture.h>
#include <b2_world.h>

namespace cse335
{

/**
 * @brief Default constructor for PhysicsPolygon.
 */
PhysicsPolygon::PhysicsPolygon() : Polygon()
{
}

/**
 * @brief Draws the polygon in the given graphics context.
 * @param graphics Shared pointer to the graphics context.
 */
void PhysicsPolygon::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    auto position = GetPosition();
    auto rotation = GetRotation();

    DrawPolygon(graphics, position.m_x, position.m_y, rotation);
}

/**
 * @brief Installs the polygon into the Box2D physics world.
 * @param world Shared pointer to the Box2D world.
 */
void PhysicsPolygon::InstallPhysics(std::shared_ptr<b2World> world)
{
    b2BodyDef bodyDefinition;
    bodyDefinition.type = mType;
    mBody = world->CreateBody(&bodyDefinition);

    b2FixtureDef fixtureDef;
    b2CircleShape circle;
    b2PolygonShape poly;

    if (IsCircle())
    {
        circle.m_radius = Radius() / Consts::MtoCM - 0.005;
        fixtureDef.shape = &circle;
    }
    else
    {
        auto boundingBox = Polygon::BoundingBox();
        auto size = wxPoint2DDouble(boundingBox.m_width/2, boundingBox.m_height/2);
        auto center = boundingBox.GetCentre();
        auto scale = (size - wxPoint2DDouble(0.95, 0.95)) / size;

        std::vector<b2Vec2> vertices;
        for (auto v : *this)
        {
            auto scaled = ((v - center) * scale) + center;
            vertices.push_back(b2Vec2(scaled.m_x / Consts::MtoCM, scaled.m_y / Consts::MtoCM));
        }

        poly.Set(&vertices[0], vertices.size());
        fixtureDef.shape = &poly;
    }

    fixtureDef.density = mDensity;
    fixtureDef.friction = mFriction;
    fixtureDef.restitution = mRestitution;

    mBody->CreateFixture(&fixtureDef);
    mBody->SetTransform(b2Vec2(mInitialPosition.m_x / Consts::MtoCM,
                               mInitialPosition.m_y / Consts::MtoCM), mInitialRotation);
}

/**
 * @brief Set the polygon's position in the machine.
 * @param x X-coordinate in centimeters
 * @param y Y-coordinate in centimeters
 */
void PhysicsPolygon::SetPosition(double x, double y)
{
    if (mBody != nullptr)
    {
        auto position = b2Vec2(x / Consts::MtoCM, y / Consts::MtoCM);
        mBody->SetTransform(position, mBody->GetAngle());
        mBody->SetGravityScale(0);
    }
    else
    {
        SetInitialPosition(x, y);
    }
}

/**
 * @brief Get the polygon's current position.
 * @return Position in centimeters
 */
wxPoint2DDouble PhysicsPolygon::GetPosition()
{
    if (mBody != nullptr)
    {
        auto position = mBody->GetPosition();
        return wxPoint2DDouble(position.x * Consts::MtoCM, position.y * Consts::MtoCM);
    }
    else
    {
        return mInitialPosition;
    }
}

/**
 * @brief Set the polygon's rotation.
 * @param rotation Rotation in turns (0-1)
 */
void PhysicsPolygon::SetRotation(double rotation)
{
    if (mBody != nullptr)
    {
        mBody->SetTransform(mBody->GetPosition(), rotation * M_PI * 2);
        mBody->SetGravityScale(0);
    }
    else
    {
        SetInitialRotation(rotation);
    }
}

/**
 * @brief Get the polygon's current rotation.
 * @return Rotation in turns (0-1)
 */
double PhysicsPolygon::GetRotation()
{
    if (mBody != nullptr)
    {
        return mBody->GetAngle() / (M_PI * 2);
    }
    else
    {
        return mInitialRotation / (M_PI * 2);
    }
}

/**
 * @brief Set the polygon as a dynamic body.
 */
void PhysicsPolygon::SetDynamic()
{
    mType = b2_dynamicBody;
}

/**
 * @brief Set the polygon as a kinematic body.
 */
void PhysicsPolygon::SetKinematic()
{
    mType = b2_kinematicBody;
}

/**
 * @brief Set the physics properties before installation.
 * @param density Density in kg/m²
 * @param friction Friction coefficient [0,1]
 * @param restitution Restitution coefficient [0,1]
 */
void PhysicsPolygon::SetPhysics(double density, double friction, double restitution)
{
    mDensity = density;
    mFriction = friction;
    mRestitution = restitution;
}

/**
 * @brief Set angular velocity of the polygon.
 * @param speed Rotation speed in turns per second
 */
void PhysicsPolygon::SetAngularVelocity(double speed)
{
    if (mBody != nullptr)
    {
        mBody->SetAngularVelocity(speed * M_PI * 2);
    }
}

} // namespace cse335

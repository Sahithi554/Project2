/**
 * @file Shape.cpp
 * @author Sahithi Nalamalpu
 */

#include "pch.h"
#include "Shape.h"
#include "Machine.h"
#include "Consts.h"

// Static Behavior
class Shape::StaticBehavior : public PhysicsBehavior {
public:
    void Apply(cse335::PhysicsPolygon* polygon) override {
        // Static is default
    }
    void HandleRotation(cse335::PhysicsPolygon* polygon, double rotation) override {
        // Static objects don't rotate
    }
    void HandleSpeed(cse335::PhysicsPolygon* polygon, double speed) override {
        // Static objects don't have speed
    }
};

// Dynamic Behavior
class Shape::DynamicBehavior : public PhysicsBehavior {
public:
    void Apply(cse335::PhysicsPolygon* polygon) override {
        polygon->SetDynamic();
    }
    void HandleRotation(cse335::PhysicsPolygon* polygon, double rotation) override {
        if (polygon && polygon->GetBody()) {
            polygon->SetRotation(rotation);
        }
    }
    void HandleSpeed(cse335::PhysicsPolygon* polygon, double speed) override {
        if (polygon && polygon->GetBody()) {
            polygon->SetAngularVelocity(speed);
        }
    }
};

// Kinematic Behavior
class Shape::KinematicBehavior : public PhysicsBehavior {
public:
    void Apply(cse335::PhysicsPolygon* polygon) override {
        polygon->SetKinematic();
    }
    void HandleRotation(cse335::PhysicsPolygon* polygon, double rotation) override {
        if (polygon && polygon->GetBody()) {
            polygon->SetRotation(rotation);
        }
    }
    void HandleSpeed(cse335::PhysicsPolygon* polygon, double speed) override {
        if (polygon && polygon->GetBody()) {
            // For kinematic shapes, set angular velocity
            // Speed is in turns per second, Box2D needs radians per second
            // Clockwise is negative in Box2D
            polygon->SetAngularVelocity(-speed * 2.0 * M_PI);
        }
    }
};

/**
 * Constructor
 */
Shape::Shape(Machine* machine) : Component(machine)
{
    mPolygon = std::make_shared<cse335::PhysicsPolygon>();
    mRotationSource = std::make_unique<RotationSource>();
    mBehavior = std::make_unique<StaticBehavior>();
}



void Shape::Circle(double radius)
{
    if (mPolygon) {
        mPolygon->Circle(radius);
    }
}

void Shape::BottomCenteredRectangle(double width, double height)
{
    if (mPolygon) {
        mPolygon->BottomCenteredRectangle(width, height);
    }
}

void Shape::Rectangle(double x, double y, double width, double height)
{
    if (mPolygon) {
        mPolygon->Rectangle(x, y, width, height);
    }
}


void Shape::AddPoint(double x, double y)
{
    if (mPolygon) {
        mPolygon->AddPoint(x, y);
    }
}

void Shape::AddPoint(wxPoint2DDouble point)
{
    AddPoint(point.m_x, point.m_y);
}

void Shape::SetImage(const std::wstring& path)
{
    if (mPolygon) {
        mPolygon->SetImage(path);
    }
}

void Shape::SetColor(wxColour color)
{
    if (mPolygon) {
        mPolygon->SetColor(color);
    }
}

void Shape::SetDynamic()
{
    mBehavior = std::make_unique<DynamicBehavior>();
    if (mPolygon) {
        mBehavior->Apply(mPolygon.get());
    }
}

void Shape::SetKinematic()
{
    mBehavior = std::make_unique<KinematicBehavior>();
    if (mPolygon) {
        mBehavior->Apply(mPolygon.get());
    }
}

void Shape::SetInitialPosition(double x, double y)
{
    if (mPolygon) {
        mPolygon->SetInitialPosition(x, y);
    }
}

void Shape::SetInitialPosition(wxPoint2DDouble pos)
{
    SetInitialPosition(pos.m_x, pos.m_y);
}

void Shape::SetInitialRotation(double rotation)
{
    if (mPolygon) {
        mPolygon->SetInitialRotation(rotation);
    }
}

void Shape::SetPhysics(double density, double friction, double restitution)
{
    if (mPolygon) {
        mPolygon->SetPhysics(density, friction, restitution);
    }
}

void Shape::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (mPolygon) {
        mPolygon->Draw(graphics);
    }
}

void Shape::SetRotation(double rotation)
{
    mRotation = rotation;
    if (mBehavior && mPolygon && GetBody()) {
        mBehavior->HandleRotation(mPolygon.get(), rotation);
    }
}

void Shape::Rotate(double rotation, double speed)
{
    mRotation = rotation;
    if (mBehavior && mPolygon && GetBody()) {
        mBehavior->HandleSpeed(mPolygon.get(), speed);
    }
}
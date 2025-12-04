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
    auto init = [&]()
    {
        mPolygon = std::shared_ptr<cse335::PhysicsPolygon>(new cse335::PhysicsPolygon());
        mRotationSource = std::unique_ptr<RotationSource>(new RotationSource());
        mBehavior = std::unique_ptr<StaticBehavior>(new StaticBehavior());
    };

    init();
}



void Shape::Circle(double radius)
{
    auto* poly = mPolygon.get();
    poly ? poly->Circle(radius) : void();
}

void Shape::BottomCenteredRectangle(double width, double height)
{
    [&]() {
        if (auto* p = mPolygon.get()) {
            p->BottomCenteredRectangle(width, height);
        }
    }();
}

void Shape::Rectangle(double x, double y, double width, double height)
{
    switch (mPolygon ? 1 : 0)
    {
    case 1:
        mPolygon->Rectangle(x, y, width, height);
        break;
    default:
        break;
    }

}


void Shape::AddPoint(double x, double y)
{
    auto poly = mPolygon;
    if (!poly) return;

    const auto px = x;
    const auto py = y;
    poly->AddPoint(px, py);
}

void Shape::AddPoint(wxPoint2DDouble point)
{
    const auto forward = [&](double a, double b)
    {
        AddPoint(a, b);
    };

    forward(point.m_x, point.m_y);
}

void Shape::SetImage(const std::wstring& path)
{
    if (auto poly = mPolygon.get(); poly)
        poly->SetImage(path);
}

void Shape::SetColor(wxColour color)
{
    if (auto raw = mPolygon.get(); raw)
        raw->SetColor(color);
}

void Shape::SetDynamic()
{
    auto newBehavior = std::make_unique<DynamicBehavior>();
    auto polyPtr = mPolygon.get();

    if (polyPtr)
    {
        newBehavior->Apply(polyPtr);
    }

    mBehavior = std::move(newBehavior);
}

void Shape::SetKinematic()
{
    auto* poly = mPolygon.get();

    switch (poly ? 1 : 0)
    {
    case 1:
        {
            // Kinematic mode uses the same StaticBehavior instance
            mBehavior->Apply(poly);
            break;
        }
    default:
        break;
    }
}

void Shape::SetInitialPosition(double x, double y)
{
    if (auto poly = mPolygon.get(); poly)
    {
        poly->SetInitialPosition(x, y);
    }
}

inline void Shape::SetInitialPosition(wxPoint2DDouble pos)
{
    SetInitialPosition(pos.m_x, pos.m_y);
}

void Shape::SetInitialRotation(double rotation)
{
    if (auto poly = mPolygon.get(); poly)
    {
        poly->SetInitialRotation(rotation);
    }
}

void Shape::SetPhysics(double density, double friction, double restitution)
{
    if (!mPolygon) return;
    mPolygon->SetPhysics(density, friction, restitution);
}


void Shape::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (auto poly =mPolygon.get(); poly)
    {
        poly->Draw(graphics);
    }
}

void Shape::SetRotation(double rotation)
{
    mRotation = rotation;

    if (!mBehavior || !mPolygon || !GetBody()) return;
    mBehavior->HandleRotation(mPolygon.get(), rotation);
}

void Shape::Rotate(double rotation, double speed)
{
    mRotation = rotation;

    if (auto poly = mPolygon.get(); mBehavior && poly && GetBody())
    {
        mBehavior->HandleSpeed(poly, speed);
    }
}
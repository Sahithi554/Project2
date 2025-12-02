/**
* @file Shape.cpp
 * @author Sahithi Nalamalpu
 * @version 1.0
 *
 * Implements the Shape class that represents a drawable geometric component.
 */

#include "pch.h"
#include "Shape.h"
#include "Machine.h"
#include "Consts.h"

/**
 * Constructor
 * @param machine The machine this component belongs to
 */
Shape::Shape(Machine* machine) : Component(machine)
{
    mPolygon = std::make_shared<cse335::PhysicsPolygon>();
    mRotationSource = std::make_unique<RotationSource>();
}


void Shape::Rectangle(double x, double y, double width, double height)
{
    mPolygon->Rectangle(x, y, width, height);
}


void Shape::SetImage(const std::wstring& path)
{
    mPolygon->SetImage(path);
}


void Shape::SetColor(wxColour color)
{
    mPolygon->SetColor(color);
}


void Shape::SetDynamic()
{
    mIsDynamic = true;
    mIsKinematic = false;
    mPolygon->SetDynamic();
}


void Shape::SetKinematic()
{
    mIsKinematic = true;
    mIsDynamic = false;
    mPolygon->SetKinematic();
}


void Shape::AddPoint(wxPoint2DDouble point)
{
    mPolygon->AddPoint(point.m_x, point.m_y);
}


void Shape::AddPoint(double x, double y)
{
    mPolygon->AddPoint(x, y);
}


void Shape::SetInitialPosition(wxPoint2DDouble pos)
{
    mPolygon->SetInitialPosition(pos.m_x, pos.m_y);
}


void Shape::SetInitialPosition(double x, double y)
{
    mPolygon->SetInitialPosition(x, y);
}

void Shape::SetInitialRotation(double rotation)
{
    if (mPolygon)
    {
        mPolygon->SetInitialRotation(rotation);
    }
}

void Shape::SetPhysics(double density, double friction, double restitution)
{
    if (mPolygon)
    {
        mPolygon->SetPhysics(density, friction, restitution);
    }
}

void Shape::BottomCenteredRectangle(double width, double height)
{
    if (mPolygon)
    {
        mPolygon->BottomCenteredRectangle(width, height);
    }
}

void Shape::Circle(double radius)
{
    mPolygon->Circle(radius);
}


void Shape::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (mPolygon != nullptr)
    {
        mPolygon->Draw(graphics);
    }
}



void Shape::SetRotation(double rotation)
{
    mRotation = rotation;

    if (mPolygon != nullptr && GetBody() != nullptr)
    {
        mPolygon->SetRotation(rotation);
    }
}

void Shape::Rotate(double rotation, double speed)
{
    mRotation = rotation;

    // For shapes, use SetAngularVelocity with speed instead of SetRotation
    // This allows the physics system to handle the rotation properly
    if (mPolygon != nullptr && GetBody() != nullptr)
    {
        mPolygon->SetAngularVelocity(speed);
    }
}
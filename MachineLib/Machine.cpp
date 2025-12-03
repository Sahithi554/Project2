/**
* @file Machine.cpp
 * @author Sahithi Nalamalpu
 * Refactored to use cleaner architecture
 */

#include "pch.h"
#include "Machine.h"
#include "Component.h"
#include "ContactListener.h"
#include "Consts.h"
#include "Pulley.h"
#include <b2_world.h>

#include "Conveyor.h"
#include "Elevator.h"
#include "Motor.h"
#include "Shape.h"

/**
 * Constructor
 */
Machine::Machine(int machineId) : mMachineNum(machineId)
{
    b2Vec2 gravityVector(0.0f, -9.8f);
    mWorld = std::make_shared<b2World>(gravityVector);

    mContactListener = std::make_shared<ContactListener>();
    mWorld->SetContactListener(mContactListener.get());
}

/**
 * Add a component
 */
void Machine::AddComponent(std::shared_ptr<Component> comp)
{
    if (comp)
    {
        mComponents.push_back(comp);
    }
}

/**
 * Set frame
 */
void Machine::SetFrame(int frameIndex)
{
    mCurrFrame = frameIndex;
    double simTime = frameIndex / mFrameRate;
    Update(simTime);
}

/**
 * Draw the machine
 */
void Machine::Draw(std::shared_ptr<wxGraphicsContext> gfx)
{
    gfx->PushState();
    gfx->Translate(mLocation.x, mLocation.y);
    gfx->Scale(1.0, -1.0);

    // First pass: Draw belts
    for (auto& comp : mComponents)
    {
        if (comp)
        {
            auto* pulley = dynamic_cast<Pulley*>(comp.get());
            if (pulley != nullptr)
            {
                pulley->DrawBelts(gfx);
            }
        }
    }

    // Second pass: Draw components
    for (auto& comp : mComponents)
    {
        if (comp)
        {
            comp->Draw(gfx);
        }
    }

    gfx->PopState();
}

/**
 * Update the machine
 */
void Machine::Update(double simTime)
{
    if (mWorld)
    {
        double step = 1.0 / mFrameRate;
        int32 velIters = 6;
        int32 posIters = 2;
        mWorld->Step(step, velIters, posIters);
    }

    for (auto& comp : mComponents)
    {
        if (comp)
        {
            comp->Update(simTime);
        }
    }
}

/**
 * Reset the machine to initial state
 */
void Machine::Reset()
{
    // Create new world
    b2Vec2 gravityVector(0.0f, -9.8f);
    mWorld = std::make_shared<b2World>(gravityVector);

    // Create new contact listener
    mContactListener = std::make_shared<ContactListener>();
    mWorld->SetContactListener(mContactListener.get());

    // Reinstall all components into physics system
    for (auto& comp : mComponents)
    {
        if (comp)
        {
            // Check if component has a polygon (Shape, Motor, Conveyor, Elevator)
            auto shape = dynamic_cast<Shape*>(comp.get());
            if (shape && shape->GetPolygon())
            {
                shape->GetPolygon()->InstallPhysics(mWorld);
                shape->SetBody(shape->GetPolygon()->GetBody());
            }

            auto motor = dynamic_cast<Motor*>(comp.get());
            if (motor && motor->GetBox())
            {
                motor->GetBox()->InstallPhysics(mWorld);
                motor->SetBody(motor->GetBox()->GetBody());
                mContactListener->Add(motor->GetBody(), motor);
            }

            auto conveyor = dynamic_cast<Conveyor*>(comp.get());
            if (conveyor && conveyor->GetPolygon())
            {
                conveyor->GetPolygon()->InstallPhysics(mWorld);
                conveyor->SetBody(conveyor->GetPolygon()->GetBody());
            }

            auto elevator = dynamic_cast<Elevator*>(comp.get());
            if (elevator && elevator->GetPolygon())
            {
                elevator->GetPolygon()->InstallPhysics(mWorld);
                elevator->SetBody(elevator->GetPolygon()->GetBody());
            }
        }
    }
}


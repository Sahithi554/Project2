/**
 * @file Machine.cpp
 * @author Sahithi Nalamalpu
 *
 * Defines the Machine class, which manages a collection of components
 * and maintains a Box2D physics world for simulation updates.
 */

#include "pch.h"
#include "Machine.h"
#include "Component.h"
#include "ContactListener.h"
#include "Consts.h"
#include "Pulley.h"
#include <b2_world.h>

/**
 * Constructor: initialize a new Machine instance.
 * Sets up physics world and contact listener.
 *
 * @param machineId Identifier for this machine instance.
 */
Machine::Machine(int machineId) : mMachineNum(machineId)
{
    // Create Box2D physics world with downward gravity.
    b2Vec2 gravityVector(0.0f, -9.8f);
    mWorld = std::make_shared<b2World>(gravityVector);

    // Attach a contact listener for collision events.
    mContactListener = std::make_shared<ContactListener>();
    mWorld->SetContactListener(mContactListener.get());
}

/**
 * Add a component to the machine.
 * Stores the component but does not take exclusive ownership.
 *
 * @param comp Shared pointer to the component to add.
 */
void Machine::AddComponent(std::shared_ptr<Component> comp)
{
    if (comp)
    {
        mComponents.push_back(comp);
    }
}

/**
 * Update the machine to a specific frame.
 * Converts frame to simulation time and updates all components.
 *
 * @param frameIndex The frame number to advance to.
 */
void Machine::SetFrame(int frameIndex)
{
    mCurrFrame = frameIndex;

    // Translate frame index into simulation time.
    double simTime = frameIndex / mFrameRate;
    Update(simTime);
}

/**
 * Render the machine at its current location.
 *
 * @param gfx Shared pointer to a wxGraphicsContext for drawing.
 */
void Machine::Draw(std::shared_ptr<wxGraphicsContext> gfx)
{
    gfx->PushState();

    // Move origin to machine position.
    gfx->Translate(mLocation.x, mLocation.y);

    // Flip Y axis to match Box2D's coordinate system.
    gfx->Scale(1.0, -1.0);

    // First pass: Draw belts (must be drawn before pulleys)
    for (auto& comp : mComponents)
    {
        if (comp)
        {
            // Check if this component is a Pulley and draw its belts
            auto* pulley = dynamic_cast<Pulley*>(comp.get());
            if (pulley != nullptr)
            {
                pulley->DrawBelts(gfx);
            }
        }
    }

    // Second pass: Render each component (pulleys drawn on top of belts)
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
 * Advance the physics simulation and update components.
 *
 * @param simTime Time in seconds since frame 0.
 */
void Machine::Update(double simTime)
{
    if (mWorld)
    {
        double step = 1.0 / mFrameRate;

        // Typical Box2D iteration counts for velocity and position.
        int32 velIters = 6;
        int32 posIters = 2;

        // Advance the physics world one time step.
        mWorld->Step(step, velIters, posIters);
    }

    // Update all attached components.
    for (auto& comp : mComponents)
    {
        if (comp)
        {
            comp->Update(simTime);
        }
    }
}

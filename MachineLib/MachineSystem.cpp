/**
 * @file MachineSystem.cpp
 * @author Sahithi Nalamalpu
 *
 * Updated to use Machine1Factory
 */

#include "pch.h"
#include "IMachineSystem.h"
#include "MachineSystem.h"
#include "Machine.h"
#include "Machine1Factory.h"
#include "Machine2Factory.h"
#include "Shape.h"
#include "Motor.h"
#include "ContactListener.h"
#include "Pulley.h"
#include "Consts.h"
#include <b2_world.h>
#include <b2_body.h>

/**
 * Construct a MachineSystem.
 * @param resourcesDir Path to the resources folder (images, etc.)
 */
MachineSystem::MachineSystem(std::wstring resourcesDir) : mResourcesDir(resourcesDir)
{
    // Prepare the first machine immediately so the UI can query it.
    ChooseMachine(1);
}

/**
 * Create or return an existing Machine for the requested index.
 * @param machineNum Index of the machine to get or create.
 * @return Shared pointer to the Machine instance.
 */
std::shared_ptr<Machine> MachineSystem::GetCreateMachine(int machineNum)
{
    // Make sure our vector can hold the requested index.
    if (mMachines.size() <= static_cast<size_t>(machineNum))
    {
        mMachines.resize(machineNum + 1);
    }

    // If not present, construct and configure the machine.
    if (mMachines[machineNum] == nullptr)
    {
        std::shared_ptr<Machine> machine;

        // Use the appropriate factory based on machine number
        if (machineNum == 1)
        {
            Machine1Factory factory(mResourcesDir);
            machine = factory.Create(machineNum);
        }
        else if (machineNum == 2)
        {
            Machine2Factory factory(mResourcesDir);
            machine = factory.Create(machineNum);
        }
        else
        {
            // Default: empty machine
            machine = std::make_shared<Machine>(machineNum);
        }

        machine->SetFrameRate(mFrameRate);

        machine->Reset();


        mMachines[machineNum] = machine;
    }

    return mMachines[machineNum];
}

/**
 * Move the active machine's drawing origin.
 * @param location Device coordinates for the machine origin.
 */
void MachineSystem::SetLocation(wxPoint location)
{
    auto machine = GetCreateMachine(mCurrentMachineNum);
    if (machine != nullptr)
    {
        machine->SetLocation(location);
    }
}

/**
 * Retrieve the current machine location.
 * @return Origin point for the active machine, or (0,0) if unavailable.
 */
wxPoint MachineSystem::GetLocation()
{
    auto machine = GetCreateMachine(mCurrentMachineNum);
    if (machine != nullptr)
    {
        return machine->GetLocation();
    }
    return wxPoint(0, 0);
}

/**
 * Render the currently selected machine to the provided graphics context.
 * @param graphics Drawing context used by the caller.
 */
void MachineSystem::DrawMachine(std::shared_ptr<wxGraphicsContext> graphics)
{
    auto machine = GetCreateMachine(mCurrentMachineNum);
    if (machine != nullptr)
    {
        machine->Draw(graphics);
    }
}

/**
 * Set the frame index for the currently active machine and advance its state.
 * @param frame Frame number to set.
 */
void MachineSystem::SetMachineFrame(int frame)
{
    auto machine = GetCreateMachine(mCurrentMachineNum);
    if (machine != nullptr)
    {
        // If going backwards, reset and step forward from 0
        if (frame < mCurrentFrame)
        {
            machine->Reset();
            mCurrentFrame = 0;
        }

        // Step forward to the target frame
        for (; mCurrentFrame < frame; mCurrentFrame++)
        {
            machine->Update(1.0 / mFrameRate);
        }

        mCurrentFrame = frame;
    }
}

/**
 * Configure the global frame rate and propagate it to all machines.
 * @param rate Frames per second to use for simulation steps.
 */
void MachineSystem::SetFrameRate(double rate)
{
    mFrameRate = rate;

    for (auto machine : mMachines)
    {
        if (machine != nullptr)
        {
            machine->SetFrameRate(rate);
        }
    }
}

/**
 * Select which machine index is active.
 * @param machine Index of the machine to activate.
 */
void MachineSystem::ChooseMachine(int machine)
{
    mCurrentMachineNum = machine;
    GetCreateMachine(machine);
}

/**
 * Return the index of the currently active machine.
 * @return Active machine number.
 */
int MachineSystem::GetMachineNumber()
{
    return mCurrentMachineNum;
}

/**
 * Compute the elapsed time for the active machine based on frame count.
 * @return Elapsed time in seconds; returns 0 if frame rate is invalid.
 */
double MachineSystem::GetMachineTime()
{
    if (mFrameRate > 0)
    {
        return (double)mCurrentFrame / mFrameRate;
    }
    return 0.0;
}

/**
 * Update an internal flag (placeholder for control panel values).
 * @param flag Integer value from external control UI.
 */
void MachineSystem::SetFlag(int flag)
{
    // Reserved for future control flags.
}
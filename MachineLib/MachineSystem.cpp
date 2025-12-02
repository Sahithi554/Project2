/**
 * @file MachineSystem.cpp
 * @author sahithi
 *
 *
 * Implementation for the MachineSystem which builds and manages
 * Machine instances and their contents (components, physics world, etc.).
 */

#include "pch.h"
#include "IMachineSystem.h"
#include "MachineSystem.h"
#include "Machine.h"
#include "Shape.h"
#include "Motor.h"
#include "ContactListener.h"
#include "Pulley.h"
#include "Consts.h"
#include <b2_world.h>
#include <b2_body.h>

/// The active machine index used by the system.
static int gCurrentMachineNum = 1;

/**
 * Construct a MachineSystem.
 *
 * The system stores the resources directory and preselects the
 * initial machine (machine 1).
 *
 * @param resourcesDir Path to the resources folder (images, etc.)
 */
MachineSystem::MachineSystem(std::wstring resourcesDir) : mResourcesDir(resourcesDir)
{
    // Prepare the first machine immediately so the UI can query it.
    ChooseMachine(1);
}

/**
 * Create or return an existing Machine for the requested index.
 *
 * This routine lazily constructs the machine on first access and
 * configures it (frame rate, components, physics bodies) according
 * to the machine number. For machine 1 a set of demo components
 * (floor, beam, wedge, basketball, motor) are created and wired up.
 *
 * @param machineNum Index of the machine to get or create.
 * @return Shared pointer to the Machine instance.
 */
std::shared_ptr<Machine> MachineSystem::GetOrCreateMachine(int machineNum)
{
    // Make sure our vector can hold the requested index.
    if (mMachines.size() <= machineNum)
    {
        mMachines.resize(machineNum + 1);
    }

    // If not present, construct and configure the machine.
    if (mMachines[machineNum] == nullptr)
    {
        auto machine = std::make_shared<Machine>(machineNum);
        machine->SetFrameRate(mFrameRate);

        if (machineNum == 1)
        {
            // Assemble a sample level for machine 1. Measurements are in cm.
            auto world = machine->GetWorld();
            auto imagesDir = mResourcesDir + L"/images";

            // Floor
            const double FloorWidth = 600;
            const double FloorHeight = 15;
            auto floor = std::make_shared<Shape>(machine.get());
            floor->Rectangle(-FloorWidth/2, -FloorHeight, FloorWidth, FloorHeight);
            floor->SetImage(imagesDir + L"/floor.png");
            floor->SetInitialPosition(0, -40);
            machine->AddComponent(floor);
            if (world != nullptr)
            {
                floor->GetPolygon()->InstallPhysics(world);
                floor->SetBody(floor->GetPolygon()->GetBody());
            }

            // Beam
            const double BeamWidth = 240;
            auto beam = std::make_shared<Shape>(machine.get());
            beam->Rectangle(-FloorWidth/2, -FloorHeight, BeamWidth, FloorHeight);
            beam->SetImage(imagesDir + L"/beam.png");
            beam->SetInitialPosition(FloorWidth/2 - BeamWidth/2, 120);
            machine->AddComponent(beam);
            if (world != nullptr)
            {
                beam->GetPolygon()->InstallPhysics(world);
                beam->SetBody(beam->GetPolygon()->GetBody());
            }

            // Wedge (ramp)
            auto wedge = std::make_shared<Shape>(machine.get());
            wedge->AddPoint(-25, 0);
            wedge->AddPoint(25, 0);
            wedge->AddPoint(25, 4.5);
            wedge->AddPoint(-25, 25);
            wedge->SetImage(imagesDir + L"/wedge.png");
            wedge->SetInitialPosition(-95, 120);
            machine->AddComponent(wedge);
            if (world != nullptr)
            {
                wedge->GetPolygon()->InstallPhysics(world);
                wedge->SetBody(wedge->GetPolygon()->GetBody());
            }

            // Basketball (dynamic object)
            auto basketball = std::make_shared<Shape>(machine.get());
            basketball->Circle(16); // radius in cm
            basketball->SetImage(imagesDir + L"/basketball.png");
            basketball->SetInitialPosition(-95, 220);
            basketball->SetDynamic();
            basketball->GetPolygon()->SetPhysics(1.0, 0.5, 0.5);
            if (world != nullptr)
            {
                basketball->GetPolygon()->InstallPhysics(world);
                basketball->SetBody(basketball->GetPolygon()->GetBody());
            }
            machine->AddComponent(basketball);

            // Motor (activates when contacted)
            auto motor = std::make_shared<Motor>(machine.get(), imagesDir);
            motor->SetPosition(80, 120);
            motor->SetInitiallyRunning(false);
            motor->SetSpeed(0.25);
            if (world != nullptr)
            {
                motor->GetBox()->InstallPhysics(world);
                motor->SetBody(motor->GetBox()->GetBody());

                auto contactListener = machine->GetContactListener();
                if (contactListener != nullptr)
                {
                    // Register the motor as a listener for its body so it can react to collisions.
                    contactListener->Add(motor->GetBody(), motor.get());
                }
            }
            machine->AddComponent(motor);

            // Pulley 1 - driven directly by motor
            auto pulley1 = std::make_shared<Pulley>(machine.get(), 10);
            pulley1->SetImage(imagesDir + L"/pulley.png");
            pulley1->SetPosition(80, 145); // Position above motor
            machine->AddComponent(pulley1);

            // Connect motor to pulley1
            motor->GetRotationSource()->AddSink(pulley1.get());

            // Pulley 2 - driven by pulley1 via belt
            auto pulley2 = std::make_shared<Pulley>(machine.get(), 10);
            pulley2->SetImage(imagesDir + L"/pulley2.png");
            pulley2->SetPosition(150, 200);
            machine->AddComponent(pulley2);

            // Connect pulley1 to pulley2
            pulley1->Drive(pulley2.get());

            // Test: Create a shape and have pulley2 rotate it
            auto spoon = std::make_shared<Shape>(machine.get());
            spoon->AddPoint(-5, 0);
            spoon->AddPoint(5, 0);
            spoon->AddPoint(5, 25);
            spoon->AddPoint(-5, 25);
            spoon->SetImage(imagesDir + L"/spoon.png");
            spoon->SetInitialPosition(150, 250);
            spoon->SetKinematic();
            if (world != nullptr)
            {
                spoon->GetPolygon()->InstallPhysics(world);
                spoon->SetBody(spoon->GetPolygon()->GetBody());
            }
            machine->AddComponent(spoon);

            // Connect pulley2 to shape
            pulley2->GetRotationSource()->AddSink(spoon.get());

            // Platform 1 - Upper platform for dominoes (right side)
            // Based on example: position (300, 250), width 300cm
            // Rectangle is centered, so -150 to +150 relative to platform center
            auto platform1 = std::make_shared<Shape>(machine.get());
            platform1->Rectangle(-150, -FloorHeight/2, 300, FloorHeight);
            platform1->SetImage(imagesDir + L"/floor.png");
            platform1->SetInitialPosition(300, 300);  // Center of platform
            machine->AddComponent(platform1);
            if (world != nullptr)
            {
                platform1->GetPolygon()->InstallPhysics(world);
                platform1->SetBody(platform1->GetPolygon()->GetBody());
            }

            // Platform 2 - Lower platform for bowling pins (right side)
            // Based on example: position (300, 100), width 300cm
            auto platform2 = std::make_shared<Shape>(machine.get());
            platform2->Rectangle(-150, -FloorHeight/2, 300, FloorHeight);
            platform2->SetImage(imagesDir + L"/floor.png");
            platform2->SetInitialPosition(300, 100);  // Center of platform
            machine->AddComponent(platform2);
            if (world != nullptr)
            {
                platform2->GetPolygon()->InstallPhysics(world);
                platform2->SetBody(platform2->GetPolygon()->GetBody());
            }
        }

        // Store the constructed machine for future requests.
        mMachines[machineNum] = machine;
    }

    return mMachines[machineNum];
}

/**
 * Move the active machine's drawing origin.
 *
 * @param location Device coordinates for the machine origin.
 */
void MachineSystem::SetLocation(wxPoint location)
{
    auto machine = GetOrCreateMachine(gCurrentMachineNum);
    if (machine != nullptr)
    {
        machine->SetLocation(location);
    }
}

/**
 * Retrieve the current machine location.
 *
 * @return Origin point for the active machine, or (0,0) if unavailable.
 */
wxPoint MachineSystem::GetLocation()
{
    auto machine = GetOrCreateMachine(gCurrentMachineNum);
    if (machine != nullptr)
    {
        return machine->GetLocation();
    }
    return wxPoint(0, 0);
}

/**
 * Render the currently selected machine to the provided graphics context.
 *
 * @param graphics Drawing context used by the caller.
 */
void MachineSystem::DrawMachine(std::shared_ptr<wxGraphicsContext> graphics)
{
    auto machine = GetOrCreateMachine(gCurrentMachineNum);
    if (machine != nullptr)
    {
        machine->Draw(graphics);
    }
}

/**
 * Set the frame index for the currently active machine and advance its state.
 *
 * @param frame Frame number to set.
 */
void MachineSystem::SetMachineFrame(int frame)
{
    mCurrentFrame = frame;
    auto machine = GetOrCreateMachine(gCurrentMachineNum);
    if (machine != nullptr)
    {
        machine->SetFrame(frame);
    }
}

/**
 * Configure the global frame rate and propagate it to all machines.
 *
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
 *
 * Ensures that the requested machine exists and becomes the current one.
 *
 * @param machine Index of the machine to activate.
 */
void MachineSystem::ChooseMachine(int machine)
{
    gCurrentMachineNum = machine;
    GetOrCreateMachine(machine);
}

/**
 * Return the index of the currently active machine.
 *
 * @return Active machine number.
 */
int MachineSystem::GetMachineNumber()
{
    return gCurrentMachineNum;
}

/**
 * Compute the elapsed time for the active machine based on frame count.
 *
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
 *
 * @param flag Integer value from external control UI.
 */
void MachineSystem::SetFlag(int flag)
{
    // Reserved for future control flags.
}

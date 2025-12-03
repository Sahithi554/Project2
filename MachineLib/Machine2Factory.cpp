/**
 * @file Machine2Factory.cpp
 * @author Sahithi Nalamalpu
 */

#include "pch.h"
#include "Machine2Factory.h"
#include "Machine.h"
#include "Shape.h"
#include "Pulley.h"
#include "Motor.h"
#include "Conveyor.h"
#include "ContactListener.h"

/// The images directory in resources
const std::wstring ImagesDirectory2 = L"/images";

/**
 * Constructor
 * @param resourcesDir Path to the resources directory
 */
Machine2Factory::Machine2Factory(std::wstring resourcesDir) : mResourcesDir(resourcesDir)
{
    mImagesDir = mResourcesDir + ImagesDirectory2;
}

/**
 * Factory method to create machine #2
 * @param num The machine number
 * @return Shared pointer to the created machine
 */
std::shared_ptr<Machine> Machine2Factory::Create(int num)
{
    auto machine = std::make_shared<Machine>(num);
    auto world = machine->GetWorld();
    auto contactListener = machine->GetContactListener();

    // Floor
    const double FloorWidth = 600;
    const double FloorHeight = 15;
    auto floor = std::make_shared<Shape>(machine.get());
    floor->Rectangle(-FloorWidth/2, -FloorHeight, FloorWidth, FloorHeight);
    floor->SetImage(mImagesDir + L"/floor.png");
    floor->SetInitialPosition(0, -40);
    machine->AddComponent(floor);
    if (world) {
        floor->GetPolygon()->InstallPhysics(world);
        floor->SetBody(floor->GetPolygon()->GetBody());
    }

    // Basketball
    auto basketball = std::make_shared<Shape>(machine.get());
    basketball->Circle(16);
    basketball->SetImage(mImagesDir + L"/basketball.png");
    basketball->SetInitialPosition(-150, 250);
    basketball->SetDynamic();
    basketball->SetPhysics(1, 0.5, 0.5);
    machine->AddComponent(basketball);
    if (world) {
        basketball->GetPolygon()->InstallPhysics(world);
        basketball->SetBody(basketball->GetPolygon()->GetBody());
    }

    // Ramp
    auto ramp = std::make_shared<Shape>(machine.get());
    ramp->AddPoint(-50, 0);
    ramp->AddPoint(50, 0);
    ramp->AddPoint(50, 5);
    ramp->AddPoint(-50, 40);
    ramp->SetImage(mImagesDir + L"/wedge.png");
    ramp->SetInitialPosition(-100, 150);
    machine->AddComponent(ramp);
    if (world) {
        ramp->GetPolygon()->InstallPhysics(world);
        ramp->SetBody(ramp->GetPolygon()->GetBody());
    }

    // Motor
    auto motor = std::make_shared<Motor>(machine.get(), mImagesDir);
    motor->SetPosition(50, 100);
    motor->SetInitiallyRunning(false);  // Idle until ball hits it
    motor->SetSpeed(0.5);
    machine->AddComponent(motor);
    if (world) {
        motor->GetBox()->InstallPhysics(world);
        motor->SetBody(motor->GetBox()->GetBody());
        if (contactListener) {
            contactListener->Add(motor->GetBody(), motor.get());
        }
    }
    auto motorShaft = motor->GetShaftPosition();

    // Pulley on motor
    auto pulley1 = std::make_shared<Pulley>(machine.get(), 10);
    pulley1->SetImage(mImagesDir + L"/pulley.png");
    pulley1->SetPosition(motorShaft.m_x, motorShaft.m_y);
    machine->AddComponent(pulley1);
    motor->GetSource()->AddSink(pulley1.get());

    // Second pulley
    auto pulley2 = std::make_shared<Pulley>(machine.get(), 15);
    pulley2->SetImage(mImagesDir + L"/pulley2.png");
    pulley2->SetPosition(150, 150);
    machine->AddComponent(pulley2);
    pulley1->Drive(pulley2.get());

    // Spinning shape driven by pulley2
    auto spinner = std::make_shared<Shape>(machine.get());
    spinner->Rectangle(-10, -40, 20, 80);
    spinner->SetImage(mImagesDir + L"/spoon.png");
    spinner->SetInitialPosition(150, 200);
    spinner->SetKinematic();
    machine->AddComponent(spinner);
    if (world) {
        spinner->GetPolygon()->InstallPhysics(world);
        spinner->SetBody(spinner->GetPolygon()->GetBody());
    }
    pulley2->GetRotationSource()->AddSink(spinner.get());

    // Platform
    auto platform = std::make_shared<Shape>(machine.get());
    platform->Rectangle(-100, -FloorHeight/2, 200, FloorHeight);
    platform->SetImage(mImagesDir + L"/floor.png");
    platform->SetInitialPosition(-150, 50);
    machine->AddComponent(platform);
    if (world) {
        platform->GetPolygon()->InstallPhysics(world);
        platform->SetBody(platform->GetPolygon()->GetBody());
    }

    return machine;
}
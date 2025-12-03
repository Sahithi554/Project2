/**
 * @file Machine1Factory.cpp
 * @author Anik Momtaz
 * @author Sahithi Nalamalpu (corrections)
 */

#include "pch.h"
#include "Machine1Factory.h"
#include "Machine.h"
#include "Shape.h"
#include "Pulley.h"
#include "Motor.h"
#include "Conveyor.h"
#include "Elevator.h"
#include "ContactListener.h"

/// The images directory in resources
const std::wstring ImagesDirectory = L"/images";

const double FloorWidth = 600;
const double FloorHeight = 15;

/**
 * Constructor
 * @param resourcesDir Path to the resources directory
 */
Machine1Factory::Machine1Factory(std::wstring resourcesDir) : mResourcesDir(resourcesDir)
{
    mImagesDir = mResourcesDir + ImagesDirectory;
}

/**
 * Factory method to create machine #1
 * @param num The machine number
 * @return Shared pointer to the created machine
 */
std::shared_ptr<Machine> Machine1Factory::Create(int num)
{
    auto machine = std::make_shared<Machine>(num);
    auto world = machine->GetWorld();

    // Basketball
    auto basketball = std::make_shared<Shape>(machine.get());
    basketball->Circle(16);
    basketball->SetImage(mImagesDir + L"/basketball.png");
    basketball->SetInitialPosition(-200, 350);
    basketball->SetDynamic();
    basketball->SetPhysics(1, 0.5, 0.5);
    machine->AddComponent(basketball);
    if (world) {
        basketball->GetPolygon()->InstallPhysics(world);
        basketball->SetBody(basketball->GetPolygon()->GetBody());
    }

    // Bowling ball
    auto bowlingball = std::make_shared<Shape>(machine.get());
    bowlingball->Circle(16);
    bowlingball->SetImage(mImagesDir + L"/bowlingball.png");
    bowlingball->SetInitialPosition(-300, 80);
    bowlingball->SetDynamic();
    bowlingball->SetPhysics(5, 0.5, 0.6);
    machine->AddComponent(bowlingball);
    if (world) {
        bowlingball->GetPolygon()->InstallPhysics(world);
        bowlingball->SetBody(bowlingball->GetPolygon()->GetBody());
    }

    // Tennis ball
    auto tennisball = std::make_shared<Shape>(machine.get());
    tennisball->Circle(8);
    tennisball->SetImage(mImagesDir + L"/tennisball.png");
    tennisball->SetInitialPosition(-230, 100);
    tennisball->SetDynamic();
    tennisball->SetPhysics(1, 0.5, 0.8);
    machine->AddComponent(tennisball);
    if (world) {
        tennisball->GetPolygon()->InstallPhysics(world);
        tennisball->SetBody(tennisball->GetPolygon()->GetBody());
    }

    // Floor
    auto floor = std::make_shared<Shape>(machine.get());
    int thisFloorWidth = FloorWidth + 200;
    floor->Rectangle(-thisFloorWidth/2, -FloorHeight, thisFloorWidth, FloorHeight);
    floor->SetImage(mImagesDir + L"/floor.png");
    floor->SetInitialPosition(50, -40);
    machine->AddComponent(floor);
    if (world) {
        floor->GetPolygon()->InstallPhysics(world);
        floor->SetBody(floor->GetPolygon()->GetBody());
    }

    // Elevator and Conveyor system
    ElevatorAndConveyor(machine);

    // Platform 1 (for dominoes)
    auto platform1 = std::make_shared<Shape>(machine.get());
    platform1->Rectangle(-150, -FloorHeight/2, 300, FloorHeight);
    platform1->SetImage(mImagesDir + L"/floor.png");
    platform1->SetInitialPosition(210, 250);
    machine->AddComponent(platform1);
    if (world) {
        platform1->GetPolygon()->InstallPhysics(world);
        platform1->SetBody(platform1->GetPolygon()->GetBody());
    }

    DominoStack(machine, wxPoint2DDouble(150, 250));

    // Platform 2 (for bowling pins)
    auto platform2 = std::make_shared<Shape>(machine.get());
    platform2->Rectangle(-150, -FloorHeight/2, 300, FloorHeight);
    platform2->SetImage(mImagesDir + L"/floor.png");
    platform2->SetInitialPosition(265, 100);
    machine->AddComponent(platform2);
    if (world) {
        platform2->GetPolygon()->InstallPhysics(world);
        platform2->SetBody(platform2->GetPolygon()->GetBody());
    }

    BowlingPinStack(machine, wxPoint2DDouble(150, 100));

    // Siege contraption
    SiegeContraption(machine);


    return machine;
}

/**
 * Generate the elevator, the conveyor, and their associated components
 * @param machine Machine to add to
 */
void Machine1Factory::ElevatorAndConveyor(std::shared_ptr<Machine> machine)
{
    auto world = machine->GetWorld();
    auto contactListener = machine->GetContactListener();

    // Motor for elevator/conveyor
    auto motorEC = std::make_shared<Motor>(machine.get(), mImagesDir);
    motorEC->SetPosition(-20, 340);
    motorEC->SetInitiallyRunning(true);
    motorEC->SetSpeed(0.25);
    machine->AddComponent(motorEC);
    if (world) {
        motorEC->GetBox()->InstallPhysics(world);
        motorEC->SetBody(motorEC->GetBox()->GetBody());
        if (contactListener) {
            contactListener->Add(motorEC->GetBody(), motorEC.get());
        }
    }
    auto motorECshaft = motorEC->GetShaftPosition();

    // Conveyor
    auto conveyorEC = std::make_shared<Conveyor>(machine.get());
    conveyorEC->SetSize(100, 15);
    conveyorEC->SetImage(mImagesDir + L"/conveyor.png");
    conveyorEC->SetPosition(-340, 500);
    machine->AddComponent(conveyorEC);
    if (world) {
        conveyorEC->GetPolygon()->InstallPhysics(world);
        conveyorEC->SetBody(conveyorEC->GetPolygon()->GetBody());
    }

    // Elevators
    auto elevator1EC = std::make_shared<Elevator>(machine.get());
    elevator1EC->SetSize(50, 15);
    elevator1EC->SetImage(mImagesDir + L"/beam2.png");
    elevator1EC->SetPosition(-277.5, -47.5);
    machine->AddComponent(elevator1EC);
    if (world) {
        elevator1EC->GetPolygon()->InstallPhysics(world);
        elevator1EC->SetBody(elevator1EC->GetPolygon()->GetBody());
    }

    auto elevator2EC = std::make_shared<Elevator>(machine.get());
    elevator2EC->SetSize(50, 15);
    elevator2EC->SetImage(mImagesDir + L"/beam2.png");
    elevator2EC->SetPosition(-277.5, 102.5);
    machine->AddComponent(elevator2EC);
    if (world) {
        elevator2EC->GetPolygon()->InstallPhysics(world);
        elevator2EC->SetBody(elevator2EC->GetPolygon()->GetBody());
    }

    auto elevator3EC = std::make_shared<Elevator>(machine.get());
    elevator3EC->SetSize(50, 15);
    elevator3EC->SetImage(mImagesDir + L"/beam2.png");
    elevator3EC->SetPosition(-277.5, 252.5);
    machine->AddComponent(elevator3EC);
    if (world) {
        elevator3EC->GetPolygon()->InstallPhysics(world);
        elevator3EC->SetBody(elevator3EC->GetPolygon()->GetBody());
    }

    // Wedge near elevator
    auto wedgeEC = std::make_shared<Shape>(machine.get());
    wedgeEC->AddPoint(-25, 0);
    wedgeEC->AddPoint(25, 0);
    wedgeEC->AddPoint(25, 4.5);
    wedgeEC->AddPoint(-25, 55);
    wedgeEC->SetImage(mImagesDir + L"/wedge.png");
    wedgeEC->SetInitialRotation(-0.25);
    wedgeEC->SetInitialPosition(-490, 545);
    machine->AddComponent(wedgeEC);
    if (world) {
        wedgeEC->GetPolygon()->InstallPhysics(world);
        wedgeEC->SetBody(wedgeEC->GetPolygon()->GetBody());
    }

    // Pulley on motor shaft
    auto pulleyMotorEC = std::make_shared<Pulley>(machine.get(), 10);
    pulleyMotorEC->SetImage(mImagesDir + L"/pulley.png");
    pulleyMotorEC->SetPosition(motorECshaft.m_x, motorECshaft.m_y);
    machine->AddComponent(pulleyMotorEC);

    motorEC->GetSource()->AddSink(pulleyMotorEC.get());

    // Pulley on conveyor
    auto pulleyConveyorEC = std::make_shared<Pulley>(machine.get(), 10);
    pulleyConveyorEC->SetImage(mImagesDir + L"/pulley.png");
    pulleyConveyorEC->SetPosition(-150, 320);  // Position above conveyor
    machine->AddComponent(pulleyConveyorEC);

    pulleyMotorEC->Drive(pulleyConveyorEC.get());

    // Connect conveyor to pulley
    pulleyConveyorEC->GetRotationSource()->AddSink(conveyorEC.get());

    // Pulley for elevators
    auto pulleyElevatorEC = std::make_shared<Pulley>(machine.get(), 10);
    pulleyElevatorEC->SetImage(mImagesDir + L"/pulley.png");
    pulleyElevatorEC->SetPosition(-310, 370);
    machine->AddComponent(pulleyElevatorEC);

    pulleyConveyorEC->Drive(pulleyElevatorEC.get());

    // Connect elevators to pulley
    pulleyElevatorEC->GetRotationSource()->AddSink(elevator1EC.get());
    pulleyElevatorEC->GetRotationSource()->AddSink(elevator2EC.get());
    pulleyElevatorEC->GetRotationSource()->AddSink(elevator3EC.get());
}

/**
 * Generate the siege contraption, and its associated components
 * @param machine Machine to add to
 */
void Machine1Factory::SiegeContraption(std::shared_ptr<Machine> machine)
{
    auto world = machine->GetWorld();
    auto contactListener = machine->GetContactListener();

    // Beam for siege
    auto beamSC = std::make_shared<Shape>(machine.get());
    beamSC->BottomCenteredRectangle(180, FloorHeight);
    beamSC->SetImage(mImagesDir + L"/beam.png");
    beamSC->SetInitialPosition(-290, 420);
    machine->AddComponent(beamSC);
    if (world) {
        beamSC->GetPolygon()->InstallPhysics(world);
        beamSC->SetBody(beamSC->GetPolygon()->GetBody());
    }

    // Wedge
    auto wedgeSC = std::make_shared<Shape>(machine.get());
    wedgeSC->AddPoint(-5, 0);
    wedgeSC->AddPoint(0, 0);
    wedgeSC->AddPoint(0, 4.5);
    wedgeSC->AddPoint(-5, 45);
    wedgeSC->SetImage(mImagesDir + L"/wedge.png");
    wedgeSC->SetInitialRotation(0.25);
    wedgeSC->SetInitialPosition(-90, 240);
    machine->AddComponent(wedgeSC);
    if (world) {
        wedgeSC->GetPolygon()->InstallPhysics(world);
        wedgeSC->SetBody(wedgeSC->GetPolygon()->GetBody());
    }

    // Motor for siege (oscillating)
    auto motorSC = std::make_shared<Motor>(machine.get(), mImagesDir);
    motorSC->SetPosition(-230, 25);
    motorSC->SetInitiallyRunning(false);  // Idle until hit
    motorSC->SetSpeed(0.2);
    motorSC->SetOscillating(true);
    motorSC->SetSpread(0.1);
    motorSC->SetWindupToReleaseRatio(5.0);
    machine->AddComponent(motorSC);
    if (world) {
        motorSC->GetBox()->InstallPhysics(world);
        motorSC->SetBody(motorSC->GetBox()->GetBody());
        if (contactListener) {
            contactListener->Add(motorSC->GetBody(), motorSC.get());
        }
    }
    auto motorSCshaft = motorSC->GetShaftPosition();

    // Large middle pulley (outer)
    auto pulleyMidOuterSC = std::make_shared<Pulley>(machine.get(), 25);
    pulleyMidOuterSC->SetImage(mImagesDir + L"/pulley.png");
    pulleyMidOuterSC->SetPosition(-125, 140);
    machine->AddComponent(pulleyMidOuterSC);

    // Pulley on motor shaft
    auto pulleyMotorSC = std::make_shared<Pulley>(machine.get(), 10);
    pulleyMotorSC->SetImage(mImagesDir + L"/pulley.png");
    pulleyMotorSC->SetPosition(motorSCshaft.m_x, motorSCshaft.m_y);
    machine->AddComponent(pulleyMotorSC);

    motorSC->GetSource()->AddSink(pulleyMotorSC.get());

    // Small middle pulley (inner) - coaxial with outer
    auto pulleyMidInnerSC = std::make_shared<Pulley>(machine.get(), 10);
    pulleyMidInnerSC->SetImage(mImagesDir + L"/pulley.png");
    pulleyMidInnerSC->SetPosition(-125, 140);
    machine->AddComponent(pulleyMidInnerSC);

    pulleyMotorSC->Drive(pulleyMidInnerSC.get());

    // Connect inner to outer (they're on the same shaft, so they rotate together)
    pulleyMidInnerSC->GetRotationSource()->AddSink(pulleyMidOuterSC.get());

    // Pulley for the arm/spoon
    auto pulleyArmSC = std::make_shared<Pulley>(machine.get(), 10);
    pulleyArmSC->SetImage(mImagesDir + L"/pulley.png");
    pulleyArmSC->SetPosition(-205, 215);
    machine->AddComponent(pulleyArmSC);
    auto pulleyArmSCshaft = pulleyArmSC->GetPosition();

    pulleyMidOuterSC->Drive(pulleyArmSC.get());

    // Spoon (catapult arm)
    auto spoonSC = std::make_shared<Shape>(machine.get());
    spoonSC->SetInitialPosition(pulleyArmSCshaft.m_x, pulleyArmSCshaft.m_y);
    spoonSC->AddPoint(-7, 10);
    spoonSC->AddPoint(7, 10);
    spoonSC->AddPoint(7, -60);
    spoonSC->AddPoint(-7, -60);
    spoonSC->SetImage(mImagesDir + L"/spoon.png");
    spoonSC->SetKinematic();
    spoonSC->SetInitialRotation(0.5);
    machine->AddComponent(spoonSC);
    if (world) {
        spoonSC->GetPolygon()->InstallPhysics(world);
        spoonSC->SetBody(spoonSC->GetPolygon()->GetBody());
    }

    pulleyArmSC->GetRotationSource()->AddSink(spoonSC.get());
}

/**
 * Generate a stack of dominoes
 * @param machine Machine to add to
 * @param position Base position for the domino stack
 */
void Machine1Factory::DominoStack(std::shared_ptr<Machine> machine, wxPoint2DDouble position)
{
    Domino(machine, position + wxPoint2DDouble(-DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight/2 + DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Blue);
    Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight/2 + DominoWidth/2, DominoHeight*2.5), 0, DominoColor::Green);
    Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth/2, DominoHeight*2.5), 0, DominoColor::Blue);
    Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth*2.5, DominoHeight*3 + DominoWidth/2), 0.25, DominoColor::Black);

    Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Blue);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 + DominoHeight/2 - DominoWidth*2.5, DominoHeight*2 + DominoWidth/2), 0.25, DominoColor::Black);

    Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
    Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Blue);
    Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 + DominoHeight/2 - DominoWidth*2.5, DominoHeight*2 + DominoWidth/2), 0.25, DominoColor::Black);

    Domino(machine, position + wxPoint2DDouble(-DominoHeight*3 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight*3 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
    Domino(machine, position + wxPoint2DDouble(-DominoHeight*3 + DominoHeight/2 - DominoWidth*2.5, DominoHeight + DominoWidth/2), 0.25, DominoColor::Black);

    Domino(machine, position + wxPoint2DDouble(DominoHeight*3 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(DominoHeight*3 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
    Domino(machine, position + wxPoint2DDouble(DominoHeight*3 + DominoHeight/2 - DominoWidth*2.5, DominoHeight + DominoWidth/2), 0.25, DominoColor::Black);
}

/**
 * Generate a stack of bowling pins
 * @param machine Machine to add to
 * @param position Base position for the pin stack
 */
void Machine1Factory::BowlingPinStack(std::shared_ptr<Machine> machine, wxPoint2DDouble position)
{
    BowlingPin(machine, position + wxPoint2DDouble(0, BowlingPinHeight/2));
    BowlingPin(machine, position + wxPoint2DDouble(-DominoHeight, BowlingPinHeight/2));
    BowlingPin(machine, position + wxPoint2DDouble(DominoHeight, BowlingPinHeight/2));
    Domino(machine, position + wxPoint2DDouble(-DominoHeight/2, BowlingPinHeight + DominoWidth/2), 0.25, DominoColor::Red);
    Domino(machine, position + wxPoint2DDouble(DominoHeight/2, BowlingPinHeight + DominoWidth/2), 0.25, DominoColor::Green);

    BowlingPin(machine, position + wxPoint2DDouble(-DominoHeight/2, BowlingPinHeight*1.5 + DominoWidth));
    BowlingPin(machine, position + wxPoint2DDouble(DominoHeight/2, BowlingPinHeight*1.5 + DominoWidth));
    Domino(machine, position + wxPoint2DDouble(0, BowlingPinHeight*2 + DominoWidth*1.5), 0.25, DominoColor::Blue);

    BowlingPin(machine, position + wxPoint2DDouble(0, BowlingPinHeight*2.5 + DominoWidth*2));
}

/**
 * Create a domino and add it to the machine.
 * @param machine Machine to add the domino to
 * @param position Position to place the center of the domino
 * @param rotation Rotation in turns
 * @param color Color code
 * @return Returns the created domino shape
 */
std::shared_ptr<Shape> Machine1Factory::Domino(std::shared_ptr<Machine> machine,
                                                wxPoint2DDouble position,
                                                double rotation,
                                                DominoColor color)
{
    auto world = machine->GetWorld();
    auto x = position.m_x;
    auto y = position.m_y;

    auto domino = std::make_shared<Shape>(machine.get());
    domino->Rectangle(-DominoWidth/2, -DominoHeight/2, DominoWidth, DominoHeight);

    switch(color)
    {
        case DominoColor::Black:
            domino->SetImage(mImagesDir + L"/domino-black.png");
            break;
        case DominoColor::Red:
            domino->SetImage(mImagesDir + L"/domino-red.png");
            break;
        case DominoColor::Green:
            domino->SetImage(mImagesDir + L"/domino-green.png");
            break;
        case DominoColor::Blue:
            domino->SetImage(mImagesDir + L"/domino-blue.png");
            break;
    }

    domino->SetInitialPosition(x, y);
    domino->SetInitialRotation(rotation);
    domino->SetDynamic();
    domino->SetPhysics(0.5, 0.5, 0.75);
    machine->AddComponent(domino);

    if (world) {
        domino->GetPolygon()->InstallPhysics(world);
        domino->SetBody(domino->GetPolygon()->GetBody());
    }

    return domino;
}

/**
 * Create a bowling pin and add it to the machine.
 * @param machine Machine to add the pin to
 * @param position Position to place the center of the pin
 * @return Returns the created bowling pin shape
 */
std::shared_ptr<Shape> Machine1Factory::BowlingPin(std::shared_ptr<Machine> machine,
                                                     wxPoint2DDouble position)
{
    auto world = machine->GetWorld();
    auto x = position.m_x;
    auto y = position.m_y;

    auto pin = std::make_shared<Shape>(machine.get());
    pin->Rectangle(-BowlingPinWidth/2, -BowlingPinHeight/2, BowlingPinWidth, BowlingPinHeight);
    pin->SetImage(mImagesDir + L"/pin.png");
    pin->SetInitialPosition(x, y);
    pin->SetDynamic();
    pin->SetPhysics(0.5, 0.5, 1.0);
    machine->AddComponent(pin);

    if (world) {
        pin->GetPolygon()->InstallPhysics(world);
        pin->SetBody(pin->GetPolygon()->GetBody());
    }

    return pin;
}
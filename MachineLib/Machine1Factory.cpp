/**
* @file Machine1Factory.cpp
* @author Anik Momtaz
* @author Sahithi Nalamalpu (corrections)
*
* ONLY CHANGE: Added ContactListener registration for Elevator and Conveyor
* ALL positions kept exactly as original
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


   // Floor - MAIN GROUND
   auto floor = std::make_shared<Shape>(machine.get());
   int thisFloorWidth = FloorWidth + 225;
   floor->Rectangle(-thisFloorWidth/2, -FloorHeight, thisFloorWidth, FloorHeight);
   floor->SetImage(mImagesDir + L"/floor.png");
   floor->SetInitialPosition(50, -40);
   machine->AddComponent(floor);
   if (world) {
       floor->GetPolygon()->InstallPhysics(world);
       floor->SetBody(floor->GetPolygon()->GetBody());
   }


   // Basketball - YOUR ORIGINAL POSITION
   auto basketball = std::make_shared<Shape>(machine.get());
   basketball->Circle(16);
   basketball->SetImage(mImagesDir + L"/basketball.png");
   basketball->SetInitialPosition(-340, 530);
   basketball->SetDynamic();
   basketball->SetPhysics(1, 0.5, 0.5);
   machine->AddComponent(basketball);
   if (world) {
       basketball->GetPolygon()->InstallPhysics(world);
       basketball->SetBody(basketball->GetPolygon()->GetBody());
   }


   // Bowling ball - YOUR ORIGINAL POSITION
   auto bowlingball = std::make_shared<Shape>(machine.get());
   bowlingball->Circle(16);
   bowlingball->SetImage(mImagesDir + L"/bowlingball.png");
   bowlingball->SetInitialPosition(-420, 80);
   bowlingball->SetDynamic();
   bowlingball->SetPhysics(5, 0.5, 0.6);
   machine->AddComponent(bowlingball);
   if (world) {
       bowlingball->GetPolygon()->InstallPhysics(world);
       bowlingball->SetBody(bowlingball->GetPolygon()->GetBody());
   }


   // Tennis ball - YOUR ORIGINAL POSITION
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


   // Elevator and Conveyor system
   ElevatorAndConveyor(machine);


   // Platform 1 (for dominoes) - YOUR ORIGINAL POSITION
   auto platform1 = std::make_shared<Shape>(machine.get());
   platform1->Rectangle(-150, 0, 300, FloorHeight);
   platform1->SetImage(mImagesDir + L"/floor.png");
   platform1->SetInitialPosition(210, 250);
   machine->AddComponent(platform1);
   if (world) {
       platform1->GetPolygon()->InstallPhysics(world);
       platform1->SetBody(platform1->GetPolygon()->GetBody());
   }


   DominoStack(machine, wxPoint2DDouble(210, 250 + FloorHeight));


   // Platform 2 (for bowling pins) - YOUR ORIGINAL POSITION
   auto platform2 = std::make_shared<Shape>(machine.get());
   platform2->Rectangle(-150, 0, 300, FloorHeight);
   platform2->SetImage(mImagesDir + L"/floor.png");
   platform2->SetInitialPosition(265, 100);
   machine->AddComponent(platform2);
   if (world) {
       platform2->GetPolygon()->InstallPhysics(world);
       platform2->SetBody(platform2->GetPolygon()->GetBody());
   }


   BowlingPinStack(machine, wxPoint2DDouble(265, 100 + FloorHeight));


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


   // Motor for elevator/conveyor - YOUR ORIGINAL POSITION
   auto motorEC = std::make_shared<Motor>(machine.get(), mImagesDir);
   motorEC->SetPosition(-130, 510);
   motorEC->SetInitiallyRunning(true);
   motorEC->SetSpeed(1.0);
   machine->AddComponent(motorEC);
   if (world) {
       motorEC->GetBox()->InstallPhysics(world);
       motorEC->SetBody(motorEC->GetBox()->GetBody());
       if (contactListener) {
           contactListener->Add(motorEC->GetBody(), motorEC.get());
       }
   }
   auto motorECshaft = motorEC->GetShaftPosition();


   // Conveyor - YOUR ORIGINAL POSITION
   auto conveyorEC = std::make_shared<Conveyor>(machine.get());
   conveyorEC->SetSize(100, 15);
   conveyorEC->SetImage(mImagesDir + L"/conveyor.png");
   conveyorEC->SetPosition(-340, 500);
   machine->AddComponent(conveyorEC);
   if (world) {
       conveyorEC->GetPolygon()->InstallPhysics(world);
       conveyorEC->SetBody(conveyorEC->GetPolygon()->GetBody());
       // ✅ ONLY CHANGE: Added ContactListener registration
       if (contactListener) {
           contactListener->Add(conveyorEC->GetBody(), conveyorEC.get());
       }
   }


   // Elevator - YOUR ORIGINAL POSITION
   auto elevator1EC = std::make_shared<Elevator>(machine.get());
   elevator1EC->SetSize(50, 15);
   elevator1EC->SetImage(mImagesDir + L"/beam2.png");
   elevator1EC->SetPosition(-420, -50);
   elevator1EC->GetPolygon()->SetKinematic();
   machine->AddComponent(elevator1EC);
   if (world) {
       elevator1EC->GetPolygon()->InstallPhysics(world);
       elevator1EC->SetBody(elevator1EC->GetPolygon()->GetBody());
       // ✅ ONLY CHANGE: Added ContactListener registration
       if (contactListener) {
           contactListener->Add(elevator1EC->GetBody(), elevator1EC.get());
       }
   }


   // Wedge near elevator - YOUR ORIGINAL POSITION
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

   // BASKETBALL RAMP - Angled beam from basketball (-420, 550) down to conveyor (-340, 500)
   // This gives the basketball a physical surface to roll on
   auto basketballRamp = std::make_shared<Shape>(machine.get());
   basketballRamp->BottomCenteredRectangle(100, 5);  // 100cm long, 5cm thick
   basketballRamp->SetImage(mImagesDir + L"/beam.png");
   basketballRamp->SetInitialRotation(-1.0);  // Angled downward (negative = clockwise in Box2D)
   basketballRamp->SetInitialPosition(-365, 513);  // Midpoint between ball and conveyor
   machine->AddComponent(basketballRamp);
   if (world) {
       basketballRamp->GetPolygon()->InstallPhysics(world);
       basketballRamp->SetBody(basketballRamp->GetPolygon()->GetBody());
   }


   // Pulley on motor shaft - YOUR ORIGINAL POSITION
   auto pulleyMotorEC = std::make_shared<Pulley>(machine.get(), 10);
   pulleyMotorEC->SetImage(mImagesDir + L"/pulley.png");
   pulleyMotorEC->SetPosition(motorECshaft.m_x, motorECshaft.m_y);
   machine->AddComponent(pulleyMotorEC);


   motorEC->GetSource()->AddSink(pulleyMotorEC.get());


   // Pulley on conveyor - YOUR ORIGINAL POSITION
   auto pulleyConveyorEC = std::make_shared<Pulley>(machine.get(), 10);
   pulleyConveyorEC->SetImage(mImagesDir + L"/pulley.png");
   pulleyConveyorEC->SetPosition(-300, 500);
   machine->AddComponent(pulleyConveyorEC);


   pulleyMotorEC->Drive(pulleyConveyorEC.get());


   // Connect conveyor to pulley
   pulleyConveyorEC->GetRotationSource()->AddSink(conveyorEC.get());


   // Pulley for elevator - YOUR ORIGINAL POSITION
   auto pulleyElevatorEC = std::make_shared<Pulley>(machine.get(), 10);
   pulleyElevatorEC->SetImage(mImagesDir + L"/pulley.png");
   pulleyElevatorEC->SetPosition(-475, 550);
   machine->AddComponent(pulleyElevatorEC);


   pulleyConveyorEC->Drive(pulleyElevatorEC.get());


   // Connect elevator to pulley
   pulleyElevatorEC->GetRotationSource()->AddSink(elevator1EC.get());
}


/**
* Generate the siege contraption, and its associated components
* @param machine Machine to add to
*/
void Machine1Factory::SiegeContraption(std::shared_ptr<Machine> machine)
{
   auto world = machine->GetWorld();
   auto contactListener = machine->GetContactListener();


   // Beam for siege - YOUR ORIGINAL POSITION
   auto beamSC = std::make_shared<Shape>(machine.get());
   beamSC->BottomCenteredRectangle(180, FloorHeight);
   beamSC->SetImage(mImagesDir + L"/beam.png");
   beamSC->SetInitialPosition(-200, 215);
   machine->AddComponent(beamSC);
   if (world) {
       beamSC->GetPolygon()->InstallPhysics(world);
       beamSC->SetBody(beamSC->GetPolygon()->GetBody());
   }


   // Wedge - YOUR ORIGINAL POSITION
   auto wedgeSC = std::make_shared<Shape>(machine.get());
   wedgeSC->AddPoint(-5, 0);
   wedgeSC->AddPoint(0, 0);
   wedgeSC->AddPoint(0, 4.5);
   wedgeSC->AddPoint(-5, 45);
   wedgeSC->SetImage(mImagesDir + L"/wedge.png");
   wedgeSC->SetInitialRotation(0.25);
   wedgeSC->SetInitialPosition(-110, 235);
   machine->AddComponent(wedgeSC);
   if (world) {
       wedgeSC->GetPolygon()->InstallPhysics(world);
       wedgeSC->SetBody(wedgeSC->GetPolygon()->GetBody());
   }


   // Motor for siege (oscillating) - YOUR ORIGINAL POSITION
   auto motorSC = std::make_shared<Motor>(machine.get(), mImagesDir);
   motorSC->SetPosition(-230, 25);
   motorSC->SetInitiallyRunning(false);
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


   // Large middle pulley (outer) - YOUR ORIGINAL POSITION
   auto pulleyMidOuterSC = std::make_shared<Pulley>(machine.get(), 25);
   pulleyMidOuterSC->SetImage(mImagesDir + L"/pulley.png");
   pulleyMidOuterSC->SetPosition(-125, 140);
   machine->AddComponent(pulleyMidOuterSC);


   // Pulley on motor shaft - YOUR ORIGINAL POSITION
   auto pulleyMotorSC = std::make_shared<Pulley>(machine.get(), 10);
   pulleyMotorSC->SetImage(mImagesDir + L"/pulley.png");
   pulleyMotorSC->SetPosition(motorSCshaft.m_x, motorSCshaft.m_y);
   machine->AddComponent(pulleyMotorSC);


   motorSC->GetSource()->AddSink(pulleyMotorSC.get());


   // Small middle pulley (inner) - YOUR ORIGINAL POSITION
   auto pulleyMidInnerSC = std::make_shared<Pulley>(machine.get(), 10);
   pulleyMidInnerSC->SetImage(mImagesDir + L"/pulley.png");
   pulleyMidInnerSC->SetPosition(-125, 140);
   machine->AddComponent(pulleyMidInnerSC);


   pulleyMotorSC->Drive(pulleyMidInnerSC.get());


   // Connect inner to outer
   pulleyMidInnerSC->GetRotationSource()->AddSink(pulleyMidOuterSC.get());


   // Pulley for the arm/spoon - YOUR ORIGINAL POSITION
   auto pulleyArmSC = std::make_shared<Pulley>(machine.get(), 10);
   pulleyArmSC->SetImage(mImagesDir + L"/pulley.png");
   pulleyArmSC->SetPosition(-205, 215);
   machine->AddComponent(pulleyArmSC);
   auto pulleyArmSCshaft = pulleyArmSC->GetPosition();


   pulleyMidOuterSC->Drive(pulleyArmSC.get());


   // Spoon (catapult arm) - YOUR ORIGINAL POSITION
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
* @param position Base position for the domino stack (TOP of platform surface)
*/
void Machine1Factory::DominoStack(std::shared_ptr<Machine> machine, wxPoint2DDouble position)
{
   // Center stack (pyramid)
   Domino(machine, position + wxPoint2DDouble(-DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight/2 + DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Blue);
   Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight/2 + DominoWidth/2, DominoHeight*2.5), 0, DominoColor::Green);
   Domino(machine, position + wxPoint2DDouble(DominoHeight/2 - DominoWidth/2, DominoHeight*2.5), 0, DominoColor::Blue);
   Domino(machine, position + wxPoint2DDouble(0, DominoHeight*3 + DominoWidth/2), 0.25, DominoColor::Black);


   // Left stack 1
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Blue);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*1.5, DominoHeight*2 + DominoWidth/2), 0.25, DominoColor::Black);


   // Right stack 1
   Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
   Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 - DominoHeight/2 + DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Blue);
   Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5 + DominoHeight/2 - DominoWidth/2, DominoHeight*1.5), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(DominoHeight*1.5, DominoHeight*2 + DominoWidth/2), 0.25, DominoColor::Black);


   // Left stack 2
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*3 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*3 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
   Domino(machine, position + wxPoint2DDouble(-DominoHeight*3, DominoHeight + DominoWidth/2), 0.25, DominoColor::Black);


   // Right stack 2
   Domino(machine, position + wxPoint2DDouble(DominoHeight*3 - DominoHeight/2 + DominoWidth/2, DominoHeight/2), 0, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(DominoHeight*3 + DominoHeight/2 - DominoWidth/2, DominoHeight/2), 0, DominoColor::Green);
   Domino(machine, position + wxPoint2DDouble(DominoHeight*3, DominoHeight + DominoWidth/2), 0.25, DominoColor::Black);
}


/**
* Generate a stack of bowling pins
* @param machine Machine to add to
* @param position Base position for the pin stack (TOP of platform surface)
*/
void Machine1Factory::BowlingPinStack(std::shared_ptr<Machine> machine, wxPoint2DDouble position)
{
   // Bottom row - 3 pins
   BowlingPin(machine, position + wxPoint2DDouble(0, BowlingPinHeight/2));
   BowlingPin(machine, position + wxPoint2DDouble(-DominoHeight, BowlingPinHeight/2));
   BowlingPin(machine, position + wxPoint2DDouble(DominoHeight, BowlingPinHeight/2));


   // Spacers between bottom and middle
   Domino(machine, position + wxPoint2DDouble(-DominoHeight/2, BowlingPinHeight + DominoWidth/2), 0.25, DominoColor::Red);
   Domino(machine, position + wxPoint2DDouble(DominoHeight/2, BowlingPinHeight + DominoWidth/2), 0.25, DominoColor::Green);


   // Middle row - 2 pins
   BowlingPin(machine, position + wxPoint2DDouble(-DominoHeight/2, BowlingPinHeight*1.5 + DominoWidth));
   BowlingPin(machine, position + wxPoint2DDouble(DominoHeight/2, BowlingPinHeight*1.5 + DominoWidth));


   // Another spacer
   Domino(machine, position + wxPoint2DDouble(0, BowlingPinHeight*2 + DominoWidth*1.5), 0.25, DominoColor::Blue);


   // Top pin
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
   domino->SetPhysics(0.5, 0.95, 0.05);


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
   pin->SetPhysics(0.5, 0.95, 0.05);


   machine->AddComponent(pin);


   if (world) {
       pin->GetPolygon()->InstallPhysics(world);
       pin->SetBody(pin->GetPolygon()->GetBody());
   }


   return pin;
}
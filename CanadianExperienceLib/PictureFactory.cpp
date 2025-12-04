/**
 * @file PictureFactory.cpp
 * @author Charles Owen
 */

#include "pch.h"
#include "PictureFactory.h"
#include "Picture.h"
#include "HaroldFactory.h"
#include "SpartyFactory.h"
#include "Actor.h"
#include "ImageDrawable.h"
#include "MachineDrawable.h"


/// Directory within resources that contains the images.
const std::wstring ImagesDirectory = L"/images";


/**
 * Factory method to create a new picture.
 * @param resourcesDir Directory that contains the resources for this application
 * @return The created picture
 */
std::shared_ptr<Picture> PictureFactory::Create(std::wstring resourcesDir)
{
    auto imagesDir = resourcesDir + ImagesDirectory;

    auto picture = std::make_shared<Picture>();

    // Create the background and add it
    auto background = std::make_shared<Actor>(L"Background");
    background->SetClickable(false);
    background->SetPosition(wxPoint(0, 0));
    auto backgroundI =
            std::make_shared<ImageDrawable>(L"Background", imagesDir + L"/Background.jpg");
    background->AddDrawable(backgroundI);
    background->SetRoot(backgroundI);
    picture->AddActor(background);

    // Create Machine 1 Actor (left side)
    auto machine1Actor = std::make_shared<Actor>(L"Machine1");
    machine1Actor->SetPosition(wxPoint(150, 600));  // Left side of screen

    // Path to MachineLib resources
    std::wstring machineResourcesDir = resourcesDir + L"/../MachineLib/resources";

    auto machine1Drawable = std::make_shared<MachineDrawable>(L"Machine1", machineResourcesDir);
    machine1Drawable->SetMachineNumber(1);
    machine1Drawable->SetStartFrame(0);  // Machine 1 starts at frame 0
    machine1Drawable->SetScale(0.5);     // Scale to fit
    machine1Actor->SetRoot(machine1Drawable);
    machine1Actor->AddDrawable(machine1Drawable);
    picture->AddActor(machine1Actor);

    // Create Machine 2 Actor (right side)
    auto machine2Actor = std::make_shared<Actor>(L"Machine2");
    machine2Actor->SetPosition(wxPoint(700, 600));  // Right side of screen

    auto machine2Drawable = std::make_shared<MachineDrawable>(L"Machine2", machineResourcesDir);
    machine2Drawable->SetMachineNumber(2);
    machine2Drawable->SetStartFrame(450);  // Machine 2 starts at frame 450 (15 seconds)
    machine2Drawable->SetScale(0.5);       // Scale to fit
    machine2Actor->SetRoot(machine2Drawable);
    machine2Actor->AddDrawable(machine2Drawable);
    picture->AddActor(machine2Actor);

    // Create and add Harold
    HaroldFactory haroldFactory;
    auto harold = haroldFactory.Create(imagesDir);

    // This is where Harold will start out.
    harold->SetPosition(wxPoint(300, 600));
    picture->AddActor(harold);

    // Create and add Sparty
    SpartyFactory spartyFactory;
    auto sparty = spartyFactory.Create(imagesDir);

    sparty->SetPosition(wxPoint(550, 620));
    picture->AddActor(sparty);

    return picture;
}
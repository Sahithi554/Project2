/**
 * @file MachineDrawable.cpp
 * @author Sahithi Nalamalpu
 *
 * Implementation of the MachineDrawable adapter class.
 */

#include "pch.h"
#include "MachineDrawable.h"
#include "Timeline.h"

/**
 * Constructor
 * @param name The name of this drawable
 * @param resourcesDir Path to the resources directory
 */
MachineDrawable::MachineDrawable(const std::wstring& name, const std::wstring& resourcesDir)
    : Drawable(name), mResourcesDir(resourcesDir)
{
    // Create the machine using the factory
    MachineSystemFactory factory(resourcesDir);
    mMachine = factory.CreateMachineSystem();

    // Set initial machine number
    mMachine->ChooseMachine(mMachineNumber);
}

/**
 * Draw the machine at the current position
 * @param graphics Graphics context to draw on
 */
void MachineDrawable::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Save the graphics state before transformations
    graphics->PushState();

    // Apply position and scale transformations
    graphics->Translate(mPlacedPosition.x, mPlacedPosition.y);
    graphics->Scale(mScale, mScale);

    // Set the machine location to origin (we handle position via transforms)
    mMachine->SetLocation(wxPoint(0, 0));

    // Draw the machine
    mMachine->DrawMachine(graphics);

    // Restore the graphics state
    graphics->PopState();
}

/**
 * Test if a point is on the machine
 * @param pos Point to test
 * @return true if the point is on the machine
 */
bool MachineDrawable::HitTest(wxPoint pos)
{
    // Create a bounding box for hit testing
    // Machine is approximately 200x600 pixels at scale 1.0
    int width = static_cast<int>(200 * mScale);
    int height = static_cast<int>(600 * mScale);

    int left = mPlacedPosition.x - width / 2;
    int right = mPlacedPosition.x + width / 2;
    int top = mPlacedPosition.y - height;
    int bottom = mPlacedPosition.y;

    return pos.x >= left && pos.x <= right &&
           pos.y >= top && pos.y <= bottom;
}

/**
 * Set the machine number and update the machine
 * @param machine Machine number to display (1 or 2)
 */
void MachineDrawable::SetMachineNumber(int machine)
{
    mMachineNumber = machine;
    mMachine->ChooseMachine(machine);
}

/**
 * Set the timeline for this drawable
 * @param timeline The timeline to use
 */
void MachineDrawable::SetTimeline(Timeline* timeline)
{
    Drawable::SetTimeline(timeline);

    // Set the frame rate for the machine
    if (timeline != nullptr)
    {
        mMachine->SetFrameRate(timeline->GetFrameRate());
    }
}

/**
 * Override to update machine frame when keyframes are retrieved
 */
void MachineDrawable::GetKeyframe()
{
    // Call base class first
    Drawable::GetKeyframe();

    // Get the current frame from the timeline
    Timeline* timeline = GetAngleChannel()->GetTimeline();
    if (timeline != nullptr)
    {
        int currentFrame = timeline->GetCurrentFrame();
        SetMachineFrame(currentFrame);
    }
}

/**
 * Show the machine dialog to select a machine
 * @param parent Parent window for the dialog
 */
void MachineDrawable::ShowDialog(wxWindow* parent)
{
    MachineDialog dialog(parent, mMachine);
    if (dialog.ShowModal() == wxID_OK)
    {
        // Update our stored machine number
        mMachineNumber = mMachine->GetMachineNumber();
    }
}

/**
 * Update the machine to the specified frame
 * @param frame Current frame number
 */
void MachineDrawable::SetMachineFrame(int frame)
{
    // Calculate the machine frame relative to the start frame
    int machineFrame = frame - mStartFrame;

    // Only run the machine if we're past the start frame
    if (machineFrame < 0)
    {
        machineFrame = 0;
    }

    mMachine->SetMachineFrame(machineFrame);
}

/**
 * Save machine-specific state to XML
 * @param node XML node to add attributes to
 */
void MachineDrawable::SaveMachineState(wxXmlNode* node)
{
    node->AddAttribute(L"machine", wxString::Format(wxT("%i"), mMachineNumber));
    node->AddAttribute(L"startframe", wxString::Format(wxT("%i"), mStartFrame));
    node->AddAttribute(L"scale", wxString::Format(wxT("%f"), mScale));
}

/**
 * Load machine-specific state from XML
 * @param node XML node to read attributes from
 */
void MachineDrawable::LoadMachineState(wxXmlNode* node)
{
    mMachineNumber = wxAtoi(node->GetAttribute(L"machine", L"1"));
    mStartFrame = wxAtoi(node->GetAttribute(L"startframe", L"0"));

    wxString scaleStr = node->GetAttribute(L"scale", L"0.75");
    scaleStr.ToDouble(&mScale);

    // Apply the loaded machine number
    mMachine->ChooseMachine(mMachineNumber);
}
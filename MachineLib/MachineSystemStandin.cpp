/**
 * @file MachineSystemStandin.cpp
 * @sahithi
 * @brief Simple placeholder implementation of a machine system.
 *
 * This version connects to the MachineStandin helper class so the rest
 * of the program can operate even before the full machine is implemented.
 */

#include "pch.h"
#include "MachineSystemStandin.h"
#include "MachineStandin.h"

/**
 * Default constructor.
 * Creates the stand-in machine object used for temporary rendering.
 */
MachineSystemStandin::MachineSystemStandin()
{
    mStandin = std::make_shared<MachineStandin>();
}

/**
 * Specify where the stand-in machine should appear on screen.
 * @param location The top-left drawing point for the machine.
 */
void MachineSystemStandin::SetLocation(wxPoint location)
{
    mStandin->SetLocation(location);
}

/**
 * Retrieve the machine’s current drawing position.
 * @return A wxPoint describing the machine’s display coordinates.
 */
wxPoint MachineSystemStandin::GetLocation()
{
    return mStandin->GetLocation();
}

/**
 * Render the placeholder machine graphics.
 * @param graphics The drawing surface provided by wxWidgets.
 */
void MachineSystemStandin::DrawMachine(std::shared_ptr<wxGraphicsContext> graphics)
{
    graphics->SetInterpolationQuality(wxINTERPOLATION_BEST);
    mStandin->DrawMachine(graphics);
}

/**
 * Update the frame number of the animation.
 * @param frame The new animation frame index.
 */
void MachineSystemStandin::SetMachineFrame(int frame)
{
    mStandin->SetMachineFrame(frame);
}

/**
 * Set the playback frame rate.
 * Currently unused in the stand-in implementation.
 * @param rate Frames per second.
 */
void MachineSystemStandin::SetFrameRate([[maybe_unused]] double rate)
{
    // Stand-in machine does not use frame rate.
}

/**
 * Switch the active machine design based on a numeric ID.
 * @param machine ID representing the selected machine setup.
 */
void MachineSystemStandin::ChooseMachine(int machine)
{
    mStandin->SetMachine(machine);
}

/**
 * Get the ID number of the currently selected machine.
 * @return Machine number.
 */
int MachineSystemStandin::GetMachineNumber()
{
    return mStandin->GetMachine();
}

/**
 * Return the simulated machine time.
 * Stand-in machine does not track time, so this always returns 0.
 * @return Time value in seconds.
 */
double MachineSystemStandin::GetMachineTime()
{
    return 0;
}

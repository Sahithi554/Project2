/**
 * @file MachineDrawable.h
 * @author Sahithi Nalamalpu
 *
 * Adapter class that makes a machine system drawable in the Canadian Experience.
 * This is an encapsulation adapter as described in the Gang of Four book Chapter 4.
 */

#ifndef CANADIANEXPERIENCE_MACHINEDRAWABLE_H
#define CANADIANEXPERIENCE_MACHINEDRAWABLE_H

#include "Drawable.h"
#include <machine-api.h>

class Timeline;

/**
 * Adapter class that makes a machine system into a drawable.
 * Uses composition to wrap an IMachineSystem object.
 */
class MachineDrawable : public Drawable
{
private:
    /// The machine system being adapted (encapsulated)
    std::shared_ptr<IMachineSystem> mMachine;

    /// The machine number (1 or 2)
    int mMachineNumber = 1;

    /// Start frame for the machine animation
    int mStartFrame = 0;

    /// Scale factor for the machine
    double mScale = 0.75;

    /// The resources directory path
    std::wstring mResourcesDir;

public:
    MachineDrawable(const std::wstring& name, const std::wstring& resourcesDir);

    /// Default constructor (disabled)
    MachineDrawable() = delete;

    /// Copy constructor (disabled)
    MachineDrawable(const MachineDrawable&) = delete;

    /// Assignment operator (disabled)
    void operator=(const MachineDrawable&) = delete;

    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    bool HitTest(wxPoint pos) override;

    /**
     * Set the machine number to display
     * @param machine Machine number (1 or 2)
     */
    void SetMachineNumber(int machine);

    /**
     * Get the current machine number
     * @return Machine number
     */
    int GetMachineNumber() const { return mMachineNumber; }

    /**
     * Set the start frame for the machine animation
     * @param frame Frame number when machine should start
     */
    void SetStartFrame(int frame) { mStartFrame = frame; }

    /**
     * Get the start frame for the machine animation
     * @return Start frame number
     */
    int GetStartFrame() const { return mStartFrame; }

    /**
     * Set the scale factor for the machine
     * @param scale Scale factor (1.0 = normal size)
     */
    void SetScale(double scale) { mScale = scale; }

    /**
     * Get the scale factor for the machine
     * @return Scale factor
     */
    double GetScale() const { return mScale; }

    void SetTimeline(Timeline* timeline) override;

    /**
     * Override GetKeyframe to update machine frame
     */
    void GetKeyframe() override;

    /**
     * Show the machine selection dialog
     * @param parent Parent window for the dialog
     */
    void ShowDialog(wxWindow* parent);

    /**
     * Update the machine to the current frame
     * @param frame Current frame number
     */
    void SetMachineFrame(int frame);

    /**
     * Save machine-specific attributes to XML
     * @param node XML node to add attributes to
     */
    void SaveMachineState(wxXmlNode* node);

    /**
     * Load machine-specific attributes from XML
     * @param node XML node to read attributes from
     */
    void LoadMachineState(wxXmlNode* node);
};

#endif //CANADIANEXPERIENCE_MACHINEDRAWABLE_H
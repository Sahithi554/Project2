/**
 * @file Component.h
 * @author Sahithi Nalamalpu
 * @brief Base class for machine components - Complete Implementation
 */

#ifndef CANADIANEXPERIENCE_COMPONENT_H
#define CANADIANEXPERIENCE_COMPONENT_H

#include "IRotationSink.h"
#include <memory>
#include <wx/graphics.h>
#include <wx/geometry.h>

class Machine;
class RotationSource;
class b2Body;

/**
 * @brief Base class for machine components
 */
class Component : public IRotationSink {
private:
    /// Position of this component relative to the machine
    wxPoint2DDouble mPosition;

    /// The physics body for this component
    b2Body* mBody = nullptr;

    /// The machine this component belongs to
    Machine* mMachine = nullptr;

    /// The rotation source for this component
    RotationSource* mRotationSource = nullptr;

protected:
    /**
     * @brief Constructor
     * @param machine The machine this component belongs to
     */
    Component(Machine* machine);

public:
    /// Destructor
    virtual ~Component() = default;

    /// Assignment operator (disabled)
    void operator=(const Component &) = delete;

    /// Copy constructor (disabled)
    Component(const Component &) = delete;

    /**
     * @brief Draw this component
     * @param graphics Graphics context to draw on
     */
    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics) = 0;

    /**
     * @brief Update this component based on elapsed time
     * @param time Elapsed time in seconds
     */
    virtual void Update(double time) {}

    /**
     * @brief Set the position of this component
     * @param position New position
     */
    void SetPosition(wxPoint2DDouble position) { mPosition = position; }

    /**
     * @brief Get the position of this component
     * @return Position as wxPoint2DDouble
     */
    wxPoint2DDouble GetPosition() const { return mPosition; }

    /**
     * @brief Set the physics body for this component
     * @param body Pointer to the physics body
     */
    void SetBody(b2Body* body) { mBody = body; }

    /**
     * @brief Get the machine this component belongs to
     * @return Pointer to the Machine
     */
    Machine* GetMachine() const { return mMachine; }

    /**
     * @brief Get the physics body for this component
     * @return Pointer to the physics body
     */
    b2Body* GetBody() const { return mBody; }

    /**
     * @brief Get the rotation source this component is attached to
     * @return Pointer to RotationSource
     */
    RotationSource* GetRotationSource() const { return mRotationSource; }

    // IRotationSink interface implementation
    void SetRotation(double rotation) override;
    void Rotate(double rotation, double speed) override;
    void SetSource(RotationSource* source) override;
};

#endif // CANADIANEXPERIENCE_COMPONENT_H
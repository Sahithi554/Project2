/**
 * @file Component.h
 * @author Sahithi Nalamalpu
 * @brief Base class for machine components.
 *
 * Components can receive rotation from rotation sources and are
 * attached to a Machine. This header defines the abstract Component
 * base class used by concrete component implementations.
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
 * @brief Base class for machine components.
 *
 * Components may be drawn, updated each frame, and receive rotation
 * from rotation sources via the IRotationSink interface.
 */
class Component : public IRotationSink {
private:
    /// Position of this component relative to the machine
    wxPoint2DDouble mPosition;

    /// The physics body for this component (may be nullptr until installed)
    b2Body* mBody = nullptr;

    /// The machine this component belongs to
    Machine* mMachine = nullptr;

    /// The rotation source for this component (may be nullptr)
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
     * @brief Draw this component.
     * @param graphics Graphics context to draw on.
     *
     * Concrete components must implement this to render themselves.
     */
    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics) = 0;

    /**
     * @brief Update this component based on elapsed time.
     * @param time Elapsed time in seconds.
     *
     * Default implementation does nothing; override if component needs per-frame updates.
     */
    virtual void Update(double time) {}

    /**
     * @brief Set the position of this component relative to the machine.
     * @param position New position.
     */
    void SetPosition(wxPoint2DDouble position) { mPosition = position; }

    /**
     * @brief Get the position of this component relative to the machine.
     * @return Position as wxPoint2DDouble.
     */
    wxPoint2DDouble GetPosition() const { return mPosition; }



    /**
     * @brief Set the physics body for this component.
     * @param body Pointer to the physics body.
     */
    void SetBody(b2Body* body) { mBody = body; }

    /**
     * @brief Get the machine this component belongs to.
     * @return Pointer to the Machine.
     */
    Machine* GetMachine() const { return mMachine; }

    /**
     * @brief Get the physics body for this component.
     * @return Pointer to the physics body (may be nullptr if not installed).
     */
    b2Body* GetBody() const { return mBody; }


    /**
     * @brief Get the rotation source this component is attached to.
     * @return Pointer to RotationSource (may be nullptr).
     */
    RotationSource* GetRotationSource() const { return mRotationSource; }

    // IRotationSink interface implementation

    /**
     * @brief Set the rotation for this component.
     * @param rotation Rotation in turns (0-1).
     *
     * Default implementation is empty; components that respond to rotation
     * should override this method.
     */
    void SetRotation(double rotation) override;

    /**
     * @brief Rotate this component with both rotation angle and speed.
     * @param rotation Rotation in turns (0-1).
     * @param speed Rotation speed in turns per second.
     *
     * Default implementation calls SetRotation; components that need speed
     * should override this method.
     */
    void Rotate(double rotation, double speed) override { SetRotation(rotation); }

    /**
     * @brief Set the rotation source for this component.
     * @param source Rotation source pointer.
     */
    void SetSource(RotationSource* source) override;
};

#endif // CANADIANEXPERIENCE_COMPONENT_H

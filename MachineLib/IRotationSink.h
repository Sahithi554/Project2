/**
* @file IRotationSink.h
 * @author Sahithi Nalamalpu
 *
 * Declares an abstract interface for any object capable of receiving
 * rotational input supplied by a corresponding RotationSource. Classes
 * implementing this interface react to updates in rotational state.
 */

#ifndef CANADIANEXPERIENCE_IROTATIONSINK_H
#define CANADIANEXPERIENCE_IROTATIONSINK_H

class RotationSource;

/**
 * Defines the expected behavior for a rotation sink—an element that
 * accepts rotation values provided by another component. Implementations
 * must supply their own logic for how the rotation affects them.
 */
class IRotationSink {
public:
    /// Virtual destructor to support safe cleanup through base pointers.
    virtual ~IRotationSink() = default;

    /**
     * Update the rotation value for the sink.
     * @param rotation A normalized rotation amount, where 0–1 represents a full turn.
     */
    virtual void SetRotation(double rotation) = 0;

    /**
     * Rotate the sink with both rotation angle and speed.
     * @param rotation A normalized rotation amount, where 0–1 represents a full turn.
     * @param speed Rotation speed in turns per second.
     */
    virtual void Rotate(double rotation, double speed) { SetRotation(rotation); }

    /**
     * Associate this sink with a rotation source so it can receive updates.
     * @param source Pointer to the RotationSource responsible for delivering rotation data.
     */
    virtual void SetSource(RotationSource* source) = 0;
};

#endif // CANADIANEXPERIENCE_IROTATIONSINK_H

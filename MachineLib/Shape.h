/**
 * @file Shape.h
 * @author Sahithi Nalamalpu
 * @version 1.0
 *
 * A shape component that can be static, dynamic, or kinematic.
 * Shapes can be assigned a color or an image and can receive rotation from rotation sources.
 */

#ifndef CANADIANEXPERIENCE_SHAPE_H
#define CANADIANEXPERIENCE_SHAPE_H

#include "Component.h"
#include "RotationSource.h"
#include "PhysicsPolygon.h"
#include <memory>
#include <wx/graphics.h>

class Machine;

/**
 * Shape component representing a geometric object in a machine.
 */
class Shape : public Component {


private:

    std::shared_ptr<cse335::PhysicsPolygon> mPolygon; ///< The polygon representing this shape
    std::unique_ptr<RotationSource> mRotationSource; ///< Rotation source for this shape

    double mRotation = 0; ///< Current rotation in turns (0-1)

    bool mIsDynamic = false; ///< True if the shape is dynamic
    bool mIsKinematic = false; ///< True if the shape is kinematic

public:
    /**
     * Constructor
     * @param machine The machine this component belongs to
     */
    Shape(Machine* machine);

    /// Destructor
    virtual ~Shape() = default;

    /// Copy constructor disabled
    Shape(const Shape &) = delete;

    /// Assignment operator disabled
    void operator=(const Shape &) = delete;

    void Rectangle(double x, double y, double width, double height);

    void SetImage(const std::wstring& path);
    void SetColor(wxColour color);
    void SetDynamic();
    void SetKinematic();


    void AddPoint(wxPoint2DDouble point);
    void AddPoint(double x, double y);

    void SetInitialPosition(wxPoint2DDouble pos);
    void SetInitialPosition(double x, double y);

    void SetInitialRotation(double rotation);

    void SetPhysics(double density, double friction, double restitution);

    void BottomCenteredRectangle(double width, double height);

    void Circle(double radius);
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    void SetRotation(double rotation) override;
    void Rotate(double rotation, double speed) override;

    /// Accessors
    std::shared_ptr<cse335::PhysicsPolygon> GetPolygon() const { return mPolygon; }

    RotationSource* GetRotationSource() const { return mRotationSource.get(); }
};

#endif // CANADIANEXPERIENCE_SHAPE_H

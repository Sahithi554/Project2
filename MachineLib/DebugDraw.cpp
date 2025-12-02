/**
 * @file DebugDraw.cpp
 * @author Sahithi Nalamalpu
 * @version 1.0
 *
 * Implements DebugDraw for drawing Box2D objects in a wxGraphicsContext.
 * Used primarily for debugging purposes (drawing outlines, points, and transforms).
 */

#include "pch.h"
#include "DebugDraw.h"
#include "Consts.h"

/**
 * Constructor
 * @param graphics Graphics context to draw on
 */
DebugDraw::DebugDraw(std::shared_ptr<wxGraphicsContext> graphics)
{
    mGraphics = graphics;
}

/**
 * Draw a circle outline
 * @param center Center of the circle in meters
 * @param radius Radius in meters
 * @param color Color to draw
 */
void DebugDraw::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
{
    SetPenColor(color);
    mGraphics->SetBrush(wxNullBrush);

    double x = center.x * Consts::MtoCM;
    double y = center.y * Consts::MtoCM;
    double r = radius * Consts::MtoCM;

    mGraphics->DrawEllipse(x - r, y - r, r * 2, r * 2);
}

/**
 * Draw a solid circle (actually draws outline for debugging)
 * @param center Center of the circle in meters
 * @param radius Radius in meters
 * @param axis Axis to indicate rotation (unused for outline)
 * @param color Color to draw
 */
void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
{
    DrawCircle(center, radius, color);
}

/**
 * Draw a polygon outline
 * @param vertices Array of vertices in meters
 * @param vertexCount Number of vertices
 * @param color Color to draw
 */
void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    SetPenColor(color);

    auto path = mGraphics->CreatePath();
    path.MoveToPoint(vertices[0].x * Consts::MtoCM * mFineLine,
                     vertices[0].y * Consts::MtoCM * mFineLine);

    for(int i = 1; i < vertexCount; i++)
    {
        path.AddLineToPoint(vertices[i].x * Consts::MtoCM * mFineLine,
                            vertices[i].y * Consts::MtoCM * mFineLine);
    }
    path.CloseSubpath();

    mGraphics->PushState();
    mGraphics->Scale(1.0 / mFineLine, 1.0 / mFineLine);
    mGraphics->SetPen(*wxRED_PEN);
    mGraphics->SetBrush(wxNullBrush);
    mGraphics->StrokePath(path);
    mGraphics->PopState();
}

/**
 * Draw a solid polygon (actually draws outline for debugging)
 * @param vertices Array of vertices in meters
 * @param vertexCount Number of vertices
 * @param color Color to draw
 */
void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    DrawPolygon(vertices, vertexCount, color);
}

/**
 * Draw a point as a crosshair
 * @param p Point to draw
 * @param size Size of point (not used)
 * @param color Color to draw
 */
void DebugDraw::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
    SetPenColor(color);

    double x = p.x * Consts::MtoCM * mFineLine;
    double y = p.y * Consts::MtoCM * mFineLine;
    double crosshairRange = mCrosshairSize / 2 * Consts::MtoCM * mFineLine;

    mGraphics->PushState();
    mGraphics->Scale(1.0 / mFineLine, 1.0 / mFineLine);
    mGraphics->StrokeLine(x - crosshairRange, y, x + crosshairRange, y);
    mGraphics->StrokeLine(x, y - crosshairRange, x, y + crosshairRange);
    mGraphics->PopState();
}

/**
 * Draw a 2D transform as a rotated crosshair
 * @param xf The transform to draw
 */
void DebugDraw::DrawTransform(const b2Transform &xf)
{
    double x = xf.p.x * Consts::MtoCM * mFineLine;
    double y = xf.p.y * Consts::MtoCM * mFineLine;
    float crosshairRange = float(mCrosshairSize / 2 * Consts::MtoCM * mFineLine);

    float s = xf.q.s * crosshairRange; // sine of rotation
    float c = xf.q.c * crosshairRange; // cosine of rotation

    mGraphics->PushState();
    mGraphics->Scale(1.0 / mFineLine, 1.0 / mFineLine);

    mGraphics->SetPen(*wxRED_PEN);
    mGraphics->StrokeLine(x - c, y - s, x, y);
    mGraphics->StrokeLine(x + s, y - c, x - s, y + c);

    mGraphics->SetPen(*wxGREEN_PEN);
    mGraphics->StrokeLine(x, y, x + c, y + s);

    mGraphics->PopState();
}

/**
 * Set pen color from Box2D color
 * @param color Box2D color
 * @param width Pen width in pixels (default 1)
 */
void DebugDraw::SetPenColor(const b2Color &color, int width)
{
    wxColour clr(int(color.r * 255), int(color.g * 255), int(color.b * 255), int(color.a * 255));
    wxPen pen(clr, width);
    mGraphics->SetPen(pen);
}

/**
 * Draw a line segment
 * @param p1 Start point in meters
 * @param p2 End point in meters
 * @param color Color to draw
 */
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    SetPenColor(color);

    double x1 = p1.x * Consts::MtoCM;
    double y1 = p1.y * Consts::MtoCM;
    double x2 = p2.x * Consts::MtoCM;
    double y2 = p2.y * Consts::MtoCM;

    mGraphics->SetPen(*wxRED_PEN);
    mGraphics->StrokeLine(x1, y1, x2, y2);
}

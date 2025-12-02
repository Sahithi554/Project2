/**
 * @file DebugDraw.h
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * Debugging support to draw the world from the viewpoint
 * of the physics system.
 *
 * @version 2.00 Updated to support finer lines
 */



#ifndef _DEBUGDRAW_H
#define _DEBUGDRAW_H

#include <b2_draw.h>

/**
 * A drawing utility that converts Box2D debug-draw callbacks into
 * renderable graphics on a wxWidgets graphics context. Only simple
 * outlines and markers are drawn, since the purpose is to help visualize
 * the physics simulation rather than produce production-quality graphics.
 */
class DebugDraw : public b2Draw {
private:
    /// The graphics context used for issuing draw commands.
    std::shared_ptr<wxGraphicsContext> mGraphics;

    /**
     * Helper that configures the active pen color and thickness based
     * on a Box2D color object.
     * @param color Box2D color components in the range [0,1]
     * @param width Pen thickness (default 1px)
     */
    void SetPenColor(const b2Color& color, int width = 1);

    /// The on-screen size of crosshair indicators, expressed in meters.
    double mCrosshairSize = 0.15;

    /**
     * Adjustment factor for line rendering. A larger value results
     * in narrower drawn strokes, useful for high-resolution debugging.
     */
    double mFineLine = 1;

public:
    /**
     * Construct a debug drawer that sends all rendering output to
     * the provided wxGraphicsContext.
     * @param graphics Target drawing surface
     */
    DebugDraw(std::shared_ptr<wxGraphicsContext> graphics);

    /// Default construction is disabled
    DebugDraw() = delete;

    /// Copy construction is disallowed
    DebugDraw(const DebugDraw &) = delete;

    /// Assignment is disabled
    void operator=(const DebugDraw &) = delete;

    /**
     * Adjust the scale used to compute line thickness. A lineWidth
     * of 1 corresponds to the default stroke size. Smaller values
     * cause lines to be drawn more finely.
     * @param lineWidth New scale value
     */
    void SetLineWidth(double lineWidth) { mFineLine = 1.0 / lineWidth; }

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override;
    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override;
    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;
    void DrawTransform(const b2Transform &xf) override;
    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override;
};

#endif //_DEBUGDRAW_H

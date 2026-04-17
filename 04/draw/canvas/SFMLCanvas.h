#ifndef SFML_CANVAS_H
#define SFML_CANVAS_H

#include "ICanvas.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>

class SFMLCanvas final : public ICanvas {
public:
    class RenderError : public std::runtime_error {
    public:
        explicit RenderError(const std::string& msg) : std::runtime_error(msg) {}
    };

    class OutOfBoundsError : public RenderError {
    public:
        explicit OutOfBoundsError(const std::string& msg) : RenderError(msg) {}
    };

    explicit SFMLCanvas(sf::RenderTarget& target);
    ~SFMLCanvas() override = default;

    void DrawLine(const Point& from, const Point& to, uint32_t lineColor) override;
    void FillPolygon(const std::vector<Point>& points, uint32_t fillColor) override;
    void DrawCircle(const Point& center, double radius, uint32_t lineColor) override;
    void FillCircle(const Point& center, double radius, uint32_t fillColor) override;

    void SetScale(double scale);
    void SetOffset(double offsetX, double offsetY);
    void Clear(uint32_t backgroundColor = 0xFFFFFFFF) const;

    double GetScale() const { return m_scale; }
    double GetOffsetX() const { return m_offsetX; }
    double GetOffsetY() const { return m_offsetY; }
    sf::RenderTarget& GetTarget() const { return m_target; }

    bool IsPointVisible(const Point& p) const;

private:
    static float SafeDoubleToFloat(double value, const std::string& context = "");
    static sf::Color ToSFMLColor(uint32_t color);
    sf::Vector2f ToScreen(const Point& p) const;
    float ValidateRadius(double radius) const;
    static bool IsValidForRendering(const sf::Vector2f& screenPoint) ;

    static constexpr double MAX_SAFE_COORD = 1e6;

    sf::RenderTarget& m_target;
    double m_scale = 1.0;
    double m_offsetX = 0.0;
    double m_offsetY = 0.0;
};

#endif // SFML_CANVAS_H
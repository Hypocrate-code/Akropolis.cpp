// #include "UI/HexagonalButton.hpp"
#include <QGraphicsSceneMouseEvent>
#include "UI/HexItem.hpp"
#include <iostream>
#include "Tuile.hpp"
#include <QPainter>
#include <QPainterPath>


QPolygonF QCreateHexagon(QPointF center, qreal radius)
{
    QPolygonF hex;
    for (int i = 0; i < 6; ++i) {
        qreal angle = M_PI / 3 * i;
        hex << QPointF(
            center.x() + radius * cos(angle),
            center.y() + radius * sin(angle)
        );
    }
    return hex;
}

HexItem::HexItem(const Hexagone* hex, QPoint center, int radius) : QGraphicsObject(), m_hexagon(hex)
    {
        std::cout << "Creating HexItem for hexagon of type " << Utils::type_to_string(hex->getType()) << " and color " << Utils::color_to_string(hex->getCouleur()) << std::endl;
        m_polygon = QCreateHexagon(center, radius);

        // Import de la texture
        QPixmap texture(Utils::get_texture(hex));
        
        QRectF bounds = m_polygon.boundingRect();
        // Mise à l'échelle de la texture
        QPixmap scaled = texture.scaled(
            bounds.size().toSize(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        );

        // Création du brush à partir de la texture
        QBrush brush(scaled);
        
        // Replacement du brush correctement sur le polygone
        QTransform t;
        t.translate(bounds.left(), bounds.top());
        brush.setTransform(t);

        m_normal = brush;
        
        effect = new QGraphicsColorizeEffect;
        effect->setColor(Qt::green);
        effect->setStrength(0);
        setGraphicsEffect(effect);

        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);

        setCursor(Qt::PointingHandCursor);
    }

QRectF HexItem::boundingRect() const
{
    return m_polygon.boundingRect().adjusted(-1, -1, 1, 1);
}

QPainterPath HexItem::shape() const
{
    QPainterPath path;
    path.addPolygon(m_polygon);
    return path;
}

void HexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    //painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(m_normal);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(m_polygon);
}
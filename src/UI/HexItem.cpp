// #include "UI/HexagonalButton.hpp"
#include <QGraphicsSceneMouseEvent>
#include "UI/HexItem.hpp"


QPolygon QCreateHexagon(QPointF center, qreal radius)
{
    QPolygon hex;
    for (int i = 0; i < 6; ++i) {
        qreal angle = M_PI / 3 * i;
        hex << QPoint(
            center.x() + radius * cos(angle),
            center.y() + radius * sin(angle)
        );
    }
    return hex;
}

HexItem::HexItem(const Hexagone* hex, QPoint center, int radius) : QGraphicsPolygonItem()
    {
        QPolygonF poly = QCreateHexagon(center, radius);

        // Import de la texture
        QPixmap texture(Utils::get_texture(hex));
        
        QRectF bounds = poly.boundingRect();
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
        
        setPolygon(poly);
        setBrush(m_normal);
        setPen(QPen(Qt::transparent));
        
        effect = new QGraphicsColorizeEffect;
        effect->setColor(Qt::green);
        effect->setStrength(0);
        setGraphicsEffect(effect);

        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);

        setCursor(Qt::PointingHandCursor);
    }
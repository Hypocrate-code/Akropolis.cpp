// #include "UI/HexagonalButton.hpp"
#include <QGraphicsSceneMouseEvent>
#include "UI/HexItem.hpp"


QPoint center(0, 0);

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

HexItem::HexItem(Hexagone* hex)
    {
        QPolygon poly = QCreateHexagon(center, 100);

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





// PETIT CHEH A TOUT CE CODE GENERE

// HexagonalButton::HexagonalButton(QWidget *parent)
//     : QPushButton(parent)
// {
//     setupAnimations();
//     setSize(m_size);
// }

// HexagonalButton::HexagonalButton(const QString &text, QWidget *parent)
//     : QPushButton(text, parent)
// {
//     setupAnimations();
//     setSize(m_size);
// }

// void HexagonalButton::setTexture(const QPixmap &texture)
// {
//     m_texture = texture;
//     update();
// }

// void HexagonalButton::setTexture(const QString &imagePath)
// {
//     m_texture.load(imagePath);
//     update();
// }

// void HexagonalButton::setSize(int size)
// {
//     m_size = size;
//     setFixedSize(size, size);
//     update();
// }

// void HexagonalButton::setHoverColor(const QColor &color)
// {
//     m_hoverColor = color;
//     update();
// }

// void HexagonalButton::setPressColor(const QColor &color)
// {
//     m_pressColor = color;
//     update();
// }

// QSize HexagonalButton::sizeHint() const
// {
//     return QSize(m_size, m_size);
// }

// QSize HexagonalButton::minimumSizeHint() const
// {
//     return QSize(50, 50);
// }

// QPainterPath HexagonalButton::createHexagonPath() const
// {
//     QPainterPath path;
//     const qreal width = this->width();
//     const qreal height = this->height();
    
//     // Calculate hexagon points (flat-topped hexagon)
//     QPointF points[6];
    
//     // Start at top-left corner
//     points[0] = QPointF(width * 0.25, 0);
//     points[1] = QPointF(width * 0.75, 0);
//     points[2] = QPointF(width, height * 0.5);
//     points[3] = QPointF(width * 0.75, height);
//     points[4] = QPointF(width * 0.25, height);
//     points[5] = QPointF(0, height * 0.5);
    
//     path.moveTo(points[0]);
//     for (int i = 1; i < 6; ++i) {
//         path.lineTo(points[i]);
//     }
//     path.closeSubpath();
    
//     return path;
// }

// void HexagonalButton::paintEvent(QPaintEvent *event)
// {
//     Q_UNUSED(event);
    
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);
//     painter.setOpacity(m_opacity);
    
//     QPainterPath hexagon = createHexagonPath();
    
//     // Set clipping region to hexagon
//     painter.setClipPath(hexagon);
    
//     // Draw background
//     painter.fillPath(hexagon, palette().button());
    
//     // Draw texture if available
//     if (!m_texture.isNull()) {
//         // Scale texture to fit hexagon
//         QRectF targetRect = hexagon.boundingRect();
//         painter.drawPixmap(targetRect.toRect(), m_texture.scaled(
//             targetRect.size().toSize(),
//             Qt::IgnoreAspectRatio,
//             Qt::SmoothTransformation
//         ));
//     }
    
//     // Remove clipping for hover/press effects
//     painter.setClipping(false);
    
//     // Draw hover effect
//     if (m_hovered) {
//         painter.fillPath(hexagon, m_hoverColor);
//     }
    
//     // Draw press effect
//     if (m_pressed) {
//         painter.fillPath(hexagon, m_pressColor);
//     }
    
//     // Draw border
//     QPen borderPen(m_borderColor);
//     borderPen.setWidth(2);
//     painter.setPen(borderPen);
//     painter.drawPath(hexagon);
    
//     // Draw text if no texture
//     if (m_texture.isNull() && !text().isEmpty()) {
//         painter.setPen(m_textColor);
//         QFont font = painter.font();
//         font.setPointSize(10);
//         painter.setFont(font);
//         painter.drawText(rect(), Qt::AlignCenter, text());
//     }
// }

// void HexagonalButton::mousePressEvent(QMouseEvent *event)
// {
//     if (createHexagonPath().contains(event->pos())) {
//         m_pressed = true;
//         m_clickAnimation->setDirection(QPropertyAnimation::Forward);
//         m_clickAnimation->start();
//         update();
//     }
//     QPushButton::mousePressEvent(event);
// }

// void HexagonalButton::mouseReleaseEvent(QMouseEvent *event)
// {
//     m_pressed = false;
//     update();
//     QPushButton::mouseReleaseEvent(event);
// }

// void HexagonalButton::enterEvent(QEnterEvent *event)
// {
//     Q_UNUSED(event);
//     m_hovered = true;
//     m_hoverAnimation->setDirection(QPropertyAnimation::Forward);
//     m_hoverAnimation->start();
//     update();
// }

// void HexagonalButton::leaveEvent(QEvent *event)
// {
//     Q_UNUSED(event);
//     m_hovered = false;
//     m_hoverAnimation->setDirection(QPropertyAnimation::Backward);
//     m_hoverAnimation->start();
//     update();
// }

// bool HexagonalButton::event(QEvent *event)
// {
//     // Handle hover events for the hexagon shape
//     if (event->type() == QEvent::HoverMove) {
//         QHoverEvent *hoverEvent = static_cast<QHoverEvent*>(event);
//         bool inside = createHexagonPath().contains(hoverEvent->position().toPoint());
        
//         if (inside && !m_hovered) {
//             m_hovered = true;
//             m_hoverAnimation->setDirection(QPropertyAnimation::Forward);
//             m_hoverAnimation->start();
//             update();
//         } else if (!inside && m_hovered) {
//             m_hovered = false;
//             m_hoverAnimation->setDirection(QPropertyAnimation::Backward);
//             m_hoverAnimation->start();
//             update();
//         }
//     }
    
//     return QPushButton::event(event);
// }

// void HexagonalButton::setupAnimations()
// {
//     // Hover animation
//     m_hoverAnimation = new QPropertyAnimation(this, "opacity", this);
//     m_hoverAnimation->setDuration(150);
//     m_hoverAnimation->setStartValue(0.9);
//     m_hoverAnimation->setEndValue(1.0);
//     m_hoverAnimation->setEasingCurve(QEasingCurve::OutQuad);
    
//     // Click animation
//     m_clickAnimation = new QPropertyAnimation(this, "opacity", this);
//     m_clickAnimation->setDuration(100);
//     m_clickAnimation->setStartValue(1.0);
//     m_clickAnimation->setEndValue(0.8);
//     m_clickAnimation->setEasingCurve(QEasingCurve::OutQuad);
// }
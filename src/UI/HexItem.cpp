// #include "UI/HexagonalButton.hpp"
#include <QGraphicsSceneMouseEvent>
#include "UI/HexItem.hpp"
#include "Tuile.hpp"
#include <QPainter>
#include <QPainterPath>
#include "Utils.hpp"
#include <iostream>


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
        //if (hex && hex->getVoisinsTOP() && hex->getVoisinsTOP()->getType() == Type::Fantome) {
        //    m_hexagon = hex->getVoisinsTOP();
        //}
        m_polygon = QCreateHexagon(center, radius);

        // Import de la texture
        if (!m_hexagon) return;

        QPixmap texture;// = Utils::get_texture(m_hexagon);

        if (hex->getType() == Type::Fantome && hex->getVoisinsBOT()) 
            texture = Utils::get_texture(hex->getVoisinsBOT());
        else
            texture = Utils::get_texture(hex);
        
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
        

        if (hex->getType() == Type::Fantome){
            setOpacity(0.25);
            if (hex->getVoisinsBOT()){
                setOpacity(0);
            }
        }

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
    
    painter->setRenderHint(QPainter::Antialiasing);
    
    if (m_hexagon && m_hexagon->getType() != Type::Fantome) {
        const int depthOffset = 8;

        // Hex à sommet pointé vers l'est (flat-top):
        // Indices des points: 0(E), 1(SE), 2(SW), 3(W), 4(NW), 5(NE)
        //  - S  : [1]-[2]
        //  - SE : [0]-[1]
        struct SideInfo { int segStart; const Hexagone* voisin; };
        SideInfo sides[] = {
            {1, m_hexagon->getVoisinsS()},   // Sud -> segment [1]-[2]
            {0, m_hexagon->getVoisinsSE()}   // Sud-Est -> segment [0]-[1]
        };

        for (const auto& side : sides) {
            const Hexagone* voisin = side.voisin;

            // Dessiner le côté si pas de voisin réel (null ou Fantome)
            bool drawSide = (!voisin) || (voisin->getType() == Type::Fantome);

            if (drawSide) {
                QPolygonF sideSegment;
                int segEnd = (side.segStart + 1) % 6;

                // Ajouter les 2 points du segment
                sideSegment << m_polygon[side.segStart];
                sideSegment << m_polygon[segEnd];

                // Ajouter les points décalés pour l'effet de profondeur
                sideSegment << m_polygon[segEnd] + QPointF(depthOffset, depthOffset);
                sideSegment << m_polygon[side.segStart] + QPointF(depthOffset, depthOffset);

                // Dessiner le segment du côté
                painter->setBrush(QColor(100, 100, 100));
                painter->setPen(QPen(QColor(60, 60, 60), 1));
                painter->drawPolygon(sideSegment);
            }
        }
    }
    
    // Dessiner l'hexagone principal avec un contour
    painter->setBrush(m_normal);
    painter->setPen(QPen(QColor(80, 80, 80), 2)); // Contour gris foncé de 2px
    painter->drawPolygon(m_polygon);

    // Lightweight hover highlight overlay (green tint)
    if (hovered) {
        painter->setBrush(QColor(0, 255, 0, 70));
        painter->setPen(QPen(QColor(0, 180, 0), 2));
        painter->drawPolygon(m_polygon);
    }
}
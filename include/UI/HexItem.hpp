#ifndef HEXITEM_HPP
#define HEXITEM_HPP
#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsColorizeEffect>
#include "Utils.hpp"

class Hexagone;

class HexItem : public QGraphicsPolygonItem
{
public:

    HexItem(const Hexagone* hex, QPoint center, int radius);

protected:

    // Effets visuels hover

    void hoverEnterEvent(QGraphicsSceneHoverEvent*) override
    {
        effect->setStrength(0.2);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override
    {
        effect->setStrength(0);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton) {
            emitClicked();
        }
        QGraphicsPolygonItem::mousePressEvent(event);
    }

private:
    QBrush m_normal;
    QBrush m_hover;
    QGraphicsColorizeEffect* effect;

    void emitClicked()
    {
        qDebug() << "Hex clicked";
    }
};



// #include <QPushButton>
// #include <QPainterPath>
// #include <QPainter>
// #include <QPixmap>
// #include <QPropertyAnimation>

// class HexagonalButton : public QPushButton
// {
//     Q_OBJECT
//     Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    
// public:
//     explicit HexagonalButton(QWidget *parent = nullptr);
//     explicit HexagonalButton(const QString &text, QWidget *parent = nullptr);
    
//     void setTexture(const QPixmap &texture);
//     void setTexture(const QString &imagePath);
//     void setSize(int size);
//     void setHoverColor(const QColor &color);
//     void setPressColor(const QColor &color);
    
//     QSize sizeHint() const override;
//     QSize minimumSizeHint() const override;
    
//     qreal opacity() const { return m_opacity; }
//     void setOpacity(qreal opacity) { m_opacity = opacity; update(); }
    
// protected:
//     void paintEvent(QPaintEvent *event) override;
//     void mousePressEvent(QMouseEvent *event) override;
//     void mouseReleaseEvent(QMouseEvent *event) override;
//     void enterEvent(QEnterEvent *event) override;
//     void leaveEvent(QEvent *event) override;
//     bool event(QEvent *event) override;
    
// private:
//     QPainterPath createHexagonPath() const;
//     void setupAnimations();
    
//     QPixmap m_texture;
//     QColor m_hoverColor = QColor(255, 255, 255, 30);
//     QColor m_pressColor = QColor(255, 255, 255, 50);
//     QColor m_borderColor = QColor(100, 100, 100);
//     QColor m_textColor = Qt::white;
    
//     int m_size = 100;
//     qreal m_opacity = 1.0;
//     bool m_hovered = false;
//     bool m_pressed = false;
    
//     QPropertyAnimation *m_hoverAnimation;
//     QPropertyAnimation *m_clickAnimation;
// };

#endif // HEXAGONALBUTTON_HPP
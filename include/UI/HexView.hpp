#ifndef HEXSCENE_HPP
#define HEXSCENE_HPP
#include <QGraphicsView>
#include <QGraphicsScene>
#include <stack>
#include <unordered_set>
#include "Cite.hpp"

class Hexagone;

class HexView : public QGraphicsView
{   
    Q_OBJECT
    
private:
    QGraphicsScene* scene = new QGraphicsScene(this);
    std::unordered_set<const Hexagone *> visited;
    std::stack<const Hexagone *> pile;
    int radiusHex;
    void drawRecursive(const Hexagone* hex, QPoint centre);

public:
    HexView(int radius = 77, QWidget* parent = nullptr);
    HexView(const Cite* cite, int radius = 77, QWidget* parent = nullptr);
    HexView(const Hexagone* hex, int radius = 77, QWidget* parent = nullptr);
    void launchDrawRecursive(const Hexagone* hex, QPoint centre = QPoint(0,0));
    const void setDrag(bool isDraggable) {isDraggable ? setDragMode(QGraphicsView::ScrollHandDrag) : setDragMode(QGraphicsView::NoDrag);};
    void clearView() {scene->clear(); visited.clear(); while(!pile.empty()) pile.pop();};

    // ~HexView();

signals:
    void hexagonClicked(const Hexagone* hex);

private slots:
    void onHexItemClicked(const Hexagone* hex);
};




#endif
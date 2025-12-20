#include "UI/HexView.hpp"
#include "UI/HexItem.hpp"
#include "Tuile.hpp"

HexView::HexView(int radius, QWidget* parent) : QGraphicsView(parent), radiusHex(radius)
{
    setScene(scene);
};
HexView::HexView( const Cite* cite, int radius, QWidget* parent) : QGraphicsView(parent), radiusHex(radius)
{
    setScene(scene);
    launchDrawRecursive(cite->getTuileDeDepart()->get_hexagones()[1]);
}
HexView::HexView(const Hexagone* hex, int radius, QWidget* parent) : QGraphicsView(parent), radiusHex(radius)
{
    setScene(scene);
    launchDrawRecursive(hex);
}
void HexView::drawRecursive(const Hexagone* hex, QPoint centre)
{   
    HexItem* item = new HexItem(hex, centre, radiusHex);
    scene->addItem(item);
    
    connect(item, &HexItem::hexagonClicked, this, &HexView::onHexItemClicked);
    
    int direction = 0;
    
    const Hexagone *h = pile.top();
    pile.pop();
    if (visited.count(h))
        return;
    visited.insert(h);

    for(Hexagone* v : hex->getVoisins()) {
        if (v != nullptr || visited.count(v))
        {
            pile.push(v);
            drawRecursive(v, Utils::getCentreVoisin(centre, direction, radiusHex));
        }
        direction++;
    }

}

void HexView::onHexItemClicked(const Hexagone* hex)
{
    emit hexagonClicked(hex);
}
void HexView::launchDrawRecursive(const Hexagone* hex, QPoint centre) {
    pile.push(hex);
    drawRecursive(hex, centre);
};
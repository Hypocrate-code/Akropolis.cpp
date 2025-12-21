#include "UI/HexView.hpp"
#include "UI/HexItem.hpp"
#include "Tuile.hpp"
#include <utility>
#include <unordered_set>
#include <stack>
#include <QWheelEvent>

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

void HexView::onHexItemClicked(const Hexagone* hex)
{
    //revoie lhex tout en haut
    const Hexagone* topHex = hex;;
    while (topHex->getVoisinsTOP()) {
        topHex = topHex->getVoisinsTOP();
    }
    emit hexagonClicked(topHex);
}

void HexView::launchDrawRecursive(const Hexagone* hex, QPoint centre) {
    if (!hex) return;

    std::unordered_set<const Hexagone*> seen;
    std::stack<std::pair<const Hexagone*, QPoint>> st;
    st.push({hex, centre});

    while (!st.empty()) {
        auto [h, pos] = st.top();
        st.pop();
        if (!h || seen.count(h)) continue;
        seen.insert(h);

        // Calculer l'offset 3D basé sur le niveau
        int niveau = h->getNiveau();
        const int offsetPerLevel = 8; // Décalage par niveau pour effet isométrique
        QPoint pos3D = pos - QPoint(offsetPerLevel * (niveau - 1), offsetPerLevel * (niveau - 1));

        HexItem* item = new HexItem(h, pos3D, radiusHex);
        item->setZValue(niveau * 10); // Z-order basé sur le niveau pour un bon rendu 3D
        scene->addItem(item);
        connect(item, &HexItem::hexagonClicked, this, &HexView::onHexItemClicked);

        int direction = 0;
        for (Hexagone* v : h->getVoisins()) {
            if (v && !seen.count(v)) {
                st.push({v, Utils::getCentreVoisin(pos, direction, radiusHex)});
            }
            ++direction;
        }
    }
}

void HexView::drawTuile(const Tuile* tuile, QPoint centerPos) {
    if (!tuile) return;
    
    const auto& hexagones = tuile->get_hexagones();
    if (hexagones.size() < 3) return;
    
    // Calculer la position de chaque hexagone en cherchant sa direction depuis hex0
    QPoint positions[3] = {QPoint(0, 0), QPoint(0, 0), QPoint(0, 0)};
    
    for (int i = 1; i < 3; ++i) {
        for (int dir = 0; dir < 6; ++dir) {
            if (hexagones[0]->getVoisinIndice(dir) == hexagones[i]) {
                positions[i] = Utils::getCentreVoisin(QPoint(0, 0), dir, radiusHex);
                break;
            }
        }
    }
    
    // Centrer sur le barycentre
    QPoint center((positions[0].x() + positions[1].x() + positions[2].x()) / 3,
                  (positions[0].y() + positions[1].y() + positions[2].y()) / 3);
    QPoint offset = centerPos - center;
    
    // Afficher les 3 hexagones
    for (int i = 0; i < 3; ++i) {
        HexItem* item = new HexItem(hexagones[i], positions[i] + offset, radiusHex);
        scene->addItem(item);
        connect(item, &HexItem::hexagonClicked, this, &HexView::onHexItemClicked);
    }
}
void HexView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;

    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

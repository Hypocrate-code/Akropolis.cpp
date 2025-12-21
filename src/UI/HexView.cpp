#include "UI/HexView.hpp"
#include "UI/HexItem.hpp"
#include "Tuile.hpp"
#include <utility>
#include <unordered_set>
#include <stack>

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

        HexItem* item = new HexItem(h, pos, radiusHex);
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
    
    // Récupérer le masque binaire des voisins (bit 7=S, bit 6=SO, ..., bit 2=SE)
    uint8_t mask = hexagones[0]->getVoisinsNonFantomeBin();
    int directions[2], idx = 0;
    for (int dir = 0; dir < 6 && idx < 2; ++dir) {
        if (mask & (1 << (7 - dir))) directions[idx++] = dir;
    }
    
    // Calculer positions relatives
    QPoint positions[3] = {
        QPoint(0, 0),
        Utils::getCentreVoisin(QPoint(0, 0), directions[0], radiusHex),
        Utils::getCentreVoisin(QPoint(0, 0), directions[1], radiusHex)
    };
    
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
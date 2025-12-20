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
#include <QCheckBox>
#include <QFontDatabase>

class AkrCheckBox : public QCheckBox
{
private:
    int idFont = QFontDatabase::addApplicationFont(":assets/fonts/Jost/static/Jost-Regular.ttf");
public:
    AkrCheckBox(QString t);
    ~AkrCheckBox();
};

#pragma once

#include <QWidget>
#include <QLineEdit>

class PlayerCountPage : public QWidget {
    Q_OBJECT
public:
    PlayerCountPage(QWidget* parent = nullptr);
signals:
    void playerCountConfirmed( uint32_t count );
private slots:
    void onConfirmClicked();

private:

    QLineEdit* lineEdit;
};
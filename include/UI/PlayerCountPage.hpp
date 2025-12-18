#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "Jeu.hpp"
#include <QMessageBox>

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
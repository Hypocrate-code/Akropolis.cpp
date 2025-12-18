#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <vector>
#include <QVector>
#include <string_view>
#include "UI/GamePushButton.hpp"
class PlayerNamePage : public QWidget {
    Q_OBJECT
public:
    PlayerNamePage(QWidget* parent = nullptr);


    void setPlayerCount(int count);

signals:
    void playerNamesConfirmed(const std::vector<std::string_view>& names);
private slots:
    void onConfirmClicked();

private:
    QVBoxLayout* layout;
    GamePushButton* confirmBtn;
    QVector<QLineEdit*> edits;
};
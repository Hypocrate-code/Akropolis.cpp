#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <vector>
#include <QVector>
#include <string_view>

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
    QVector<QLineEdit*> edits;
};
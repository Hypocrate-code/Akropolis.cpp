//
// Created by thiba on 06/11/2025.
//
#include <iostream>
#include "Tuile.hpp"

#include "Exception.hpp"
#include "Jeu.hpp"
#include <QApplication>

#include "UI/mainwindow.h"

#include <QLocale>
#include <QTranslator>
#include <execution>

int main(int argc, char *argv[])
{
    try
    {
        Jeu *j = Jeu::getInstance();

        if (argc > 1 && std::string(argv[1]) == "--no-gui")
        {
            j->setQtDisplay(false);
            j->StartMenuC();
            j->Lancer();

            std::cout << std::endl
                      << "Fin d'exécution d'Akropolis.cpp" << std::endl;
            j->EndGame();
        }
        else
        {

            QApplication a(argc, argv);
            a.setStyleSheet(R"(
                QWidget {
                    background-color: rgba(244, 244, 244, 1);
                    color: rgba(2, 17, 42, 1);
                }
            )");

            int idFontJostRegular = QFontDatabase::addApplicationFont(":assets/fonts/Jost/static/Jost-Regular.ttf");
            int idFontJostMedium = QFontDatabase::addApplicationFont(":assets/fonts/Jost/static/Jost-Medium.ttf");
            int idFontJostBlack = QFontDatabase::addApplicationFont(":assets/fonts/Jost/static/Jost-Bold.ttf");

            if (idFontJostMedium == -1 || idFontJostRegular == -1 || idFontJostBlack == -1) {
               qWarning() << "Erreur lors du chargement des polices";
            }

            MainWindow w;
            w.show();
            w.setWindowState(Qt::WindowMaximized);
            return a.exec();
        }

        return 0;
    }
    catch (Exception &e)
    {
        std::cout << e.getInfo();
        return 1;
    }
}

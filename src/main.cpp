#include <QApplication>
#include <QLabel>
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Iniciando Health Monitor...\n";
    
    QApplication app(argc, argv);
    
    QLabel *label = new QLabel();
    label->setText("<h1>🏥 Health Monitor</h1>"
                   "<p><b>Estado:</b> ✅ Funcionando</p>"
                   "<p><b>Qt:</b> " QT_VERSION_STR "</p>"
                   "<p><b>Usuario:</b> xx-juancamilo777</p>");
    label->setAlignment(Qt::AlignCenter);
    label->resize(400, 200);
    label->show();
    
    return app.exec();
}

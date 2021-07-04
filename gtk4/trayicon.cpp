#include "trayicon.h"

#include <QApplication>
#include <qsystemtrayicon.h>

void iconActivated(QSystemTrayIcon::ActivationReason reason);

QSystemTrayIcon *trayIcon;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  trayIcon = new QSystemTrayIcon();
  trayIcon->setIcon(
      QIcon("/usr/share/icons/hicolor/32x32/apps/fastfingers.png"));
  trayIcon->setToolTip("FastFingers Cheatsheet");
  trayIcon->setVisible(true);

  a.connect(trayIcon, &QSystemTrayIcon::activated, &a, &iconActivated);

  cheatscreen_init();

  return a.exec();
}

void iconActivated(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
  case QSystemTrayIcon::Trigger:
  case QSystemTrayIcon::DoubleClick:
    cheatscreen_toggle();
    break;
  case QSystemTrayIcon::MiddleClick:
    cheatscreen_toggle();
    break;
  default:;
  }
}

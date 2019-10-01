#!/usr/bin/env python3

from monitorsettings import MonitorSettings
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5 import uic

class Ui(QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        uic.loadUi('kscreensystem.ui', self)
        self.show()

app = QApplication([])
window = Ui()
app.exec_()

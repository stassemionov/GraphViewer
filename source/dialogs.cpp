#include "include/dialogs.h"
#include "include/service.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>

// *** StepSpecifingDialog class implementation *** //

StepSpecifingDialog::StepSpecifingDialog(const double *step_init, QWidget *parent)
    : QDialog(parent), m_current_step(step_init)
{
    this->setWindowTitle("Установка шага сетки");
    this->setFixedSize(QSize(250, 100));
    this->setModal(true);
    QVBoxLayout* layout = new QVBoxLayout;
    this->setLayout(layout);
    m_spin_box = new QDoubleSpinBox;
    m_spin_box->setMinimum(0.0);
    m_spin_box->setValue(*m_current_step);
    m_spin_box->setDecimals(3);
    m_spin_box->setSingleStep(0.001);
    layout->addWidget(m_spin_box);
    QHBoxLayout* hlayout = new QHBoxLayout;
    layout->addLayout(hlayout);
    m_button = new QPushButton("OK");
    hlayout->addWidget(m_button);
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(accept()));
    QPushButton* cancel_button = new QPushButton("Отмена");
    hlayout->addWidget(cancel_button);
    connect(cancel_button, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

StepSpecifingDialog::~StepSpecifingDialog()
{
    delete m_spin_box;
    delete m_button;
}

double StepSpecifingDialog::getValue() const
{
    return m_spin_box->value();
}


// *** DataEditionDialog class implementation *** //

DataEditionDialog::DataEditionDialog(const Points& init_points,
                                     QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Редактор входных данных");
    this->setMinimumSize(QSize(300, 200));
    this->setMaximumSize(QSize(300, QApplication::desktop()->height()));
    this->setModal(true);
    QVBoxLayout* layout = new QVBoxLayout;
    this->setLayout(layout);
    m_text_edit = new QTextEdit();
    m_text_edit->setText(convertToString(init_points));
    QTextCursor cursor = m_text_edit->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_text_edit->setTextCursor(cursor);
    layout->addWidget(m_text_edit);
    QHBoxLayout* hlayout = new QHBoxLayout;
    layout->addLayout(hlayout);
    m_button = new QPushButton("Установить");
    hlayout->addWidget(m_button);
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(accept()));
    QPushButton* cancel_button = new QPushButton("Отмена");
    hlayout->addWidget(cancel_button);
    connect(cancel_button, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

DataEditionDialog::~DataEditionDialog()
{
    delete m_button;
    delete m_text_edit;
}

bool DataEditionDialog::getValue(Points& points) const
{
    QString all_text = m_text_edit->toPlainText();
    QTextStream stream(&all_text, QIODevice::ReadOnly);
    return readPoints(stream, points);
}

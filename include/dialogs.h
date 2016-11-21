#ifndef DIALOGS_H
#define DIALOGS_H

#include "include/points.h"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDialog>
#include <QTextEdit>

class StepSpecifingDialog : public QDialog
{

    Q_OBJECT

public:
    explicit StepSpecifingDialog(const double* step_init, QWidget *parent = nullptr);

    ~StepSpecifingDialog();

    double getValue() const;

private:
    QDoubleSpinBox* m_spin_box = nullptr;
    QPushButton* m_button = nullptr;
    const double* m_current_step = nullptr;
};

class DataEditionDialog : public QDialog
{

    Q_OBJECT

public:
    explicit DataEditionDialog(const Points& init_points, QWidget *parent = nullptr);
    ~DataEditionDialog();

    bool getValue(Points& points) const;

private:
    QTextEdit* m_text_edit = nullptr;
    QPushButton* m_button = nullptr;
};

#endif // DIALOGS_H

/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is
 * 'psimpl - generic n-dimensional polyline simplification'.
 *
 * The Initial Developer of the Original Code is
 * Elmar de Koning.
 * Portions created by the Initial Developer are Copyright (C) 2010-2011
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/*
    psimpl - generic n-dimensional polyline simplification
    Copyright (C) 2010-2011 Elmar de Koning, edekoning@gmail.com

    This file is part of psimpl, and is hosted at SourceForge:
    http://sourceforge.net/projects/psimpl/
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DPWorker.h"
#include <cmath>
#include <limits>
#include <QTextStream>
#include <QToolButton>
#include <QFileDialog>


namespace psimpl {

    MainWindow::MainWindow (QWidget *parent) :
        QMainWindow (parent),
        ui (new Ui::MainWindow)
    {
        mWorker = new DPWorker (this);

        ui->setupUi (this);
        ui->polyTypeComboBox->setCurrentIndex(VECTOR_DOUBLE);

#ifndef _DEBUG
        ui->algorithmComboBox->removeItem (DOUGLAS_PEUCKER_REFERENCE);
#endif
        connect (mWorker, SIGNAL (SignalGeneratingPolyline ()),
                 this, SLOT (SlotGeneratingPolyline ()));
        connect (mWorker, SIGNAL (SignalConvertingPolyline ()),
                 this, SLOT (SlotConvertingPolyline ()));
        connect (mWorker, SIGNAL (SignalSimplifyingPolyline ()),
                 this, SLOT (SlotSimplifyingPolyline ()));
        connect (mWorker, SIGNAL (SignalCleaningConvertedPolyline ()),
                 this, SLOT (SlotCleaningConvertedPolyline ()));

        connect (mWorker, SIGNAL (SignalGeneratedPolyline (int, QVector <qreal>&)),
                 this, SLOT (SlotGeneratedPolyline (int, QVector <qreal>&)));
        connect (mWorker, SIGNAL (SignalSimplifiedPolyline (int, QVector <qreal>&)),
                 this, SLOT (SlotSimplifiedPolyline (int, QVector <qreal>&)));
        connect (mWorker, SIGNAL (SignalSimplifiedPolyline (int, QVector <qreal>&, double, double, double, double)),
                 this, SLOT (SlotSimplifiedPolyline (int, QVector <qreal>&, double, double, double, double)));
    }

    MainWindow::~MainWindow ()
    {
        delete ui;
        delete mWorker;
    }

    void MainWindow::changeEvent (QEvent *e)
    {
        QMainWindow::changeEvent (e);

        switch (e->type ()) {
        case QEvent::LanguageChange:
            ui->retranslateUi (this);
            break;

        default:
            break;
        }
    }

    void MainWindow::EnableButtons ()
    {
        ui->generatePushButton->setEnabled (true);
        ui->simplifyPushButton->setEnabled (true);
        ui->togglePushButton->setEnabled(
            ui->generatedPolylineCheckBox->isChecked () !=
            ui->simplifiedPolylineCheckBox->isChecked ());
        ui->loadPolylineButton->setEnabled (true);
        ui->savePolylineButton->setEnabled (true);
        ui->saveSimplificationButton->setEnabled (true);
    }

    void MainWindow::DisableButtons ()
    {
        ui->generatePushButton->setDisabled (true);
        ui->simplifyPushButton->setDisabled (true);
        ui->togglePushButton->setDisabled (true);
        ui->loadPolylineButton->setDisabled (true);
        ui->savePolylineButton->setDisabled (true);
        ui->saveSimplificationButton->setDisabled (true);
    }

    void MainWindow::on_generatePushButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();
        mWorker->Generate (ui->polyPointCountSpinBox->value ());
    }

    void MainWindow::on_simplifyPushButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();

        switch (ui->algorithmComboBox->currentIndex ())
        {
        case NTH_POINT:
            mWorker->SimplifyNP ((Container)ui->polyTypeComboBox->currentIndex (), ui->npSpinBox->value ());
            break;

        case RADIAL_DISTANCE:
            mWorker->SimplifyRD ((Container)ui->polyTypeComboBox->currentIndex (), ui->rdLineEdit->text ());
            break;

        case PERPENDICULAR_DISTANCE:
            mWorker->SimplifyPD ((Container)ui->polyTypeComboBox->currentIndex (), ui->pdLineEdit->text (), ui->pdSpinBox->value ());
            break;

        case REUMANN_WITKAM:
            mWorker->SimplifyRW ((Container)ui->polyTypeComboBox->currentIndex (), ui->rwLineEdit->text ());
            break;

        case OPHEIM:
            mWorker->SimplifyOp ((Container)ui->polyTypeComboBox->currentIndex (), ui->opMinLineEdit->text (), ui->opMaxLineEdit->text ());
            break;

        case LANG:
            mWorker->SimplifyLa ((Container)ui->polyTypeComboBox->currentIndex (), ui->laLineEdit->text (), ui->laLookAheadSpinBox->value ());
            break;

        case DOUGLAS_PEUCKER_CLASSIC:
            mWorker->SimplifyDP_classic ((Container)ui->polyTypeComboBox->currentIndex (), ui->dpcLineEdit->text ());
            break;

        case DOUGLAS_PEUCKER:
            mWorker->SimplifyDP ((Container)ui->polyTypeComboBox->currentIndex (), ui->dpLineEdit->text ());
            break;

        case DOUGLAS_PEUCKER_N:
            mWorker->SimplifyDP_N ((Container)ui->polyTypeComboBox->currentIndex (), ui->dpvSpinBox->value ());
            break;

        case DOUGLAS_PEUCKER_REFERENCE:
            mWorker->SimplifyDP_reference (ui->dprLineEdit->text ());
            break;
        }
    }

    void MainWindow::on_algorithmComboBox_currentIndexChanged (int index)
    {
        if (index == DOUGLAS_PEUCKER_REFERENCE) {
            ui->polyTypeComboBox->setDisabled (true);
        }
        else {
            ui->polyTypeComboBox->setEnabled (true);
        }
    }

    void MainWindow::on_generatedPolylineCheckBox_toggled (bool checked)
    {
        ui->renderArea->SetVisibleGeneratedPolyline (checked);
        ui->togglePushButton->setDisabled(
            ui->generatedPolylineCheckBox->isChecked () ==
            ui->simplifiedPolylineCheckBox->isChecked ());
        update();
    }

    void MainWindow::on_simplifiedPolylineCheckBox_toggled (bool checked)
    {
        ui->renderArea->SetVisibleSimplifiedPolyline (checked);
        ui->togglePushButton->setDisabled(
            ui->generatedPolylineCheckBox->isChecked () ==
            ui->simplifiedPolylineCheckBox->isChecked ());
        update();
    }

    void MainWindow::on_keepAspectRatioCheckBox_toggled (bool checked)
    {
        ui->renderArea->SetKeepAspectRatio (checked);
        update();
    }

    void MainWindow::on_loadPolylineButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();

        QString filename =
            QFileDialog::getOpenFileName (
                this, tr ("Load generated polyline"), "", tr ("CSV (*.poly)"));

        QFile file (filename);
        if (!file.open (QIODevice::ReadOnly | QIODevice::Text)) {
            ui->statusBar->showMessage (QString ("Failed to open %1").arg (filename));
        }
        else {
            mWorker->mGeneratedCoords.clear ();
            mWorker->mSimplifiedCoords.clear ();

            QTextStream in (&file);
            while (!in.atEnd ()) {
                QString line = in.readLine ();
                QStringList list = line.split (',', QString::SkipEmptyParts);
                if (list.size () < 2) {
                    break;
                }
                mWorker->mGeneratedCoords.push_back (list [0].toDouble ());
                mWorker->mGeneratedCoords.push_back (list [1].toDouble ());
            }
            ui->statusBar->showMessage (QString ("Loaded %1").arg (filename));
            ui->renderArea->SetGeneratedPolyline (mWorker->mGeneratedCoords);
            ui->simplGroupBox->setEnabled (true);
        }
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }

    void MainWindow::on_savePolylineButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();

        QString filename =
            QFileDialog::getSaveFileName (
                this, tr ("Save generated polyline"), "", tr ("CSV (*.poly)"));

        QFile file (filename);
        if (!file.open (QIODevice::WriteOnly | QIODevice::Text)) {
            ui->statusBar->showMessage (QString ("Failed to open %1").arg (filename));
        }
        else {
            QTextStream out (&file);
            out.setRealNumberPrecision (std::numeric_limits <double>::digits10 + 1);
            for (int i=0; i<mWorker->mGeneratedCoords.size (); i+=2)
            {
                out << mWorker->mGeneratedCoords [i] << ',' << mWorker->mGeneratedCoords [i+1] << '\n';
            }
            file.close ();
            ui->statusBar->showMessage (QString ("Saved %1").arg (filename));
        }
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();

        QFileInfo info (file);
        filename = info.completeBaseName ().append(".algo");
        file.setFileName(info.path ().append ("/").append (filename));

        if (file.open (QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out (&file);
            out.setDevice (&file);
            out << "simplify_nth_point," << ui->npSpinBox->value () << '\n';
            out << "simplify_radial_distance," << ui->rdLineEdit->text () << '\n';
            out << "simplify_perpendicular_distance," << ui->pdLineEdit->text () << ',' << ui->pdSpinBox->value () << '\n';
            out << "simplify_reumann_witkam," << ui->rwLineEdit->text () << '\n';
            out << "simplify_opheim," << ui->opMinLineEdit->text () << ',' << ui->opMaxLineEdit->text () << '\n';
            out << "simplify_lang," << ui->laLineEdit->text () << ',' << ui->laLookAheadSpinBox->value () << '\n';
            out << "simplify_douglas_peucker_classic," << ui->dpcLineEdit->text () << '\n';
            out << "simplify_douglas_peucker," << ui->dpLineEdit->text () << '\n';
            out << "simplify_douglas_peucker_n," << ui->dpvSpinBox->value () << '\n';
            file.close ();
        }
    }

    void MainWindow::on_saveSimplificationButton_clicked ()
    {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        DisableButtons ();

        QString filename =
            QFileDialog::getSaveFileName (
                this, tr ("Save simplified polyline"), "", tr ("CSV (*.simpl)"));

        QFile file (filename);
        if (!file.open (QIODevice::WriteOnly | QIODevice::Text)) {
            ui->statusBar->showMessage (QString ("Failed to open %1").arg (filename));
        }
        else {
            QTextStream out (&file);
            for (int i=0; i<mWorker->mSimplifiedCoords.size (); i+=2)
            {
                out << mWorker->mSimplifiedCoords [i] << ',' << mWorker->mSimplifiedCoords [i+1] << '\n';
            }
            file.close ();
            ui->statusBar->showMessage (QString ("Saved %1").arg (filename));
        }
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }

    void MainWindow::SlotGeneratingPolyline () {
        ui->statusBar->showMessage ("Generating polyline...");
    }

    void MainWindow::SlotConvertingPolyline () {
        ui->statusBar->showMessage (QString ("Converting polyline to '%1'...").arg (ui->polyTypeComboBox->currentText ()));
    }

    void MainWindow::SlotSimplifyingPolyline () {
        ui->statusBar->showMessage ("Simplifying polyline...");
    }

    void MainWindow::SlotCleaningConvertedPolyline () {
        ui->statusBar->showMessage ("Deleting converted polyline...");
    }

    void MainWindow::SlotGeneratedPolyline (int duration, QVector <qreal>& polyline)
    {
        ui->statusBar->showMessage (QString ("Generation took %1 ms").arg (duration));
        ui->renderArea->SetGeneratedPolyline (polyline);
        ui->simplGroupBox->setEnabled (true);
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }

    void MainWindow::SlotSimplifiedPolyline (int duration, QVector <qreal>& polyline)
    {
        int pointCount = polyline.count () / 2;
        ui->maxValueLabel->setText ("-");
        ui->sumValueLabel->setText ("-");
        ui->meanValueLabel->setText ("-");
        ui->stdValueLabel->setText ("-");
        ui->statusBar->showMessage (
            QString ("Simplification took %1 ms; %2 (%3%) points remaining").
                arg (duration).
                arg (pointCount).
                arg (100.0 * pointCount / mWorker->GetGeneratedPointCount()));
        ui->renderArea->SetSimplifiedPolyline(polyline);
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }

    void MainWindow::SlotSimplifiedPolyline (int duration, QVector <qreal>& polyline, double max, double sum, double mean, double std)
    {
        int pointCount = polyline.count () / 2;
        ui->maxValueLabel->setText (QString::number (max));
        ui->sumValueLabel->setText (QString::number (sum));
        ui->meanValueLabel->setText (QString::number (mean));
        ui->stdValueLabel->setText (QString::number (std));
        ui->statusBar->showMessage (
            QString ("Simplification took %1 ms; %2 (%3%) points remaining").
                arg (duration).
                arg (pointCount).
                arg (100.0 * pointCount / mWorker->GetGeneratedPointCount()));
        ui->renderArea->SetSimplifiedPolyline(polyline);
        EnableButtons ();
        QApplication::restoreOverrideCursor ();
        update();
    }
} // namespace psimpl

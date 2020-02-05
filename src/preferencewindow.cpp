/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "include/preferencewindow.hpp"
#include "../ui/ui_preferencewindow.h"

#include "Extensions/EditorTheme.hpp"
#include <QAction>
#include <QCXXHighlighter>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPythonCompleter>
#include <QPythonHighlighter>
#include <QSaveFile>

#include <QDebug>

#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerpython.h>

PreferenceWindow::PreferenceWindow(Settings::SettingManager *manager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PreferenceWindow)
{
    ui->setupUi(this);
    this->manager = manager;
    setWindowTitle("Preferences");

    editor = new QsciScintilla();
    oldEditor = new QCodeEditor();

	useQcodeEditor = !manager->useScintilla();

    editor->setMarginType(1, QsciScintilla::MarginType::NumberMargin);
    editor->setMarginWidth(1, "000");
    editor->setFolding(QsciScintilla::FoldStyle::BoxedTreeFoldStyle);

    editor->setAutoCompletionSource(QsciScintilla::AutoCompletionSource::AcsAll);

    editor->setAutoCompletionThreshold(3);
    editor->setAutoCompletionReplaceWord(false);
    editor->setAutoCompletionCaseSensitivity(true);

    editor->SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 0);
    
	if (useQcodeEditor)
        ui->verticalLayout_3->insertWidget(0, oldEditor);
    else
        ui->verticalLayout_3->insertWidget(0, editor);

    connect(ui->snippets, SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(onCurrentSnippetChanged(const QString &)));
    connect(ui->snippets_lang, SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(onSnippetsLangChanged(const QString &)));

    applySettingsToui();
    resize(QDesktopWidget().availableGeometry(this).size() * 0.45);
    setConstraints();
    applySettingsToEditor();
}

void PreferenceWindow::setConstraints()
{
    ui->tab_length->setMinimum(1);
    ui->tab_length->setMaximum(20);

    ui->time_limit->setMinimum(1);
    ui->time_limit->setMaximum(3600000);

    ui->companion_port->setMinimum(10000);
    ui->companion_port->setMaximum(65535);

    ui->transparency_slider->setMinimum(60);
    ui->transparency_slider->setMaximum(100);
}

void PreferenceWindow::updateSnippets()
{
    ui->snippets->clear();
    auto lang = ui->snippets_lang->currentText();
    auto names = manager->getSnippetsNames(lang);
    ui->snippets->addItems(names);
}

void PreferenceWindow::switchToSnippet(const QString &text)
{
    updateSnippets();
    int index = ui->snippets->findText(text);
    if (index != -1)
        ui->snippets->setCurrentIndex(index);
}

void PreferenceWindow::applySettingsToui()
{
    ui->tabWidget->setCurrentIndex(0);

    ui->editor_theme->setCurrentText(manager->getEditorTheme());
    ui->tab_length->setValue(manager->getTabStop());

    if (!manager->getFont().isEmpty())
        currentFont.fromString(manager->getFont());
    ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));

    ui->savetest->setChecked(manager->isSaveTests());
    ui->wrap->setChecked(manager->isWrapText());
    ui->indent->setChecked(manager->isAutoIndent());
    ui->parentheses->setChecked(manager->isAutoParenthesis());
    ui->replace_tabs->setChecked(manager->isTabsReplaced());
    ui->format_on_save->setChecked(manager->isFormatOnSave());
    ui->use_hot_exit->setChecked(manager->isUseHotExit());

    ui->defaultLang->setCurrentText(manager->getDefaultLang());

    ui->cpp_compiler_cmd->setText(manager->getCompileCommandCpp());
    ui->cpp_args_cmd->setText(manager->getRuntimeArgumentsCpp());

    ui->java_compiler_cmd->setText(manager->getCompileCommandJava());
    ui->java_args_cmd->setText(manager->getRuntimeArgumentsJava());
    ui->java_start_cmd->setText(manager->getRunCommandJava());

    ui->python_args_cmd->setText(manager->getRuntimeArgumentsPython());
    ui->python_start_cmd->setText(manager->getRunCommandPython());

    ui->companion_use->setChecked(manager->isCompetitiveCompanionActive());
    ui->companion_new_tab->setChecked(manager->isCompetitiveCompanionOpenNewTab());
    ui->companion_port->setValue(manager->getConnectionPort());

    ui->clang_format_binary->setText(manager->getClangFormatBinary());
    ui->clang_format_style->setPlainText(manager->getClangFormatStyle());

    cppTemplatePath = manager->getTemplatePathCpp();
    pythonTemplatePath = manager->getTemplatePathPython();
    javaTemplatePath = manager->getTemplatePathJava();

    ui->update_startup->setChecked(manager->isCheckUpdateOnStartup());
    ui->beta_update->setChecked(manager->isBeta());

    ui->time_limit->setValue(manager->getTimeLimit());

    ui->cpp_template->setText(cppTemplatePath.isEmpty() ? "<Not selected>" : "..." + cppTemplatePath.right(30));
    ui->py_template->setText(pythonTemplatePath.isEmpty() ? "<Not selected>" : "..." + pythonTemplatePath.right(30));
    ui->java_template->setText(javaTemplatePath.isEmpty() ? "<Not selected>" : "..." + javaTemplatePath.right(30));

    ui->hotkeys->setChecked(manager->isHotkeyInUse());
    on_hotkeys_clicked(manager->isHotkeyInUse());

    ui->compile_hotkey->setKeySequence(manager->getHotkeyCompile());
    ui->run_hotkey->setKeySequence(manager->getHotkeyRun());
    ui->format_hotkey->setKeySequence(manager->getHotkeyFormat());
    ui->compileRun_hotkey->setKeySequence(manager->getHotkeyCompileRun());
    ui->kill_hotkey->setKeySequence(manager->getHotkeyKill());
    ui->toggle_hotkey->setKeySequence(manager->getHotkeyViewModeToggler());
    ui->snippets_hotkey->setKeySequence(manager->getHotkeySnippets());

    ui->transparency_slider->setValue(manager->getTransparency());

    auto lang = manager->getDefaultLang();
    int lang_index = ui->snippets_lang->findText(lang);
    if (lang_index != -1)
        ui->snippets_lang->setCurrentIndex(lang_index);
    onSnippetsLangChanged(lang);

    ui->cf_path->setText(manager->getCFPath());
}

void PreferenceWindow::extractSettingsFromUi()
{
    manager->setEditorTheme(ui->editor_theme->currentText());
    manager->setTabStop(ui->tab_length->value());
    manager->setFont(currentFont.toString());

    manager->setSaveTests(ui->savetest->isChecked());
    manager->setWrapText(ui->wrap->isChecked());
    manager->setAutoIndent(ui->indent->isChecked());
    manager->setAutoParenthesis(ui->parentheses->isChecked());
    manager->setTabsReplaced(ui->replace_tabs->isChecked());
    manager->formatOnSave(ui->format_on_save->isChecked());
    manager->setUseHotExit(ui->use_hot_exit->isChecked());

    manager->setDefaultLanguage(ui->defaultLang->currentText());

    manager->setCompileCommandsCpp(ui->cpp_compiler_cmd->text());
    manager->setRuntimeArgumentsCpp(ui->cpp_args_cmd->text());

    manager->setCompileCommandsJava(ui->java_compiler_cmd->text());
    manager->setRuntimeArgumentsJava(ui->java_args_cmd->text());
    manager->setRunCommandJava(ui->java_start_cmd->text());

    manager->setRunCommandPython(ui->python_start_cmd->text());
    manager->setRuntimeArgumentsPython(ui->python_args_cmd->text());

    manager->setClangFormatBinary(ui->clang_format_binary->text());
    manager->setClangFormatStyle(ui->clang_format_style->toPlainText());

    manager->setCompetitiveCompanionActive(ui->companion_use->isChecked());
    manager->setCompetitiveCompanionOpenNewTab(ui->companion_new_tab->isChecked());
    manager->setConnectionPort(ui->companion_port->value());

    manager->setBeta(ui->beta_update->isChecked());
    manager->checkUpdateOnStartup(ui->update_startup->isChecked());

    manager->setTimeLimit(ui->time_limit->value());

    manager->setTemplatePathCpp(cppTemplatePath);
    manager->setTemplatePathJava(javaTemplatePath);
    manager->setTemplatePathPython(pythonTemplatePath);

    manager->setHotKeyInUse(ui->hotkeys->isChecked());

    manager->setHotkeyRun(ui->run_hotkey->keySequence());
    manager->setHotkeyKill(ui->kill_hotkey->keySequence());
    manager->setHotkeyFormat(ui->format_hotkey->keySequence());
    manager->setHotkeyCompile(ui->compile_hotkey->keySequence());
    manager->setHotkeyCompileRun(ui->compileRun_hotkey->keySequence());
    manager->setHotkeyViewModeToggler(ui->toggle_hotkey->keySequence());
    manager->setHotkeySnippets(ui->snippets_hotkey->keySequence());

    manager->setCFPath(ui->cf_path->text());
}

void PreferenceWindow::updateShow()
{
    applySettingsToui();
    show();
    raise();
}

PreferenceWindow::~PreferenceWindow()
{
    delete ui;
    delete editor;
}

void PreferenceWindow::on_ok_clicked()
{
    extractSettingsFromUi();
    close();
    emit settingsApplied();
    applySettingsToEditor();
}

void PreferenceWindow::on_cancel_clicked()
{
    close();
}

void PreferenceWindow::on_apply_clicked()
{
    extractSettingsFromUi();
    emit settingsApplied();
    applySettingsToEditor();
}

void PreferenceWindow::on_hotkeys_clicked(bool checked)
{
    ui->compile_hotkey->setEnabled(checked);
    ui->compileRun_hotkey->setEnabled(checked);
    ui->run_hotkey->setEnabled(checked);
    ui->kill_hotkey->setEnabled(checked);
    ui->format_hotkey->setEnabled(checked);
    ui->toggle_hotkey->setEnabled(checked);
    ui->snippets_hotkey->setEnabled(checked);
}

void PreferenceWindow::on_font_button_clicked()
{
    bool ok = false;
    QFont fp = QFontDialog::getFont(&ok, currentFont);

    if (ok)
    {
        currentFont = fp;
        ui->font_button->setText(currentFont.family() + " " + QString::number(currentFont.pointSize()));
    }
}

void PreferenceWindow::on_cpp_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose C++ template File"), "",
                                                 "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)");
    if (filename.isEmpty())
        return;
    cppTemplatePath = filename;
    ui->cpp_template->setText("..." + cppTemplatePath.right(30));
}

void PreferenceWindow::on_py_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Python template File"), "", "Python Files (*.py)");
    if (filename.isEmpty())
        return;
    pythonTemplatePath = filename;
    ui->py_template->setText("..." + pythonTemplatePath.right(30));
}

void PreferenceWindow::on_java_template_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Choose Java template File"), "", "Java Files (*.java)");
    if (filename.isEmpty())
        return;
    javaTemplatePath = filename;
    ui->java_template->setText("..." + javaTemplatePath.right(30));
}

void PreferenceWindow::on_load_snippets_from_files_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    QString fileType = "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)";
    if (lang == "Java")
        fileType = "Java Files (*.java)";
    else if (lang == "Python")
        fileType = "Python Files (*.py)";
    QStringList filenames = QFileDialog::getOpenFileNames(
        this, "Select one or more files to be loaded as snippets for " + lang, "", fileType);
    for (auto &filename : filenames)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "File load error", "Failed to read " + filename);
        }
        else
        {
            auto snippetName = QFileInfo(file).baseName();
            if (snippetName.isEmpty())
                snippetName = QFileInfo(file).fileName();
            if (ui->snippets->findText(snippetName) != -1)
                snippetName = getNewSnippetName(lang, snippetName);
            if (!snippetName.isEmpty())
            {
                manager->setSnippet(lang, snippetName, file.readAll());
                switchToSnippet(snippetName);
            }
            file.close();
        }
    }
}

void PreferenceWindow::on_extract_snippets_to_files_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    QString suffix = ".cpp";
    QString fileType = "C++ Files (*.cpp *.hpp *.h *.cc *.cxx *.c)";
    if (lang == "Java")
    {
        suffix = ".java";
        fileType = "Java Files (*.java)";
    }
    else if (lang == "Python")
    {
        suffix = ".py";
        fileType = "Python Files (*.py)";
    }
    auto dirPath = QFileDialog::getExistingDirectory(this, "Choose a directory to extract snippets to");
    if (QFile::exists(dirPath))
    {
        QDir dir(dirPath);
        auto names = manager->getSnippetsNames(lang);
        for (auto name : names)
        {
            auto content = manager->getSnippet(lang, name);
            auto filePath = dir.filePath(name + suffix);
            if (QFile::exists(filePath))
                filePath = QFileDialog::getSaveFileName(this, "Save snippet " + name + " to:", dirPath, fileType);
            while (!filePath.isEmpty())
            {
                QSaveFile saveFile(filePath);
                saveFile.open(QIODevice::WriteOnly | QIODevice::Text);
                saveFile.write(content.toStdString().c_str());
                if (saveFile.commit())
                    break;
                QMessageBox::warning(this, "File save error", "Failed to save to " + filePath);
                filePath = QFileDialog::getSaveFileName(this, "Save snippet " + name + " to:", dirPath, fileType);
            }
        }
    }
}

void PreferenceWindow::onSnippetsLangChanged(const QString &lang)
{
    updateSnippets();
    if (lang == "Python")
    {
        oldEditor->setHighlighter(new QPythonHighlighter);
        oldEditor->setCompleter(new QPythonCompleter);

        editor->setLexer(new QsciLexerPython);
    }

    else if (lang == "Java")
    {
        oldEditor->setHighlighter(new QCXXHighlighter);
        oldEditor->setCompleter(nullptr);

        editor->setLexer(new QsciLexerJava);
    }

    else
    {
        oldEditor->setHighlighter(new QCXXHighlighter);
        oldEditor->setCompleter(nullptr);

        editor->setLexer(new QsciLexerCPP);
    }

    // Lexer reset the colors
    editor->setUnmatchedBraceForegroundColor(Qt::red);
    editor->setMatchedBraceBackgroundColor(Qt::darkCyan);
    editor->setMatchedBraceForegroundColor(Qt::white);
    editor->setBraceMatching(QsciScintilla::BraceMatch::SloppyBraceMatch);

    // Todo: Everything except the CallTips is working.
}

void PreferenceWindow::onCurrentSnippetChanged(const QString &text)
{
    auto lang = ui->snippets_lang->currentText();
    auto content = manager->getSnippet(lang, text);

    if (useQcodeEditor)
    {
        oldEditor->setFocus(Qt::OtherFocusReason);
        oldEditor->setPlainText(content);
    }
    else
    {
        editor->setText(content);
        editor->setFocus(Qt::OtherFocusReason);
    }
}

void PreferenceWindow::applySettingsToEditor()
{
    auto data = manager->toData();

    editor->setTabIndents(data.isTabsReplaced);
    editor->setTabWidth(data.tabStop);
    editor->setAutoIndent(data.isAutoIndent);

    oldEditor->setTabReplace(data.isTabsReplaced);
    oldEditor->setTabReplaceSize(data.tabStop);
    oldEditor->setAutoIndentation(data.isAutoIndent);
    oldEditor->setAutoParentheses(data.isAutoParenthesis);

    if (!data.font.isEmpty())
    {
        QFont font;
        font.fromString(data.font);
        editor->setFont(font);
        oldEditor->setFont(font);
    }

    editor->setTabWidth(data.tabStop);
    const int tabStop = data.tabStop;

    QFontMetrics metric(editor->font());
    oldEditor->setTabReplaceSize(tabStop);
    oldEditor->setTabStopDistance(tabStop * metric.horizontalAdvance("9"));

    if (data.isWrapText)
    {
        editor->setWrapMode(QsciScintilla::WrapMode::WrapWord);
        oldEditor->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
        editor->setWrapMode(QsciScintilla::WrapMode::WrapNone);
        oldEditor->setWordWrapMode(QTextOption::NoWrap);
    }

    if (data.editorTheme == "Light")
        oldEditor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (data.editorTheme == "Drakula")
        oldEditor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (data.editorTheme == "Monkai")
        oldEditor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (data.editorTheme == "Solarised")
        oldEditor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (data.editorTheme == "Solarised Dark")
        oldEditor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
        oldEditor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
}

void PreferenceWindow::on_snippet_save_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    if (ui->snippets->currentIndex() != -1)
    {
        auto name = ui->snippets->currentText();
        manager->setSnippet(lang, name, useQcodeEditor ? oldEditor->toPlainText() : editor->text());
    }
    else
    {
        auto name = getNewSnippetName(lang);
        if (!name.isEmpty())
        {
            QString content;
            if (useQcodeEditor)
                content = oldEditor->toPlainText();
            else
                content = editor->text();
            manager->setSnippet(lang, name, content);
            switchToSnippet(name);
        }
    }
}

void PreferenceWindow::on_snippet_new_clicked()
{
    auto lang = ui->snippets_lang->currentText();
    auto name = getNewSnippetName(lang);
    if (!name.isEmpty())
    {
        manager->setSnippet(lang, name, "");
        switchToSnippet(name);
    }
}

void PreferenceWindow::on_snippet_delete_clicked()
{
    int index = ui->snippets->currentIndex();
    if (index != -1)
    {
        auto name = ui->snippets->currentText();
        auto res = QMessageBox::question(this, "Delete?", "Do you want to delete the snippet " + name + "?");
        if (res == QMessageBox::Yes)
        {
            auto lang = ui->snippets_lang->currentText();
            ui->snippets->removeItem(index);
            manager->removeSnippet(lang, name);
        }
    }
}

void PreferenceWindow::on_snippet_rename_clicked()
{
    if (ui->snippets->currentIndex() != -1)
    {
        auto lang = ui->snippets_lang->currentText();
        auto name = getNewSnippetName(lang);
        if (!name.isEmpty())
        {
            QString content;

            if (useQcodeEditor)
                content = oldEditor->toPlainText();
            else
                content = editor->text();

            auto currentName = ui->snippets->currentText();
            manager->removeSnippet(lang, currentName);
            manager->setSnippet(lang, name, content);
            switchToSnippet(name);
        }
    }
}

void PreferenceWindow::on_transparency_slider_valueChanged(int value)
{
    manager->setTransparency(value);
    parentWidget()->setWindowOpacity(value / 100.0);
}

QString PreferenceWindow::getNewSnippetName(const QString &lang, const QString &old)
{
    QString label = "New name:";
    if (!old.isEmpty())
        label = "The name " + old + " is used for " + lang + "\n" + label;
    auto name = QInputDialog::getText(this, tr("Snippet Name"), label);
    if (name.isEmpty())
        return QString();
    else if (ui->snippets->findText(name) == -1)
        return name;
    else
        return getNewSnippetName(lang, name);
}

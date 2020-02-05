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

#include "Extensions/CFTools.hpp"
#include <QFileInfo>
#include <QUrl>

namespace Network
{

CFTools::CFTools(QString path, MessageLogger *logger) : path(path)
{
    log = logger;
    icon = new QSystemTrayIcon();
    CFToolProcess = new QProcess();
}

CFTools::~CFTools()
{
    if (icon != nullptr)
        delete icon;
    if (CFToolProcess != nullptr)
        delete CFToolProcess;
}

void CFTools::submit(const QString &filePath, const QString &url, const QString &lang)
{
    QString problemContestId, problemCode;
    QRegularExpression regexContest(".*://codeforces.com/contest/([1-9][0-9]*)/problem/(0|[A-Z][1-9]?)");
    auto matchContest = regexContest.match(url);
    if (matchContest.hasMatch())
    {
        problemContestId = matchContest.captured(1);
        problemCode = matchContest.captured(2);
        if (problemCode == "0")
            problemCode = "A";
    }
    else
    {
        QRegularExpression regexProblemSet(".*://codeforces.com/problemset/problem/([1-9][0-9]*)/([A-Z][1-9]?)");
        auto matchProblemSet = regexProblemSet.match(url);
        if (matchProblemSet.hasMatch())
        {
            problemContestId = matchProblemSet.captured(1);
            problemCode = matchProblemSet.captured(2);
        }
        else
        {
            log->error("CF Tool", "Failed to parse the problem URL " + url);
            return;
        }
    }
    CFToolProcess->setProgram(path);
    CFToolProcess->setArguments({"submit", problemContestId, problemCode, filePath});
    connect(CFToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadReady()));
    CFToolProcess->start();
}

bool CFTools::check(QString path)
{
    QProcess checkProcess;

    checkProcess.start(path, {"--version"});

    bool finished = checkProcess.waitForFinished(2000);
    return finished && checkProcess.exitCode() == 0;
}

void CFTools::updatePath(QString p)
{
    path = p;
}

void CFTools::onReadReady()
{
    QString newResponse = CFToolProcess->readAll();
    auto shortStatus = newResponse.right(newResponse.size() - newResponse.indexOf("status:") - 8);
    if (newResponse.contains("status: Happy New Year") || newResponse.contains("status: Accepted"))
    {
        log->message("CF Tool", shortStatus, "green");
        icon->show();
        icon->showMessage("CF Tool", shortStatus);
        icon->hide();
    }
    else if (newResponse.contains("status: Running on"))
    {
        log->info("CFTool", shortStatus);
    }
    else
    {
        log->error("CF Tool", newResponse);
        icon->show();
        icon->showMessage("CF Tool", newResponse, QSystemTrayIcon::Warning);
        icon->hide();
    }
}

} // namespace Network

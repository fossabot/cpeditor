/*
* Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com> 
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
* Believe Software is "Software" and it isn't not immune to bugs.
* 
*/


#include <MessageLogger.hpp>
#include <QDateTime>

namespace Log {

QPlainTextEdit* MessageLogger::box = nullptr;

void MessageLogger::setContainer(QPlainTextEdit* value) {
  MessageLogger::box = value;
}

void MessageLogger::info(std::string head, std::string body) {
  std::string ans = "<b>[";
  int long long timestamp = QDateTime::currentSecsSinceEpoch();
  auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
  ans += val.toString().toStdString();
  ans += "] [";
  ans += head;
  ans += "] </b>";
  ans += "<font color=blue>[";
  ans += body;
  ans += "]</font>";

  box->appendHtml(QString::fromStdString(ans));
}
void MessageLogger::warn(std::string head, std::string body) {
  std::string ans = "<b>[";
  int long long timestamp = QDateTime::currentSecsSinceEpoch();
  auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
  ans += val.toString().toStdString();
  ans += "] [";
  ans += head;
  ans += "] </b>";
  ans += "<font color=green>[";
  ans += body;
  ans += "]</font>";

  box->appendHtml(QString::fromStdString(ans));
}
void MessageLogger::error(std::string head, std::string body) {
  std::string ans = "<b>[";
  auto timestamp = QDateTime::currentSecsSinceEpoch();
  auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
  ans += val.toString().toStdString();
  ans += "] [";
  ans += head;
  ans += "] </b>";
  ans += "<font color=red>[";
  ans += body;
  ans += "]</font>";

  box->appendHtml(QString::fromStdString(ans));
}
void MessageLogger::clear() {
  box->clear();
}

}  // namespace Log
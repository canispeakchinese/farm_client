#include <qapplication.h>
#include <QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    char* file = (char*)context.file;
    char prefix[] = "farm_client/";
    int len = strlen(prefix);

    if(context.file != NULL) {
        for(int i=0; context.file[i] != '\0'; i++) {
            if(strncmp(&context.file[i], prefix, len) == 0) {
                file = (char*)&context.file[i+len];
                break;
            }
        }
    }

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug:%s (%s:%u) %s\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), file, context.line, localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info:%s (%s:%u) %s\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), file, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning:%s (%s:%u) %s\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), file, context.line, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical:%s (%s:%u) %s\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), file, context.line, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal:%s (%s:%u) %s\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), file, context.line, localMsg.constData());
        abort();
    }
}

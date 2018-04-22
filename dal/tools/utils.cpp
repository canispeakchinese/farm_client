#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    char* file = (char*)context.file;
    char prefix[] = "../farm/";

    if(context.file != NULL) {
        for(int i=0; context.file[i] != '\0'; i++) {
            if(strncmp(&context.file[i], prefix, 8) == 0) {
                file = (char*)&context.file[i+8];
                break;
            }
        }
    }

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug:(%s:%u) %s\n", file, context.line, localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info:(%s:%u) %s\n", file, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning:(%s:%u) %s\n", file, context.line, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical:(%s:%u) %s\n", file, context.line, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal:(%s:%u) %s\n", file, context.line, localMsg.constData());
        abort();
    }
}

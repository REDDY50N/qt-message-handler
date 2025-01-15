/*!
 *  @brief Custom debug logger
 *
 *  This is a example for a custom message handler.
 *  Consider to make a class if using it in your projects.
 */

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

// -- CLI arguments
constexpr const char* CONSOLELOG_ARG{"--consolelog"};   /// print logs to std out instead redirecting to file file
constexpr const char* LOG_FILE{"/tmp/myapp.log"};       /// logs are written into file if the consolelog arg is not passed

// -- Configure the debug message output format
enum class LogMessageFormat
{
    FullColor,
    Full,
    Standard,
    Minimal,
    None
};

// -- Implement the message formats
void setLogMessageFormat(const LogMessageFormat& format = LogMessageFormat::Standard) {
    switch(format)
    {
        case LogMessageFormat::FullColor:
            /*!
             * Style and color (ANSI-Escape-Codes - Patter: \33[<attribute><color>m):
             * <attribute>: normal or reset = 0 | bold = 1
             * <color>: red =  31 | green = 32 | yellow = 33 | blue = 34 | 35 = magenta | white = 37
             * Example Reset: \033[0m */
                qSetMessagePattern("\033[0;37m%{time yyyy-MM-dd hh:mm:ss} " // DATE in white
                           " %{if-info}\033[0;32m[info]%{endif}"               // INFO in green
                           "%{if-debug}\033[0;34m[debug]%{endif}"              // DEBUG in blue
                           "%{if-warning}\033[1;33m[warning]%{endif}"          // WARNING in yellow and bold
                           "%{if-critical}\033[1;31m[critical]%{endif}"        // CRITICAL in red and bold
                           "%{if-fatal}\033[1;35m[fatal]%{endif}"              // FATAL in magenta and bold
                           " %{message} (%{file}:%{line})"
                           "\033[0m");
        break;
        case LogMessageFormat::Full:
            qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss} [%{type}] %{message} (%{file}:%{line})");
        break;
        case LogMessageFormat::Standard:
            qSetMessagePattern("[%{type}] %{message} (%{file}:%{line})");
        break;
        case LogMessageFormat::Minimal:
            qSetMessagePattern("%{message}");
        break;
        case LogMessageFormat::None:
            qSetMessagePattern("");
        break;
        default:
            qSetMessagePattern("[%{type}] %{message} (%{file}:%{line})");
        break;
    }
}


// -- Message Handler Setup
void messageHandler(const QtMsgType type, const QMessageLogContext &context, const QString &message) {
    QFile file(LOG_FILE);

    Q_UNUSED(context); // maybe implement if needed

    // -- Open logfile in append mode
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);

        // --Current timestamp
        const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        // -- Log-Level
        QString level;
        switch (type) {
        case QtDebugMsg:
            level = "[DEBUG]";
            break;
        case QtInfoMsg:
            level = "[INFO]";
            break;
        case QtWarningMsg:
            level = "[WARNING]";
            break;
        case QtCriticalMsg:
            level = "[CRITICAL]";
            break;
        case QtFatalMsg:
            level = "[FATAL]";
            break;
        }

        // -- Log message format
        out << "[" << timestamp << "] " << level << ": " << message << "\n";

        file.close();
    }
}


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // -- For reading the cli args into list
    QStringList args {};

    // -- Parse cli arguments
    for ( int argi = 0; argi < argc;  argi++ ) {
        args.append( argv[argi] );
    }

    // -- Install custom message logger
    if ( ! args.contains( CONSOLELOG_ARG ) ) {
        qInfo() << "Debug output will be stored in" << LOG_FILE << "\nUse --consolelog to print debug logs on console.";
        qInstallMessageHandler(messageHandler);
        setLogMessageFormat(LogMessageFormat::Full);
    }
    else {
        setLogMessageFormat(LogMessageFormat::FullColor);
    }


    // -- Example usage
    qDebug() << "Nothing to see here ...";
    qInfo() << "For your information ...";
    qWarning() << "This is the final warning!";
    qCritical() << "Houston we have a big problem!";

    return QCoreApplication::exec();
}

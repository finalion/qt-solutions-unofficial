#include "widget.h"

#include <QApplication>
#include <QProcess>
#include <QDir>

class Restarter
{
public:
    Restarter(int argc, char *argv[], bool ignoreExitCode=false): m_ignoreExitCode(ignoreExitCode)
    {
        Q_UNUSED(argc)
        m_executable = QString::fromLocal8Bit(argv[0]);
        m_workingPath = QDir::currentPath();
        m_restartCode = 0;
    }

    void setRestartCode(int exitCode)
    {
        m_restartCode = exitCode;
    }

    void setArguments(const QStringList &args) { m_args = args; }

    QStringList arguments() const { return m_args; }

    int restartOrExit(int exitCode)
    {
        //如果ignoreExitCode为True, 则无论exitCode为何值，都自动执行重启
        //否则，是否重启由传入的exitCode确定
        if(m_ignoreExitCode)
            return restart(exitCode);
        else
            return (exitCode == m_restartCode)?restart(exitCode): exitCode;
//        return qApp->property("restart").toBool() ? restart(exitCode) : exitCode;
    }

    int restart(int exitCode)
    {
        QProcess::startDetached(m_executable, m_args, m_workingPath);
        return exitCode;
    }

private:
    QString m_executable;
    QStringList m_args;
    QString m_workingPath;
    int m_restartCode;
    bool m_ignoreExitCode;
};

#include "qtsingleapplication.h"
using namespace SharedTools;

int main(int argc, char *argv[])
{
    Restarter restarter(argc, argv);
    restarter.setRestartCode(10);
    QtSingleApplication a("test", argc, argv);
    a.setBlock(false);
    if(a.isRunning())
    {
        qDebug("is running\n");
        a.sendMessage(QString::number(a.applicationPid())+" tried, but failed...");
        return 0;
    }
    Widget w;
    w.show();
    return restarter.restartOrExit(a.exec());
}

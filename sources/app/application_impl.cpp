#include "application_impl.h"

#include "service_registry.h"
#include "presentation_manager.h"

#include <QApplication>
#include <QMessageBox>
#include <QLockFile>
#include <QDebug>

using namespace core;


Application::Impl::Impl(Application* parent, QApplication& app, domain::ServiceRegistry& serviceReg,
                        presentation::PresentationManager& presentationMgr) : QObject(parent)
{
    m_isExternalApp = true;
    m_app = &app;

    m_isExternalService = true;
    m_serviceRegistry = &serviceReg;

    m_isExternalPresentationManager = true;
    m_presentationManager = &presentationMgr;
    this->init(m_app->applicationName());
}

Application::Impl::Impl(Application* parent, int &argc, char** argv, const QString& appName) : QObject(parent)
{
    if (QApplication::instance())
    {
        m_isExternalApp = true;
        m_app = QApplication::instance();
    }
    else
    {
        m_isExternalApp = false;
        m_app = new QApplication(argc, argv);
    }
    QApplication::setApplicationName(appName);

    if (domain::ServiceRegistry::instance())
    {
        m_isExternalService = true;
        m_serviceRegistry = domain::ServiceRegistry::instance();
    }
    else
    {
        m_isExternalService = false;
        m_serviceRegistry = new domain::ServiceRegistry();
    }

    if (presentation::PresentationManager::instance())
    {
        m_isExternalPresentationManager = true;
        m_presentationManager = presentation::PresentationManager::instance();
    }
    else
    {
        m_isExternalPresentationManager = false;
        m_presentationManager = new presentation::PresentationManager();
    }
    this->init(appName);
}

int Application::Impl::exec()
{
    return QApplication::exec();
}

void Application::Impl::quit()
{
    QApplication::quit();
}

void Application::Impl::exit(int retcode)
{
    return QApplication::exit(retcode);
}

Application::Impl::~Impl()
{
    if(!m_isExternalPresentationManager && !m_presentationManager.isNull())
        delete m_presentationManager;
    if (!m_isExternalService && m_serviceRegistry)
        delete m_serviceRegistry;
    if (m_lock)
        delete m_lock;

    if (!m_isExternalApp)
    {
        QApplication::quit();
        delete m_app;
    }
    qDebug()<<"app core impl destroyed";
}

void Application::Impl::init(const QString& appName)
{
    m_lock = new QLockFile("lidar.lock");
    if (!m_lock->tryLock())
    {
        QMessageBox::warning(0, appName, appName+tr(" is already running!"));
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        //qFatal("%s is locked!", appName.toStdString().c_str());
    }
}

bool Application::Impl::start()
{
    presentationManager->mainWindow()->show();
    return true;
}

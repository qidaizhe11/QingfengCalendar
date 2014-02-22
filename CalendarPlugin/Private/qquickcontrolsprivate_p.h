
#ifndef QQUICKCONTROLSPRIVATE_P_H
#define QQUICKCONTROLSPRIVATE_P_H

#include "qqml.h"

class QQuickControlsPrivate {

public:

//    inline static QObject *registerTooltipModule(QQmlEngine *engine, QJSEngine *jsEngine)
//    {
//        Q_UNUSED(engine);
//        Q_UNUSED(jsEngine);
//        return new QQuickTooltip();
//    }

    inline static QObject *registerSettingsModule(QQmlEngine *engine, QJSEngine *jsEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(jsEngine);
        return new QQuickControlSettings(engine);
    }

};

#endif // QQUICKCONTROLSPRIVATE_P_H

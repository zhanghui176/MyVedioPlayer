#include "Utility.h"

Utility::Utility() {}

QString Utility::formatSecondToQTime(double seconds) {
    int totalMs = static_cast<int>(seconds * 1000);
    QTime time(0, 0); // 从00:00开始
    time = time.addMSecs(totalMs);

    if (seconds >= 3600) {
        return time.toString("hh:mm:ss");
    } else {
        return time.toString("mm:ss");
    }
}

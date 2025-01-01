#ifndef SSD1306DISPLAY_H
#define SSD1306DISPLAY_H

#include <QObject>
#include <ssd1306_i2c.h>
#include <QMap>

class SSD1306Display : public QObject
{
    Q_OBJECT
public:
    explicit SSD1306Display(const char *i2c_dev, QObject *parent = nullptr);

public slots:
    void setCurrentProgram(unsigned char val);

    void onCurrentPatchChanged(unsigned int id, const QString &name, bool isRequesting);

    void onMsDisconnected(unsigned int id);

private:
    ssd1306_i2c_t *oled{nullptr};
    ssd1306_framebuffer_t *fbp{nullptr};

    QMap<int, QByteArray> currentPatchNamesMap;
    char programNumStr[2];
    bool isRequestingState{false};
    bool lastRequestingState{false};

    void updateDisplay();
};

#endif // SSD1306DISPLAY_H

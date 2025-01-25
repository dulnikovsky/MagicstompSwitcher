#ifndef SSD1306DISPLAY_H
#define SSD1306DISPLAY_H

#include <QObject>
#include <ssd1306_i2c.h>
#include <QMap>

class QTimer;

class SSD1306Display : public QObject
{
    Q_OBJECT
public:
    explicit SSD1306Display(const char *i2c_dev, QObject *parent = nullptr);
    ~SSD1306Display();

public slots:
    void setCurrentProgram(unsigned char val);
    void onCurrentPatchChanged(unsigned int id, const QString &name, bool isRequesting);
    void onMsDisconnected(unsigned int id);
    void setInverted(bool val);

    void setParamEditMode(bool val);

    bool drawParamVal(const QByteArray &paramter, const QByteArray &val);

private slots:
    void finishSplash();

private:
    ssd1306_i2c_t *oled{nullptr};
    ssd1306_framebuffer_t *fbp{nullptr};
    ssd1306_framebuffer_t *param_edit_fbp{nullptr};

    QMap<int, QByteArray> currentPatchNamesMap;
    char programNumStr[2];
    bool isRequestingState{false};
    bool lastRequestingState{false};

    const int splashScreenTimeout{2000};

    QTimer *splashTimer{nullptr};

    void updateDisplay();
};

#endif // SSD1306DISPLAY_H
